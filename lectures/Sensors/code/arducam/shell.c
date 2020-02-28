/* 
 * This code brought to you by the rag tag duo of Arjun and Eric! Visit us at
 * our github accounts: https://github.com/arjunvb, https://github.com/efritz09
 * 
 * Completed on: March 14, 2016
 */

#include "printf.h"
#include "gpio.h"
#include "circular.h"
#include "keyboard.h"
#include "console.h"
#include "gl.h"
#include "shell.h"
#include "rzmalloc.h"
#include "omni.h"
#include "strtol.h"
#include "arducam.h"

#define COMMANDS_LENGTH 18


int cmd_normal(int argc, const char *argv[]);
int cmd_capture(int argc, const char *argv[]);
int cmd_stream(int argc, const char *argv[]);
int cmd_antique(int argc, const char *argv[]);
int cmd_red(int argc, const char *argv[]);
int cmd_green(int argc, const char *argv[]);
int cmd_blue(int argc, const char *argv[]);
int cmd_neg(int argc, const char *argv[]);
int cmd_sat(int argc, const char *argv[]);
int cmd_contrast(int argc, const char *argv[]);
int cmd_brightness(int argc, const char *argv[]);
int cmd_grey(int argc, const char *argv[]);
int cmd_help(int argc, const char *argv[]);

int cmd_cloudy(int argc, const char *argv[]);
int cmd_auto(int argc, const char *argv[]);
int cmd_sunny(int argc, const char *argv[]);
int cmd_office(int argc, const char *argv[]);
int cmd_home(int argc, const char *argv[]);

static const command_t commands[] = {
  {"capture", "captures and displays an image", cmd_capture},
  {"stream", "streams images in real time", cmd_stream},
  {"normal","resets the settings", cmd_normal},
  {"red","gives image a red bias", cmd_red},
  {"green", "gives image a green bias", cmd_green},
  {"blue","gives image a blue bias", cmd_blue},
  {"negative","creates a negative of the image", cmd_neg},
  {"saturation", "sets the image saturation (-2 - 2)", cmd_sat},
  {"contrast", "sets the image contrast (-2 - 2)", cmd_contrast},
  {"brightness", "sets the image brightness (-2 - 2)", cmd_brightness},
  {"greyscale","sets the image to greyscale",cmd_grey},
  {"auto", "removes special effects",cmd_auto},
  {"antique","gives the image an 'antique' look", cmd_antique},
  {"sunny", "gives it a 'sunny' look",cmd_sunny},
  {"cloudy","gives it a 'cloudy' look",cmd_cloudy},
  {"office", "gives it an 'office' look", cmd_office},
  {"home", "gives it a 'home' look", cmd_home},
  {"help","explains a given command or lists available commands",cmd_help}
};

//displays all the commands if argc = 0, otherwise, describes the command requested
int cmd_help(int argc, const char *argv[]) {
  console_printf(": ");  
  if(argc == 0) {
    int i = 0;
    console_printf("Commands available:\n");
    for(; i < COMMANDS_LENGTH-1; i++) {
      console_printf(commands[i].name);
      console_printf(", ");
    }
    console_printf(commands[i].name);
  }else if(argc == 1) {
    char* value = (char*)argv[1];
    for(int i = 0; i < COMMANDS_LENGTH; i++) {
      if(string_compare(value, commands[i].name)) {
        console_printf(commands[i].description);
        return 0;
      }
    }
    console_printf("ERROR: '%s' is invalid",value);
  }
  else console_printf("ERROR: too many arguments");
  return 0;
}

int cmd_cloudy(int argc, const char *argv[]) 
{
  omni_set_cloudy();
  capture_image();
  return 0;
}

int cmd_auto(int argc, const char *argv[]) 
{
  omni_set_auto();
  capture_image();
  return 0;
}

int cmd_sunny(int argc, const char *argv[]) 
{
  omni_set_sunny();
  capture_image();
  return 0;
}

