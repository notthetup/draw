  #include "cmd.h"

char cmdhistory[2] = {0,0};

void execute_cmd(){
  // usb_printf("# CMD : %c %c\r\n", cmdhistory[1], cmdhistory[0]);
  char ret[10] = {0};
  if (cmdhistory[1] == 'L'){
    if (cmdhistory[0] == 'G') {
      uint16_t val = gpio_get(LED_GREEN_PORT, LED_GREEN_PIN);
      if (val == 0) gpio_set(LED_GREEN_PORT, LED_GREEN_PIN);
      else gpio_clear(LED_GREEN_PORT, LED_GREEN_PIN);
      sprintf(ret, "#LG%d\r\n", val == 0 ? 0 : 1);
    }
    if (cmdhistory[0] == 'R') {
     uint16_t val = gpio_get(LED_RED_PORT, LED_RED_PIN);
     if (val == 0) gpio_set(LED_RED_PORT, LED_RED_PIN);
     else gpio_clear(LED_RED_PORT, LED_RED_PIN);
     sprintf(ret, "#LR%d\r\n", val == 0 ? 0 : 1);
    }
  }
  usb_printf("%s", ret);
}

void cmd_parser(char * buf, int len){
  for (int i = 0; i < len; ++i) {
    // usb_printf("# %d:%d\r\n", i, buf[i]);
    if (buf[i] == '\r'){
      execute_cmd();
      cmdhistory[1] = 0;
      cmdhistory[0] = 0;
    }else{
      cmdhistory[1] = cmdhistory[0];
      cmdhistory[0] = buf[i];
    }
  }
}
