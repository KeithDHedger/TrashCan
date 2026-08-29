/*
 *
 * ©K. D. Hedger. Sat 29 Aug 19:18:40 BST 2026 keithdhedger@gmail.com

 * This file (trash.cpp) is part of TrashCan.

 * TrashCan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * TrashCan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TrashCan.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	window->show();

	setWindowSticky(display,window->winId());
	ret=app.exec();
	prefs.setValue("app/geometry",window->saveGeometry());
 
    XCloseDisplay(display);

    return(ret);
}