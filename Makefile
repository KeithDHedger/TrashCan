
PREFIX=/usr
VERSION=1.0.0

all:
	g++ trash.cpp trashCanClass.cpp prefsClass.cpp QT_AboutBox.cpp -DVERSION="\"$(VERSION)\"" -DDATADIR="\"$(DESTDIR)$(PREFIX)/share/trashcan\"" -Wall `pkg-config --cflags --libs Qt6Core Qt6Widgets x11 gio-2.0` -fPIC -o trashcan

install: all
	mkdir -vp "$(DESTDIR)$(PREFIX)/share/trashcan"
	mkdir -vp "$(DESTDIR)$(PREFIX)/bin"
	cp "./trashcan" "$(DESTDIR)$(PREFIX)/bin"
	cp "./user-trash-full.png" "user-trash.png" "gpl-3.0.txt" "$(DESTDIR)$(PREFIX)/share/trashcan"

clean:
	rm "./trashcan" || exit 0

local: clean
	g++ trash.cpp trashCanClass.cpp prefsClass.cpp QT_AboutBox.cpp -DVERSION="\"$(VERSION)\"" -DDATADIR="\".\"" -Wall `pkg-config --cflags --libs Qt6Core Qt6Widgets x11 gio-2.0` -fPIC -o trashcan
