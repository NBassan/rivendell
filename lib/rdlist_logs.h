// rdlist_logs.h
//
// Select a Rivendell Log
//
//   (C) Copyright 2007,2016 Fred Gleason <fredg@paravelsystems.com>
//
// The RDListLogs class creates a basic dialog that displays a list of logs
// (log name, description, and service) and allows the user to select one.  If
// user security is enabled (by passing a an RDUser object to the constructor)
// then the list of logs is filtered based on the users permissions.
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

#ifndef RDLIST_LOGS_H
#define RDLIST_LOGS_H

#include <qsqldatabase.h>
#include <qdialog.h>
#include <q3listview.h>
#include <qpushbutton.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <rduser.h>

class RDListLogs : public QDialog
{
  Q_OBJECT

 public:
  /**
   * Constructor for the RDListLogs object.
   *
   * NOTE: the presence of the optional rduser parameter is used to flag if
   * user security should be used instead of host based security.
   */
  RDListLogs(QString *logname,const QString &stationname,
	     QWidget *parent=0,RDUser *rduser=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void doubleClickedData(Q3ListViewItem *,const QPoint &,int);
  void closeEvent(QCloseEvent *);
  void okButtonData();
  void cancelButtonData();

 private:
  void RefreshList();
  Q3ListView *list_log_list;
  QString *list_logname;
  QString list_stationname;
  RDUser *list_user;
};


#endif  // RDLIST_LOGS_H
