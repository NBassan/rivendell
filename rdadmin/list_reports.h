// list_reports.h
//
// List Rivendell Reports
//
//   (C) Copyright 2002-2004,2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#ifndef LIST_REPORTS_H
#define LIST_REPORTS_H

#include <qdialog.h>
#include <q3listbox.h>
#include <q3textedit.h>
#include <qpixmap.h>
#include <qradiobutton.h>
#include <qsqldatabase.h>

class ListReports : public QDialog
{
  Q_OBJECT
  public:
   ListReports(QWidget *parent=0);
   ~ListReports();
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  private slots:
   void addData();
   void editData();
   void deleteData();
   void doubleClickedData(Q3ListBoxItem *item);
   void closeData();

  private:
   void DeleteReport(QString rptname);
   void RefreshList(QString rptname="");
   Q3ListBox *list_box;
};


#endif


