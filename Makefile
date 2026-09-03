
PREFIX=/usr
USEQTVERS=6
VERSION:=1.0.0
SRCFILES:=src/trash.cpp src/trashCanClass.cpp src/prefsClass.cpp src/QT_AboutBox.cpp
FLAGSANDLIBS:=$(shell pkg-config --libs --cflags Qt$(USEQTVERS)Core Qt$(USEQTVERS)Widgets x11 gio-2.0) -O0 -g

all:
	g++ $(SRCFILES) -DVERSION="\"$(VERSION)\"" -DDATADIR="\"$(DESTDIR)$(PREFIX)/share/trashcan\"" -Wall $(FLAGSANDLIBS) -fPIC -o trashcan

install: all
	mkdir -vp "$(DESTDIR)$(PREFIX)/share/trashcan"
	mkdir -vp "$(DESTDIR)$(PREFIX)/bin"
	cp "./trashcan" "$(DESTDIR)$(PREFIX)/bin"
	cp data/* "$(DESTDIR)$(PREFIX)/share/trashcan"

clean:
	rm "./trashcan" || exit 0

local: clean
	g++ $(SRCFILES) -DVERSION="\"$(VERSION)\"" -DDATADIR="\"./data\"" -Wall $(FLAGSANDLIBS) -fPIC -o trashcan
