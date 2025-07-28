#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define SLEEP_SHORT_DELAY 300
#define SLEEP_LONG_DELAY 1000
#define TIMEOUT_DEFAULT 10000
#define BRIGHTNESS_FILE "/sys/class/leds/asus::kbd_backlight/brightness"
#define LOCK_FILE "/tmp/asus-backlight-idle.lock"
#define COMMAND "gdbus call --session \
    --dest org.gnome.Mutter.IdleMonitor \
    --object-path /org/gnome/Mutter/IdleMonitor/Core \
    --method org.gnome.Mutter.IdleMonitor.GetIdletime"
#define VERSION "1.0.0"



int write_brightness(int value) {
  FILE* file = fopen(BRIGHTNESS_FILE, "w");
  if (file == NULL) {
      perror("Failed to write to brightness file");
      return -1;
  }
  fprintf(file, "%d", value);
  fclose(file);
  return 0;
}


int read_brightness() {
  FILE* file = fopen(BRIGHTNESS_FILE, "r");
  if(file == NULL) {
    perror("Failed to read brightness file");
    return -1;
  }
  int status;
  fscanf(file, "%d", &status);
  fclose(file);
  return status;
}


int get_idle_time() {    
    char buffer[25];
    FILE* file = popen(COMMAND, "r");
    if(file == NULL) {
      perror("Failed to obtain result from command");
      return 1;
    }
    fgets(buffer, sizeof(buffer), file);
    pclose(file);
    int idle;
    sscanf (buffer,"%*s %d", &idle);
    return idle;
}


int already_running() {
  FILE* file = fopen(LOCK_FILE, "w+");
  if (file == NULL)
      return 1;

  int descriptor = fileno(file);
  if (lockf(descriptor, F_TLOCK, 0) < 0)
      return 1;
  return 0;
}


int main(int argc, char* argv[]) {
  const char* display = getenv("DISPLAY");
  const char* wayland = getenv("WAYLAND_DISPLAY");

  if (!display && !wayland)
    return 1;
  
  if (already_running())
    return 1;
    
    
  int timeout = TIMEOUT_DEFAULT;
  
  for(int i = 1; i < argc; i++) {
    if((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)) {
      printf("Usage: %s [options]\n", argv[0]);
      printf("Options:\n");
      printf("  -h, --help            Show this help message\n");
      printf("  -t, --timeout <ms>    Set backlight timeout in milliseconds (default: 10000)\n");
      printf("  -v, --version         Show version info\n");
      return 0;
    }
    else if((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--version") == 0)) {
      printf("%s\n", VERSION);
      return 0;
    }
    else if((strcmp(argv[i], "-t") == 0) || (strcmp(argv[i], "--timeout") == 0)) {
      if (argc > i+1) {
        timeout = atoi(argv[++i]);
        if (timeout <= 0 || timeout > 300000) {
          fprintf(stderr, "Invalid timeout.\n");
          return 1;
        }
      } else {
        fprintf(stderr, "Error: Missing value for %s\n", argv[i]);
        return 1;
      }
    }
  }
    
  
  int idle_time = 0;
  int current_status = 0;
  int stored_status = 0;
  int virtual_status = 0;
  int additional = 0;
  
  
  current_status = read_brightness();
  if(current_status >= 1) {
    virtual_status = 1;
  }
  
  
  while(1) {
    idle_time = get_idle_time();
    current_status = read_brightness();
    
    if(virtual_status == 0 && current_status >= 1) {
      additional = idle_time;
      stored_status = current_status;
    }
    if(virtual_status == 0 && idle_time <= timeout + additional) {
      write_brightness(stored_status);
      virtual_status = 1;
    }
    else if(virtual_status == 1 && idle_time > timeout + additional) {
      stored_status = current_status;
      additional = 0;
      write_brightness(0);
      virtual_status = 0;
    }
    
    
    if(current_status == 0)
      usleep(SLEEP_SHORT_DELAY * 100);
    else
      usleep(SLEEP_LONG_DELAY * 100);
  }
  
  return 0;
}
