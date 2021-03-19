// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2017-2021 Bartosz Golaszewski <bartekgola@gmail.com>

#include <errno.h>
#include <getopt.h>
#include <gpiod.h>
#include <limits.h>
#include <poll.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "tools-common.h"


static void handle_event(unsigned int in_line, struct gpiod_line_bulk *out_lines,
						 struct timespec *ts)
{
	int rv;
	int values[1];

	values[0] = 1; // on
	rv = gpiod_line_set_value_bulk(out_lines, values);
	if (rv)
		die_perror("unable to set GPIO out_lines value");

	values[0] = 0; // off
	rv = gpiod_line_set_value_bulk(out_lines, values);
	if (rv)
		die_perror("unable to set GPIO out_lines value");

	// printf("event occured! pin: %u timestamp: [%8ld.%09ld]\n",
	//        in_line, ts->tv_sec, ts->tv_nsec);
}

static void handle_signal(int signum UNUSED)
{
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	unsigned int offsets[64], num_lines = 1, events_done = 0, x;
	// bool watch_rising = false, watch_falling = false;
	// int flags = 0;
	struct timespec timeout = { 10, 0 };
	int rv, i, y, event_type;
	// struct mon_ctx ctx;
	struct gpiod_chip *chip;
	struct gpiod_line_bulk *in_lines, *evlines, *out_lines;
	// char *end;
	struct gpiod_line_request_config config;
	struct gpiod_line *line;
	struct gpiod_line_event events[16];

	/*
	 * FIXME: use signalfd once the API has been converted to using a single file
	 * descriptor as provided by uAPI v2.
	 */
	signal(SIGINT, handle_signal);
	signal(SIGTERM, handle_signal);


	chip = chip_open_lookup("gpiochip0");
	if (!chip)
		die_perror("unable to open gpiochip0");


	// *************** Input configuration ****************
	offsets[0] = 27; //input GPIO27

	in_lines = gpiod_chip_get_lines(chip, offsets, 1);
	if (!in_lines)
		die_perror("unable to retrieve GPIO lines from chip");

	memset(&config, 0, sizeof(config));

	config.consumer = "gpiomon";
	// config.request_type = event_type;
	config.request_type = GPIOD_LINE_REQUEST_EVENT_RISING_EDGE;
	// config.flags = flags;
	config.flags = 0;

	rv = gpiod_line_request_bulk(in_lines, &config, NULL);
	if (rv)
		die_perror("unable to request GPIO lines for events");

	evlines = gpiod_line_bulk_new(num_lines);
	if (!evlines)
		die("out of memory");



	// *************** Output configuration ****************
	offsets[0] = 17; //output GPIO17
	int values[] = {0}; // 1 - on, 0 - off

	out_lines = gpiod_chip_get_lines(chip, offsets, 1);
	if (!out_lines)
		die_perror("unable to retrieve GPIO lines from chip");

	memset(&config, 0, sizeof(config));

	config.consumer = "gpioset";
	config.request_type = GPIOD_LINE_REQUEST_DIRECTION_OUTPUT;
	config.flags = 0;

	rv = gpiod_line_request_bulk(out_lines, &config, values); // set initial values
	if (rv)
		die_perror("unable to request output lines");


	// infinite loop
	for (;;) {
		gpiod_line_bulk_reset(evlines);
		rv = gpiod_line_event_wait_bulk(in_lines, &timeout, evlines);
		if (rv < 0)
			die_perror("error waiting for events");
		if (rv == 0)
			continue;

		num_lines = gpiod_line_bulk_num_lines(evlines);

		for (x = 0; x < num_lines; x++) {
			line = gpiod_line_bulk_get_line(evlines, x);

			rv = gpiod_line_event_read_multiple(line, events,
							    ARRAY_SIZE(events));
			if (rv < 0)
				die_perror("error reading line events");

			for (y = 0; y < rv; y++) {
				handle_event(gpiod_line_offset(line), // input lines event
						out_lines, // output lines
					    &events[y].ts); // event timestamp
				// events_done++;

				// if (events_done >= 100){
				// 	printf("100 events \n");
				// 	events_done = 0;
				// }
			}
		}
	}

done:
	gpiod_line_release_bulk(in_lines);
	gpiod_line_bulk_free(in_lines);
	gpiod_line_release_bulk(out_lines);
	gpiod_line_bulk_free(out_lines);
	gpiod_line_bulk_free(evlines);
	gpiod_chip_close(chip);

	return EXIT_SUCCESS;
}