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


// static void handle_event(unsigned int line_offset, unsigned int event_type,
// 			 struct timespec *ts)
// {
// 	char *evname;

// 	if (event_type == GPIOD_LINE_EVENT_RISING_EDGE)
// 		evname = " RISING EDGE";
// 	else
// 		evname = "FALLING EDGE";

// 	printf("event: %s offset: %u timestamp: [%8ld.%09ld]\n",
// 	       evname, offset, ts->tv_sec, ts->tv_nsec);
// }

static void handle_signal(int signum UNUSED)
{
	exit(EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	unsigned int offsets[64], num_lines = 1, events_done = 0, x;
	// bool watch_rising = false, watch_falling = false;
	int flags = 0;
	struct timespec timeout = { 10, 0 };
	int rv, i, y, event_type;
	// struct mon_ctx ctx;
	struct gpiod_chip *chip;
	struct gpiod_line_bulk *lines, *evlines;
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

	offsets[0] = 27; //input

	chip = chip_open_lookup("gpiochip0");
	if (!chip)
		die_perror("unable to open gpiochip0");

	lines = gpiod_chip_get_lines(chip, offsets, 1);
	if (!lines)
		die_perror("unable to retrieve GPIO lines from chip");

	memset(&config, 0, sizeof(config));

	config.consumer = "gpiomon";
	// config.request_type = event_type;
	config.request_type = GPIOD_LINE_REQUEST_EVENT_RISING_EDGE;
	// config.flags = flags;
	config.flags = 0;

	rv = gpiod_line_request_bulk(lines, &config, NULL);
	if (rv)
		die_perror("unable to request GPIO lines for events");

	evlines = gpiod_line_bulk_new(num_lines);
	if (!evlines)
		die("out of memory");

	for (;;) {
		gpiod_line_bulk_reset(evlines);
		rv = gpiod_line_event_wait_bulk(lines, &timeout, evlines);
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
				// handle_event(gpiod_line_offset(line),
					    //  events[y].event_type,
					    //  &events[y].ts);
				events_done++;

				if (events_done >= 100){
					printf("100 events \n");
					events_done = 0;
				}
			}
		}
	}

done:
	gpiod_line_release_bulk(lines);
	gpiod_line_bulk_free(lines);
	gpiod_line_bulk_free(evlines);
	gpiod_chip_close(chip);

	return EXIT_SUCCESS;
}