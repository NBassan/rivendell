# rdchunk.pro
#
# The utils/rdchunk/ QMake project file for Rivendell
#
# (C) Copyright 2003-2006,2008,2016 Fred Gleason <fredg@paravelsystems.com>
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

SOURCES += rdchunk.cpp
HEADERS += rdchunk.h

TRANSLATIONS += rdchunk_es.ts
TRANSLATIONS += rdchunk_fr.ts
TRANSLATIONS += rdchunk_de.ts
TRANSLATIONS += rdchunk_pt_BR.ts
#The following line was inserted by qt3to4
QT +=  qt3support 
