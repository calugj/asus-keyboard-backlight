CC = gcc
SRC = src/asus-keyboard-backlight.c
OUT = asus-keyboard-backlight

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $(OUT)

install:
	sudo cp $(OUT) /usr/local/bin/
	sudo chown root:root /usr/local/bin/$(OUT)
	sudo chmod u+s /usr/local/bin/$(OUT)
	mkdir $(HOME)/.config/autostart
	cp src/asus-keyboard-backlight.desktop $(HOME)/.config/autostart/

uninstall:
	sudo rm /usr/local/bin/$(OUT)
	rm $(HOME)/.config/autostart/asus-keyboard-backlight.desktop

clean:
	rm -f $(OUT)
