/*
 *
 * ©K. D. Hedger. Wed 26 Aug 20:28:08 BST 2026 keithdhedger@gmail.com

 * This file (trashCanClass.cpp) is part of DevProjects.

 * DevProjects is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * DevProjects is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DevProjects.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "trashCanClass.h"

trashCanClassClass::~trashCanClassClass()
{
	delete this->empty;
	delete this->full;
	delete this->updateStatus;
}

QString trashCanClassClass::runCommandsInShell(QString commands)
{
	QString retstr="";
	FILE		*fp=NULL;
	char		line[1024];

	fp=popen(qPrintable(commands),"r");
	if(fp!=NULL)
		{
			while(fgets(line,1024,fp))
				retstr+=line;
			pclose(fp);
		}
	return(retstr);
}

trashCanClassClass::trashCanClassClass()
{
	QString		notrash=QString("%1/user-trash.png").arg(DATADIR);
	QString		gottrash=QString("%1/user-trash-full.png").arg(DATADIR);

	this->empty=new QPixmap(notrash);
	this->full=new QPixmap(gottrash);
	this->quitAction=new QAction(QIcon::fromTheme("application-exit"),"Quit",this);
	QObject::connect(this->quitAction,&QAction::triggered,[this](bool checked)
		{
			qApp->quit();
		});

	this->showTrashAction=new QAction(QIcon::fromTheme("user-trash-full"),"Show Trash",this);
	QObject::connect(this->showTrashAction,&QAction::triggered,[this](bool checked)
		{
			QDesktopServices::openUrl(QUrl("trash:///"));
		});

	this->emptyTrashAction=new QAction(QIcon::fromTheme("user-trash"),"Empty Trash",this);
	QObject::connect(this->emptyTrashAction,&QAction::triggered,[this](bool checked)
		{
			this->runCommandsInShell("echo 'Y'|trash-empty");
		});

	this->updateStatus=new QTimer();
	this->updateStatus->start(500);
	QObject::connect(this->updateStatus,&QTimer::timeout,[this]()
		{
			this->doTimer();
		});

	this->setImage();
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window|Qt::WindowStaysOnBottomHint);
	this->setAcceptDrops(true);
}

void trashCanClassClass::doTimer(void)
{
	QRect	geom=this->geometry();

	if(this->hite!=geom.height())
		this->hite=geom.height();
	this->setImage();
}

void trashCanClassClass::setImage(void)
{
	QString		notrash=QString("%1/user-trash.png").arg(DATADIR);
	QString		gottrash=QString("%1/user-trash-full.png").arg(DATADIR);
	QPalette		p;
	QString		retstr;
	QPixmap		tm;
	
	retstr=this->runCommandsInShell("trash-list");

	if(retstr.length()>0)
		{
			tm=this->full->scaledToHeight(this->hite);
			this->setMask(tm.mask());
			p.setBrush(QPalette::Window,QBrush(tm));
		}
	else
		{
			tm=this->empty->scaledToHeight(this->hite);
			this->setMask(tm.mask());
			p.setBrush(QPalette::Window,QBrush(tm));
		}
	this->pmwid=tm.width();
	this->setPalette(p);   
}

void trashCanClassClass::mousePressEvent(QMouseEvent *event)
{ 
	if((event->modifiers() & Qt::ControlModifier)==Qt::ControlModifier)
		{
			if((event->modifiers() & Qt::ShiftModifier)==Qt::ShiftModifier)
				{
					this->hite-=32;
					if(this->hite<32)
						this->hite=32;
				}
			else
				{
					this->hite+=32;
					if(this->hite>354)
						this->hite=354;
				}

			QRect geom=this->geometry();

			this->setImage();
			geom.setHeight(this->hite);
			geom.setWidth(this->pmwid);
			this->setGeometry(geom);
			this->setImage();
		}

	if(event->button()==Qt::LeftButton)
		{
			this->m_dragPosition=(event->globalPosition()-frameGeometry().topLeft()).toPoint();
			event->accept();
		}
}

void trashCanClassClass::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons()==Qt::LeftButton)
		{
			this->move((event->globalPosition()-this->m_dragPosition).toPoint());
			event->accept();
		}
}

void trashCanClassClass::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu	menu(this);

	menu.addAction(this->emptyTrashAction);
	menu.addAction(this->showTrashAction);
	menu.addAction(this->quitAction);
	menu.exec(event->globalPos());

	event->accept();
}

void trashCanClassClass::dragEnterEvent(QDragEnterEvent *e)
{
	if(e->mimeData()->hasUrls())
		e->acceptProposedAction();
}

void trashCanClassClass::dropEvent(QDropEvent *e)
{
	foreach(const QUrl &url,e->mimeData()->urls())
		{
			QString fileName = url.toLocalFile();
     //   qDebug() << "Dropped file:" << fileName;
			this->runCommandsInShell(QString("trash-put '%1'").arg(fileName));
		}
}

void trashCanClassClass::showEvent(QShowEvent *e)
{
	this->doTimer();
	e->accept();
}