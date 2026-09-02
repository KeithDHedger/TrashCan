# TrashCan
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
You will need the dev files for Qt6Core Qt6Widgets x11 and gio-2.0  

Drag files to the bin to move them to the trash.  

CTRL+leftmousbutton will make the trash can bigger.  
CTRL+SHIFT+leftmousbutton will make the trash can smaller.  
Drag the trash can to wherever you want it on screen.  
Size and postion are saved on quit.

Right click for context menu:  
Empty Trash  
Show Trash  
Quit  
  
Show trash will attempt to open the trash in the default application ( usually your file manager ).  
  
Requires gvfs to be running ( it should have been started by the file manager ), otherwise you may need to launch the app with:  
```

dbus-launch trashcan

```
The trashicon window is now set to _NET_WM_WINDOW_TYPE_DOCK as most compositors can be set to ignore drop shadows for these types of window.  

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