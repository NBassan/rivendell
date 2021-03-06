//   lib_listview.cpp
//
//   The Log ListView widget for RDAirPlay's Full Log widget.
//
//   (C) Copyright 2002-2013,2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <q3header.h>
//Added by qt3to4:
#include <QDropEvent>
#include <QDragEnterEvent>

#include <rdcartdrag.h>

#include <lib_listview.h>

LibListView::LibListView(QWidget *parent)
  : RDListView(parent)
{
  setAcceptDrops(true);
}


void LibListView::dragEnterEvent(QDragEnterEvent *e)
{
 // e->accept(RDCartDrag::canDecode(e));
    if(e->mimeData()->hasFormat(RDMIMETYPE_CART))
        e->acceptProposedAction();
}


void LibListView::dropEvent(QDropEvent *e)
{
  RDLogLine ll;
  int line=-1;
  QPoint pos(e->pos().x(),e->pos().y()-header()->sectionRect(0).height());

  if(e->mimeData()->hasFormat(RDMIMETYPE_CART)){
     QByteArray result=e->mimeData()->data(RDMIMETYPE_CART);
     RDCartDrag *data = new RDCartDrag;
     data->decodeCartData(result,&ll);
     delete data;

     RDListViewItem *item=(RDListViewItem *)itemAt(pos);
     if(item!=NULL) {
       line=item->text(15).toInt();
     }
     emit cartDropped(line,&ll);
  }

/*  if(RDCartDrag::decode(e,&ll)) {
  }*/
}
