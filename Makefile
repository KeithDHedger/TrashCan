
PREFIX=/usr
VERSION:=1.0.0
SRCFILES:=src/trash.cpp src/trashCanClass.cpp src/prefsClass.cpp src/QT_AboutBox.cpp
FLAGSANDLIBS:=$(shell pkg-config --libs --cflags Qt6Core Qt6Widgets x11 gio-2.0)

all:
	g++ $(SRCFILES) -DVERSION="\"$(VERSION)\"" -DDATADIR="\"$(DESTDIR)$(PREFIX)/share/trashcan\"" -Wall $(FLAGSANDLIBS) -fPIC -o trashcan

install: all
	mkdir -vp "$(DESTDIR)$(PREFIX)/share/trashcan"
	mkdir -vp "$(DESTDIR)$(PREFIX)/bin"
	cp "./trashcan" "$(DESTDIR)$(PREFIX)/bin"
	cp "data/user-trash-full.png" "data/user-trash.png" "data/gpl-3.0.txt" "$(DESTDIR)$(PREFIX)/share/trashcan"

clean:
	rm "./trashcan" || exit 0

local: clean
	g++ $(SRCFILES) -DVERSION="\"$(VERSION)\"" -DDATADIR="\"./data\"" -Wall $(FLAGSANDLIBS) -fPIC -o trashcan
