# TrashCan

![Trashcan](data/user-trash-full.png)

A simple stand alone trash can for those who don't run a full desktop or whose desktop doesn't provide one.  

To run the trashcan from this folder use:  
```

make local
./trashcan

```

Or to install Globally use:  
```

make
sudo make install

```

The installtion path can be changed by setting DESTDIR and/or PREFIX like so:  
Which will install to "/tmp/XXX/usr/local/bin"  
```

make PREFIX=/usr/local DESTDIR=/tmp/XXX install

```

To compile with Qt5 ( default is to use qt6 ) add USEQTVERS=5 after make eg:
```

make local USEQTVERS=5

```

You will need the dev files for Qt[5/6]Core Qt[5/6]Widgets x11 and gio-2.0  

Drag files to the bin to move them to the trash.  

```

Trashcan - Version 1.0.0
  -t,--ontop
        Make trash appear on top of other windows ( default _NET_WM_STATE_BELOW).
  -w,--windowtype
        Set trash window to type ( default _NET_WM_WINDOW_TYPE_DOCK ).

CTRL+LEFTBUTTON increases trashcan size.
CTRL+SHIFT+LEFTBUTTON decreases trashcan size.
RIGHTBUTTON brings up context menu.
Drag trashcan to desired position with LEFTBUTTON.
Size and position are saved on exit.

```
  
Show trash will attempt to open the trash in the default application ( usually your file manager ).  
  
Requires gvfs to be running ( it should have been started by the file manager ), otherwise you may need to launch the app with:  
```

dbus-launch trashcan

```
The trashicon window is now set to _NET_WM_WINDOW_TYPE_DOCK as most compositors can be set to ignore drop shadows for these types of window.  
