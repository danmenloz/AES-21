#include <termios.h>
#include <stdio.h>
#include <unistd.h>

#include "is_options.h"

int get_key_board_from_termios()
{
    int key_value;
    struct termios new_config;
    struct termios old_config;

    tcgetattr(0, &old_config);
    new_config = old_config;
    new_config.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new_config);
    key_value = getchar();
    tcsetattr(0, TCSANOW, &old_config);
    return key_value;
}

void processKeyboardEvent(void){
  int keyVal = 0;

  usleep(100);
  tcflush(0, TCIOFLUSH);
  keyVal= get_key_board_from_termios();
  switch (keyVal) {
  case 27: 
    keyVal= get_key_board_from_termios();
    if(keyVal == 91){
      keyVal= get_key_board_from_termios();
      switch (keyVal)
	{
	case 65/* up */:   
	  break;
	case 66/* down */:
	  break;
	case 67/* right */: 
	  break;
	case 68/* left */: 
	  break;   
	default :
	  break;
	}
    }
    break;
  case'd':
    show_data = (show_data + 1) % 4;
    break;
  case 'i':
    is_invert = 1 - is_invert;
    printf("invert = %d\n", is_invert);
    break;
  case 't':
    update_target_color = 1;
    break;
  case '+':
    color_threshold += 10;
    printf("color_threshold = %d\n", color_threshold);
    break;
  case '-':
    color_threshold -= 10;
    if (color_threshold < 0)
      color_threshold = 0;
    printf("color_threshold = %d\n", color_threshold);
    break;
  default:
    break;
  }
}



