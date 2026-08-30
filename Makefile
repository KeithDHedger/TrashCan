
PREFIX=/usr

all:
	g++ -DDATADIR="\"$(DESTDIR)$(PREFIX)/share/trashcan\"" -Wall `pkg-config --cflags --libs Qt6Core Qt6Widgets x11 gio-2.0` -fPIC trash.cpp trashCanClass.cpp -o trashcan

install: all
	mkdir -vp "$(DESTDIR)$(PREFIX)/share/trashcan"
	mkdir -vp "$(DESTDIR)$(PREFIX)/bin"
	cp "./trashcan" "$(DESTDIR)$(PREFIX)/bin"
	cp "./user-trash-full.png" "user-trash.png" "$(DESTDIR)$(PREFIX)/share/trashcan"

clean:
	rm "./trashcan" || exit 0

local: clean
	g++ -DDATADIR="\"`pwd`\"" -Wall `pkg-config --cflags --libs Qt6Core Qt6Widgets x11 gio-2.0` -fPIC trash.cpp trashCanClass.cpp -o trashcan
