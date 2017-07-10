# rdsoftkeys.pro
#
# The utils/rdsoftkeys QMake project file for Rivendell
#
# (C) Copyright 2003-2006,2016 Fred Gleason <fredg@paravelsystems.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License version 2 as
#   published by the Free Software Foundation.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public
#   License along with this program; if not, write to the Free Software
#   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

TEMPLATE = app

TARGET = rdsoftkeys

win32 {
  DEFINES += WIN32
  DEFINES += VERSION=\"$$[VERSION]\"
  DEFINES += PACKAGE=\"rivendell\" 
  DEFINES += PACKAGE_VERSION=\"$$[VERSION]\" 
  DEFINES += PACKAGE_NAME=\"rivendell\"
  DEFINES += PACKAGE_BUGREPORT=\"fredg@paravelsystems.com\"
}

SOURCES += rdsoftkeys.cpp
HEADERS += rdsoftkeys.h

RES_FILE += ..\..\icons\rivendell.res

INCLUDEPATH += ..\..\..\libradio\radio ..\..\lib

LIBS = -lqui -L..\..\..\libradio\radio -lradio -L..\..\lib -llib

CONFIG += qt
#The following line was inserted by qt3to4
QT += network  sql qt3support 
#The following line was inserted by qt3to4
QT +=  
#The following line was inserted by qt3to4
QT +=  
#The following line was inserted by qt3to4
QT +=  
#The following line was inserted by qt3to4
QT +=  
