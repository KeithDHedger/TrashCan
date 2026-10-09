
PREFIX=/usr
USEQTVERS=6
VERSION:=1.0.0
SRCFILES:=src/trash.cpp src/trashCanClass.cpp src/cliPrefsClass.cpp src/QT_AboutBox.cpp
FLAGSANDLIBS:=$(shell pkg-config --libs --cflags Qt$(USEQTVERS)Core Qt$(USEQTVERS)Widgets x11 gio-2.0) -O0 -g
PROGNAME:=trashcan

all:
	g++ $(SRCFILES) -DVERSION="\"$(VERSION)\"" -DDATADIR="\"$(DESTDIR)$(PREFIX)/share/trashcan\"" -Wall $(FLAGSANDLIBS) -fPIC -o $(PROGNAME)

install: all
	mkdir -vp "$(DESTDIR)$(PREFIX)/share/$(PROGNAME)"
	mkdir -vp "$(DESTDIR)$(PREFIX)/bin"
	cp -f "./trashcan" "$(DESTDIR)$(PREFIX)/bin"
	cp data/* "$(DESTDIR)$(PREFIX)/share/$(PROGNAME)"
	strip --strip-unneeded "$(DESTDIR)$(PREFIX)/bin/$(PROGNAME)"

clean:
	rm "./trashcan" || exit 0

local: clean
	g++ $(SRCFILES) -DVERSION="\"$(VERSION)\"" -DDATADIR="\"./data\"" -Wall $(FLAGSANDLIBS) -fPIC -o $(PROGNAME)
