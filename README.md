# asus-keyboard-backlight
Automatically dims the keyboard backlight after a timeout.
_Should_ work with most ASUS laptops running Gnome desktop enviroment.

# Requisites
- ASUS laptop
- Gnome

# Compatibility
Tested on ASUS VivoBook M3502QA running Fedora Linux 42.
It should be compatible with most ASUS laptops. Line `#define BRIGHTNESS_FILE` on source can be changed to match the correct brightness file, to extend support to other devices.

# Compile & Install
With make
1) `make`
2) `make install`
3) Close session (logout) and login

Alternatively, manually
1) `gcc src/asus-keyboard-backlight.c -o asus-keyboard-backlight`
2) `sudo cp asus-keyboard-backlight /usr/local/bin/`
3) `sudo chown root:root /usr/local/bin/asus-keyboard-backlight`
4) `sudo chmod u+s /usr/local/bin/asus-keyboard-backlight`
5) `mkdir ~/.config/autostart`
6) `cp src/asus-keyboard-backlight.desktop ~/.config/autostart/` 
7) Close session (logout) and login

# Uninstall
With make
1) `make uninstall`

Alternatively, manually
1) `sudo rm /usr/local/bin/asus-keyboard-backlight`
2) `rm ~/.config/autostart/asus-keyboard-backlight.desktop`

# Set the Timeout
By default, the timeout is set to 10000 milliseconds.
Timeout can be changed by specifying `asus-keyboard-backlight -t <timeout> >` on the `asus-keyboard-backlight.desktop` file.

Here's the list of commands:
```
Usage: ./asus-keyboard-backlight [options]
Options:
-h, --help            Show this help message
-t, --timeout <ms>    Set backlight timeout in milliseconds (default: 10000)
-v, --version         Show version info
```

# Notes
Step 3 of the manual installation sets the ownership of the executable to `root`, and step 4 sets the setuid bit. This ensures that this program is always run with the appropriate permissions. The brightness file, in fact, is writable only by `root`.

Step 6 requires the Gnome desktop environment to be loaded. This program does not work on TTY.
