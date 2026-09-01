/*
 *
 * ©K. D. Hedger. Sat 29 Aug 19:16:53 BST 2026 keithdhedger@gmail.com

 * This file (trashCanClass.cpp) is part of TrashCan.

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

#include <gio/gio.h>

#include "trashCanClass.h"

trashCanClassClass::~trashCanClassClass()
{
	delete this->empty;
	delete this->full;
	delete this->updateStatus;
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
			this->emptyBin();
		});

	this->updateStatus=new QTimer();
	this->updateStatus->start(500);
	QObject::connect(this->updateStatus,&QTimer::timeout,[this]()
		{
			this->doTimer();
		});

	this->imageLabel=new QLabel;
    this->imageLabel->setAlignment(Qt::AlignLeft);
	this->imageLabel->setMinimumWidth(16);
	this->imageLabel->setMinimumHeight(16);
	this->setCentralWidget(this->imageLabel);

	this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnBottomHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAcceptDrops(true);
	this->setImage();
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
	QPixmap *backgroundImage;
	bool empty=this->checkBinOccupied();
	if(empty==true)
		backgroundImage=this->full;
	else
		backgroundImage=this->empty;

	this->imageLabel->setAlignment(Qt::AlignLeft);
	this->imageLabel->setGeometry(0,0,this->width(),this->height());
 
	QPixmap bi=backgroundImage->scaledToHeight(this->hite,Qt::SmoothTransformation);
 
	this->imageLabel->setPixmap(bi);
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

			geom.setHeight(this->hite);
			geom.setWidth((int)((double)(this->hite*0.723)+0.5));
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
			QFile file(url.toLocalFile());
			file.moveToTrash();
		}
}

void trashCanClassClass::showEvent(QShowEvent *e)
{
	this->doTimer();
	e->accept();
}

void trashCanClassClass::emptyBin(void)
{
	GError			*error=NULL;
	GFileInfo		*info=NULL;
	GFileEnumerator	*enumerator=NULL;
	GFile			*trash=NULL;

	trash=g_file_new_for_uri("trash:///");

	enumerator=g_file_enumerate_children(trash,G_FILE_ATTRIBUTE_STANDARD_NAME,G_FILE_QUERY_INFO_NONE,NULL,&error);
	if(enumerator==NULL)
		{
			fprintf(stderr, "Cannot open trash: %s\n", error->message);
			g_clear_error(&error);
			g_object_unref(trash);
			return;
		}

	while((info=g_file_enumerator_next_file(enumerator,NULL,&error)) != NULL)
		{
			GFile	*item=g_file_enumerator_get_child(enumerator,info);

			if(!g_file_delete(item,NULL,&error))
				{
					fprintf(stderr,"Can't delete %s\n",g_file_info_get_display_name(info));
					g_clear_error(&error);
				}

			g_object_unref(item);
			g_object_unref(info);
		}

	if(error)
		{
			g_printerr("Trash enumeration failed: %s\n",error->message);
			g_clear_error(&error);
		}

	g_object_unref(enumerator);
	g_object_unref(trash);
}

bool trashCanClassClass::checkBinOccupied(void)
{
	GFile			*trash=NULL;
    GFileEnumerator	*enumerator=NULL;
	GFileInfo		*info=NULL;
    GError 			*error=NULL;
	bool				retval=false;

	trash=g_file_new_for_uri("trash:///");

	enumerator=g_file_enumerate_children(trash,G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME,G_FILE_QUERY_INFO_NONE,NULL,&error);
	if(enumerator==NULL)
		{
			fprintf(stderr, "Cannot open trash: %s\n", error->message);
			g_error_free(error);
			g_object_unref(trash);
			return(false);
		}
 
	info=g_file_enumerator_next_file(enumerator,NULL,&error);
	if(info != NULL)
		{
			retval=true;
			g_object_unref(info);
		}

	g_file_enumerator_close(enumerator, NULL,&error);
	if(error != NULL)
		g_error_free(error);

	g_object_unref(enumerator);
	g_object_unref(trash);
	return(retval);
}