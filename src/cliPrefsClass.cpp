/*
 *
 * ©K. D. Hedger. Fri  9 Oct 13:47:07 BST 2026 keithdhedger@gmail.com

 * This file (cliPrefsClass.cpp) is part of TrashCan.

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

#include "cliPrefsClass.h"

/**
* this->dialogPrefsClass class destroy.
*/
cliPrefsClass::~cliPrefsClass()
{
}

/**
* this->dialogPrefsClass.
*/
cliPrefsClass::cliPrefsClass(QString pname)
{
}

void cliPrefsClass::appendStrPref(QString name,QString str)
{
	if(this->prefsData.contains(qHash(name)))
		this->setPrefValue(name,this->getPrefValue(name)<<str);
	else
		this->prefsData[qHash(name)]=QStringList({str});
}

void cliPrefsClass::setPrefValue(QString name,QStringList val)
{
	this->prefsData[qHash(name)]=val;
}

QStringList cliPrefsClass::getPrefValue(QString name)
{
	return(this->prefsData.value(qHash(name)));
}

bool cliPrefsClass::doCliArgs(int argc,char **argv,option longoptions[])
{
	int			ocnt=0;
	int			c;
	std::string	optstr="";
	int			option_index;

	while(longoptions[ocnt].name!=0)
		{
			optstr+=longoptions[ocnt].val;
			if(longoptions[ocnt].has_arg!=no_argument)
				{
					if(longoptions[ocnt].has_arg==required_argument)
						optstr+=":";
					if(longoptions[ocnt].has_arg==optional_argument)
						optstr+="::";
				}
			ocnt++;
		}

	optstr+="?h";

	while(1)
		{
			option_index=0;
			c=getopt_long(argc,argv,optstr.c_str(),longoptions,&option_index);
			if(c==-1)
				break;
			if(c=='?' || c=='h')
				return(false);

			ocnt=0;
			while(longoptions[ocnt].name!=0)
				{
					if(longoptions[ocnt].val==c)
						{
							if(optarg!=NULL)
								{
									this->appendStrPref(longoptions[ocnt].name,QString(optarg));
								}
							else
								{
									if(longoptions[ocnt].has_arg==optional_argument)
										this->appendStrPref(longoptions[ocnt].name,QString(optarg));
									else
										this->appendStrPref(longoptions[ocnt].name,"");
								}
						}
					ocnt++;
				}			
		}

	while(optind<argc)
		{
			this->extraCliArgs<<argv[optind];
			optind++;
		}
	return(true);
}
