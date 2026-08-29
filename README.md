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
  
Requires gvfs to be running.  