int cmd_office(int argc, const char *argv[]) 
{
  omni_set_office();
  capture_image();
  return 0;
}

int cmd_home(int argc, const char *argv[]) 
{
  omni_set_home();
  capture_image();
  return 0;
}

int cmd_grey(int argc, const char *argv[]) 
{
  omni_set_bw();
  capture_image();
  return 0;
}

int cmd_normal(int argc, const char *argv[]) 
{
  omni_set_normal();
  capture_image();
  return 0;
}

int cmd_antique(int argc, const char *argv[]) 
{
  omni_set_antique();
  capture_image();
  return 0;
}

int cmd_red(int argc, const char *argv[]) 
{
  omni_set_reddish();
  capture_image();
  return 0;
}

int cmd_green(int argc, const char *argv[]) 
{
  omni_set_greenish();
  capture_image();
  return 0;
}

int cmd_blue(int argc, const char *argv[]) 
{
  omni_set_bluish();
  capture_image();
  return 0;
}

int cmd_neg(int argc, const char *argv[]) 
{
  omni_set_neg();
  capture_image();
  return 0;
}

int cmd_sat(int argc, const char *argv[]) 
{
  if(argc == 0) omni_set_saturation(0);
  else {
    const char* val = argv[1];
    char* end;
    int sat = (int)strtol(val,&end,16);
    omni_set_saturation(sat);
  }
  capture_image();
  return 0;
}

int cmd_contrast(int argc, const char *argv[]) 
{
  if(argc == 0) omni_set_contrast(0);
  else {
    const char* val = argv[1];
    char* end;
    int cont = (int)strtol(val,&end,16);
    omni_set_contrast(cont);
  }
  capture_image();
  return 0;
}

int cmd_brightness(int argc, const char *argv[]) 
{
  if(argc == 0) omni_set_brightness(0);
  else {
    const char* val = argv[1];
    char* end;
    int bright = (int)strtol(val,&end,16);
    omni_set_brightness(bright);
  }
  capture_image();
  return 0;
}

int cmd_stream(int argc, const char *argv[]) 
{
	return IMAGE_STREAM; 
}

int cmd_capture(int argc, const char *argv[]) 
{
	return IMAGE_CAPTURE;
}

/* process_prompt
 * processes the prompt from the user and calls the appropriate function
 */
int process_prompt(char* p) {
  //process the prompt
  char cmd[20][20];
  int cmd_row = 0;
  int cmd_col = 0;
  char* c;
  for(c = p; *c; c++) {
    if(*c == ' ') { //end of word
      cmd[cmd_row][cmd_col] = '\0';
      cmd_col = 0;
      cmd_row++;
    }
    else cmd[cmd_row][cmd_col++] = *c;
  }
  cmd[cmd_row][cmd_col] = '\0';

  char *cmd_ptr[cmd_row];
  for(int i = 0; i <= cmd_row; i++) {
    //fill the command pointer
    cmd_ptr[i] = cmd[i];
  }
  //search for a match
  int return_val = 0xFF;
  char* value = cmd[0];
  for(int i = 0; i < COMMANDS_LENGTH; i++) {
    if( string_compare(commands[i].name, value)) {
      return_val = commands[i].fn(cmd_row,(const char **)cmd_ptr);
      break;
    }
  }
  if(return_val == 0xFF) console_printf(" <- That's invalid");
  //clear the cmd;
  for(int i = 0; i < 20; i++) {
    for(int j = 0; j < 20; j++) {
      cmd[i][j] = 0x0;
    }
  }
  return return_val;
}

/* string_compare
 * simple string comparison, returns 1 if true
 */
int string_compare(char* s1, char* s2) {
  char* c;
  for(c = s1; *c; c++) {
    if(*s2 == '\0') return 0;
    if(*s1 != *s2) return 0;
    s1++;
    s2++;
  }
  if(*s1 != *s2) return 0;
  else return 1;
}
