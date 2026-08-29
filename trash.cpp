#if 0

#(c)@USER@ @DATE@ @EMAIL@

if [[ ! "X$USEVALGRIND" = "X" ]];then
	if [ ! -e ./ignorelibleaks ];then
cat>ignorelibleaks<<EOF
{
   ignore_unversioned_libs
   Memcheck:Leak
   ...
   obj:*/lib*/lib*.so
}

{
   ignore_versioned_libs
   Memcheck:Leak
   ...
   obj:*/lib*/libQt.so.*
}
EOF
	fi

	case $USEVALGRIND in
		1)
			VALGRIND="valgrind"
			;;
		2)
			VALGRIND="valgrind --leak-check=full"
			;;
		3)
			VALGRIND="valgrind --leak-check=full --show-leak-kinds=all"
			;;
		4)
			unset QT_QPA_PLATFORMTHEME
			VALGRIND="valgrind --leak-check=full  --track-origins=yes --suppressions=./ignorelibleaks -s "
			;;
	esac
fi

g++ -Wall $(pkg-config --cflags --libs Qt6Core Qt6Widgets x11) -fPIC trash.cpp trashCanClass.cpp ||exit 1
$VALGRIND ./a.out "$@"
retval=$?
#rm ./a.out
exit $retval

#endif
#define PACKAGE_NAME "TrashCan"

#include "trashCanClass.h"

#include <X11/Xlib.h>
#include <X11/Xatom.h>

void setWindowSticky(Display *display,Window window)
{
    int		screen=DefaultScreen(display);
    Window	root=RootWindow(display,screen);

	Atom net_wm_state=XInternAtom(display,"_NET_WM_STATE",False);
	Atom net_wm_state_sticky=XInternAtom(display,"_NET_WM_STATE_STICKY",False);

	XEvent event={0};
	event.xclient.type=ClientMessage;
	event.xclient.message_type=net_wm_state;
	event.xclient.display=display;
	event.xclient.window=window;
	event.xclient.format=32;

    // 1=add,0=remove,2=toggle
	event.xclient.data.l[0]=1;
	event.xclient.data.l[1]=net_wm_state_sticky;
	event.xclient.data.l[2]=0;
	event.xclient.data.l[3]=1;// source indication: normal application
	event.xclient.data.l[4]=0;

	XSendEvent(display,root,False,SubstructureRedirectMask | SubstructureNotifyMask,&event);

    XFlush(display);
}

int main(int argc, char **argv)
{
	QSettings		prefs("KDHedger","TrachCan");
    QApplication		app(argc,argv);
	int				ret;
	Display			*display;
//fprintf(stderr,">>%s<<\n",DATADIR);

 	display=XOpenDisplay(NULL);
	if(display==NULL)
		{
			fprintf(stderr,"Can't open display exiting ...");
			exit(1);
		}

 	app.setOrganizationDomain("KDHedger");
	app.setApplicationName(PACKAGE_NAME);

    trashCanClassClass *window=new trashCanClassClass();

	if(prefs.contains("app/geometry"))
		window->restoreGeometry(prefs.value("app/geometry").toByteArray());
	else
		window->setGeometry(56,480,256,354);
//qApp->installEventFilter(window);
	window->show();

	setWindowSticky(display,window->winId());
	ret=app.exec();
	prefs.setValue("app/geometry",window->saveGeometry());
 
    XCloseDisplay(display);

    return(ret);
}