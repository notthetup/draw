#include "cmd.h"

char cmdstr[2] = {0,0};

static int avg = 0;
static int tim = 0;

void execute_cmd(){
  // usb_printf("# CMD : %c %c\r\n", cmdstr[1], cmdstr[0]);
  char ret[10] = {0};
  switch (cmdstr[1]) {
    case 'L':
      toggle_led(cmdstr[0], ret);
    break;
    case 'A':
      update_averaging(cmdstr[0], ret);
    break;
    case 'T':
      update_time(cmdstr[0], ret);
    break;
    case '?':
      print_status(ret);
    break;
  }
  if (ret[0] != 0 ) usb_printf("%s", ret);
}

void toggle_led(char led, char *ret){
  if (led == 'G') {
    uint16_t val = gpio_get(LED_GREEN_PORT, LED_GREEN_PIN);
    if (val == 0) gpio_set(LED_GREEN_PORT, LED_GREEN_PIN);
    else gpio_clear(LED_GREEN_PORT, LED_GREEN_PIN);
    sprintf(ret, "#LG%d\r\n", val == 0 ? 0 : 1);
  }
  if (led == 'R') {
   uint16_t val = gpio_get(LED_RED_PORT, LED_RED_PIN);
   if (val == 0) gpio_set(LED_RED_PORT, LED_RED_PIN);
   else gpio_clear(LED_RED_PORT, LED_RED_PIN);
   sprintf(ret, "#LR%d\r\n", val == 0 ? 0 : 1);
  }
}

void update_averaging(char avgstr, char *ret){
  int a = atoi(&avgstr);
  if (a < 0) a=0;
  if (a > 8) a=8;
  avg = a;
  print_status(ret);
}

void update_time(char timstr, char *ret){
  int t = atoi(&timstr);
  if (t < 0) t=0;
  if (t > 8) t=8;
  tim = t;
  print_status(ret);
}

void print_status(char *ret){
  sprintf(ret, "#A%d\tT%d\r\n", avg, tim);
}


void cmd_parser(char * buf, int len){
  for (int i = 0; i < len; ++i) {
    // usb_printf("# %d:%d\r\n", i, buf[i]);
    if (buf[i] == '\r'){
      execute_cmd();
      cmdstr[1] = 0;
      cmdstr[0] = 0;
    }else{
      cmdstr[1] = cmdstr[0];
      cmdstr[0] = buf[i];
    }
  }
}
