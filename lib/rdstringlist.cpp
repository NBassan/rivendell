//   rdstringlist.cpp
//
//   A StringList with quote mode
//
//   (C) Copyright 2010,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <rdstringlist.h>

RDStringList::RDStringList()
  : QStringList()
{
}


RDStringList::RDStringList(const RDStringList &lhs)
  : QStringList(lhs)
{
}


RDStringList::RDStringList(const QStringList &lhs)
  : QStringList(lhs)
{
}


RDStringList RDStringList::split(const QString &sep,const QString &str,
				 const QString &esc)
{
  if(esc.isEmpty()) {
    //return (RDStringList)QStringList::split(sep,str);
    return (RDStringList)str.QString::split(sep,QString::SkipEmptyParts);
  }
  RDStringList list;
  bool escape=false;
  QChar e=esc.at(0);
  list.push_back(QString());
  for(int i=0;i<str.length();i++) {
    if(str.at(i)==e) {
      escape=!escape;
    }
    else {
      //added .at(0) to explicitly cast string to char
      if((!escape)&&(str.at(i)==sep.at(0))) {
	list.push_back(QString());
      }
      else {
	list.back()+=str.at(i);
      }
    }
  }
  return list;
}
