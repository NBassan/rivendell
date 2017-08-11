//   rdlistselector.h
//
//   An listselector widget with word wrap.
//
//   (C) Copyright 2002,2016 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef RDLISTSELECTOR_H
#define RDLISTSELECTOR_H

#include <qwidget.h>
#include <qlabel.h>
#include <QHBoxLayout>
#include <qcolor.h>
#include <q3listbox.h>
#include <qpushbutton.h>

class RDListSelector : public QWidget
{
  Q_OBJECT

 public:
  RDListSelector(QWidget *parent=0);
  uint sourceCount() const;
  uint destCount() const;
  void sourceSetLabel(QString label);
  void destSetLabel(QString label);
  void sourceInsertItem(const QString &text,int index=-1);
  void destInsertItem(const QString &text,int index=-1);
  void sourceRemoveItem(int index);
  void destRemoveItem(int index);
  QString sourceText(int index) const;
  QString destText(int index) const;
  void sourceChangeItem(const QString &text,int index);
  void destChangeItem(const QString &text,int index);
  int sourceNumItemsVisible() const;
  int destNumItemsVisible() const;
  int sourceCurrentItem() const;
  int destCurrentItem() const;

    enum StringComparisonMode {
        CaseSensitive   = 0x00001, // 0 0001
        BeginsWith      = 0x00002, // 0 0010
        EndsWith        = 0x00004, // 0 0100
        Contains        = 0x00008, // 0 1000
        ExactMatch      = 0x00010  // 1 0000
    };
    typedef uint ComparisonFlags;


  QString sourceCurrentText() const;
  QString destCurrentText() const;
  void sourceSetCurrentItem(int item);
  void destSetCurrentItem(int item);
  Q3ListBoxItem *sourceFindItem(const QString &text,
			       ComparisonFlags compare=ExactMatch) const;
  Q3ListBoxItem *destFindItem(const QString &text,
			     ComparisonFlags compare=ExactMatch) const;
  void clear();
  
 private slots:
  void addData();
  void removeData();

 private:
  void CheckButtons();
  Q3ListBox *list_source_box;
  QLabel *list_source_label;
  Q3ListBox *list_dest_box;
  QLabel *list_dest_label;
  QPushButton *list_add_button;
  QPushButton *list_remove_button;
  QHBoxLayout *list_qhbox_layout;
};


#endif  // RDLISTSELECTOR_H
