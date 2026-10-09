/*
 *
 * ©K. D. Hedger. Fri  9 Oct 13:47:16 BST 2026 keithdhedger@gmail.com

 * This file (cliPrefsClass.h) is part of TrashCan.

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

#ifndef _PREFSCLASS_
#define _PREFSCLASS_

#include <QCoreApplication>
#include <QDir>
#include <getopt.h>

class cliPrefsClass
{
	public:
		cliPrefsClass(QString pname="");
		~cliPrefsClass();

		QHash<int,QStringList>	prefsData;
		QStringList				extraCliArgs;

		bool						doCliArgs(int argc,char **argv,option longoptions[]);
		QStringList				getPrefValue(QString name);

	private:
		void						setPrefValue(QString name,QStringList val);
		void						appendStrPref(QString name,QString str);
};

#endif
