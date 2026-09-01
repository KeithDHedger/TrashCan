/*
 *
 * ©K. D. Hedger. Wed 26 Aug 20:28:08 BST 2026 keithdhedger@gmail.com

 * This file (trashCanClass.h) is part of TrashCan.

 * TrashCan is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation,either version 3 of the License,or
 * (at your option) any later version.

 * TrashCan is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY;without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with TrashCan.  If not,see <http://www.gnu.org/licenses/>.
*/

#ifndef _TRASHCANCLASS_
#define _TRASHCANCLASS_

#include <QtWidgets>

class trashCanClassClass:public QMainWindow
{
	public:

		trashCanClassClass();
		~trashCanClassClass();

		void		mousePressEvent(QMouseEvent *event) override;
		void		mouseMoveEvent(QMouseEvent *event) override;
		void		dragEnterEvent(QDragEnterEvent *e) override;
		void		dropEvent(QDropEvent *e) override;
		void		contextMenuEvent(QContextMenuEvent *event) override;
		void		showEvent(QShowEvent *event) override;

	private:
		QPoint	m_dragPosition;
		QPixmap	*empty;
		QPixmap	*full;
		QTimer	*updateStatus=NULL;
		QAction	*quitAction=NULL;
		QAction	*showTrashAction=NULL;
		QAction	*emptyTrashAction=NULL;
		QLabel	*imageLabel=NULL;
		int		hite=-1;

		void		emptyBin(void);
		bool		checkBinOccupied(void);
		void		doTimer(void);
		void		setImage(void);






};

#endif
