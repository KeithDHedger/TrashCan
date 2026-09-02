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
#include "prefsClass.h"

//#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

void setWindowProps(Display *display,Window window,const char* grp,const char *type_name,int what)
{
	Atom window_type = XInternAtom(display,grp,False);
	Atom type = XInternAtom(display,type_name,False);

	XChangeProperty(display,window,window_type,XA_ATOM,32,what,(unsigned char *)&type,1);
    XFlush(display);
}

int main(int argc, char **argv)
{
	QSettings		prefs("KDHedger",PACKAGE_NAME);
    QApplication		app(argc,argv);
	int				ret;
	Display			*display;
	prefsClass		newprefs;
	bool				parse;
//	char cwd[PATH_MAX];
//fprintf(stderr,">>%s<<->>%s<<->>%s<<\n",DATADIR,argv[0],getcwd(cwd,sizeof(cwd)));

 	display=XOpenDisplay(NULL);
	if(display==NULL)
		{
			fprintf(stderr,"Can't open display exiting ...");
			exit(1);
		}

 	app.setOrganizationDomain("KDHedger");
	app.setApplicationName(PACKAGE_NAME);

	option	long_options[]=
		{
			{"ontop",no_argument,NULL,'t'},
			{"windowtype",required_argument,NULL,'w'},
			{0,0,0,0}
		};

	parse=newprefs.doCliArgs(argc,argv,long_options);
	if(parse==false)
		{
			qDebug()<<"\nTrashcan - Version"<<VERSION;
			qDebug()<<"  -t,--ontop\n\tMake trash appear on top of other windows ( default _NET_WM_STATE_BELOW).";
			qDebug()<<"  -w,--windowtype\n\tSet trash window to type ( default _NET_WM_WINDOW_TYPE_DOCK ).";
			qDebug()<<"\nCTRL+LEFTBUTTON increases trashcan size.";
			qDebug()<<"CTRL+SHIFT+LEFTBUTTON decreases trashcan size.";
			qDebug()<<"RIGHTBUTTON brings up context menu.";
			qDebug()<<"Drag trashcan to desired position with LEFTBUTTON.";
			qDebug()<<"Size and position are saved on exit.";
			exit(0);
		}

    trashCanClassClass *window=new trashCanClassClass();

	if(prefs.contains("app/geometry"))
		window->restoreGeometry(prefs.value("app/geometry").toByteArray());
	else
		window->setGeometry(100,100,256,354);

	window->show();

	if(newprefs.prefsData.contains(newprefs.hashFromKey("windowtype")))
		setWindowProps(display,window->winId(),"_NET_WM_WINDOW_TYPE",qPrintable(newprefs.getPrefValue("windowtype").toStringList().at(0)),PropModeReplace);
	else
		setWindowProps(display,window->winId(),"_NET_WM_WINDOW_TYPE","_NET_WM_WINDOW_TYPE_DOCK",PropModeReplace);

	setWindowProps(display,window->winId(),"_NET_WM_STATE","_NET_WM_STATE_STICKY",PropModeReplace);

	if(newprefs.prefsData.contains(newprefs.hashFromKey("ontop")))
		setWindowProps(display,window->winId(),"_NET_WM_STATE","_NET_WM_STATE_ABOVE",PropModeAppend);
	else
		setWindowProps(display,window->winId(),"_NET_WM_STATE","_NET_WM_STATE_BELOW",PropModeAppend);

	ret=app.exec();
	prefs.setValue("app/geometry",window->saveGeometry());
 
    XCloseDisplay(display);

    return(ret);
}