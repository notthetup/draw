#include "cmd.h"

char lastcmd = 0;
char lastlastcmd = 0;

void execute_cmd(){
  // usb_printf("# CMD : %c %c\r\n", lastlastcmd, lastcmd);
  if (lastlastcmd == 'L'){
    if (lastcmd == 'G') {
      gpio_toggle(LED_GREEN_PORT, LED_GREEN_PIN);
    }
    if (lastcmd == 'R') {
      gpio_toggle(LED_RED_PORT, LED_RED_PIN);
    }
  }
}

void cmd_parser(char * buf, int len){
  for (int i = 0; i < len; ++i) {
    // usb_printf("# %d:%d\r\n", i, buf[i]);
    if (buf[i] == '\r'){
      execute_cmd();
      lastlastcmd = 0;
      lastcmd = 0;
    }else{
      lastlastcmd = lastcmd;
      lastcmd = buf[i];
    }
  }
}
