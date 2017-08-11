// rdcartdrag.cpp
//
// Stored value drag object for Rivendell carts.
//
//   (C) Copyright 2013,2016 Fred Gleason <fredg@paravelsystems.com>
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

#include <string.h>

#include <qstringlist.h>
//Added by qt3to4:
#include <QPixmap>

#include <rd.h>
#include <rdcart.h>
#include <rdcartdrag.h>
#include <rdprofile.h>

//
// Icons
//
#include "../icons/play.xpm"
#include "../icons/rml5.xpm"
#include "../icons/trashcan-16x16.xpm"

RDCartDrag::RDCartDrag()
  : QMimeData()//Q3StoredDrag(RDMIMETYPE_CART,src)
{

}

QPixmap RDCartDrag::getPixmap(unsigned cartnum){

    if(cartnum==0) {
      return(QPixmap(trashcan_xpm));
    }
    else {
      RDCart *cart=new RDCart(cartnum);
      switch(cart->type()) {
      case RDCart::Audio:
        delete cart;
        return(QPixmap(play_xpm));
        break;

      case RDCart::Macro:
        delete cart;
        return(QPixmap(rml5_xpm));
        break;

      case RDCart::All:
        delete cart;
        break;
      }
    }
    return QPixmap();
}


/*bool RDCartDrag::canDecode(QMimeSource *e)
{
  return e->provides(RDMIMETYPE_CART);
}


bool RDCartDrag::decode(QMimeSource *e,unsigned *cartnum,QColor *color,
			QString *title)
{
  RDProfile *p=new RDProfile();
  p->setSourceString(e->encodedData(RDMIMETYPE_CART));
  *cartnum=p->intValue("Rivendell-Cart","Number");
  if(color!=NULL) {
    color->setNamedColor(p->stringValue("Rivendell-Cart","Color"));
  }
  if(title!=NULL) {
    *title=p->stringValue("Rivendell-Cart","ButtonText");
  }

  return true;
}


bool RDCartDrag::decode(QMimeSource *e,RDLogLine *ll,
			RDLogLine::TransType next_trans,int log_mach,
			bool timescale,RDLogLine::TransType trans)
{
  unsigned cartnum;

  RDCartDrag::decode(e,&cartnum);
  ll->loadCart(cartnum,next_trans,log_mach,timescale,trans);

  return true;
}*/

bool RDCartDrag::hasFormat(const QString &mimeType){
    return mimeType==RDMIMETYPE_CART;
}

QStringList RDCartDrag::formats() const{
    return QStringList(RDMIMETYPE_CART);
}

QVariant RDCartDrag::retrieveData(const QString &mimeType, QVariant::Type type) const{
    if(mimeType==RDMIMETYPE_CART)
        return QMimeData::retrieveData(RDMIMETYPE_CART,type);
    else
        return 0;
}


void RDCartDrag::decodeCartData(QByteArray result, unsigned *cartnum,QColor *color,QString *title) {
    RDProfile *p=new RDProfile();
    p->setSourceString(QString(result));
    *cartnum=p->intValue("Rivendell-Cart","Number");
    if(color!=NULL) {
        color->setNamedColor(p->stringValue("Rivendell-Cart","Color"));
    }
    if(title!=NULL) {
        *title=p->stringValue("Rivendell-Cart","ButtonText");
    }
    return;
}

void RDCartDrag::decodeCartData(QByteArray result, RDLogLine *ll,RDLogLine::TransType next_trans,int log_mach,
                              bool timescale,RDLogLine::TransType trans) {
    unsigned cartnum;
    RDCartDrag::decodeCartData(result, &cartnum);
    ll->loadCart(cartnum,next_trans,log_mach,timescale,trans);
    return;
}


void RDCartDrag::setCartData(unsigned cartnum, const QString &title, const QColor &color)
{
  QString str="[Rivendell-Cart]\n";
  str+="Number="+QString().sprintf("%06u",cartnum)+"\n";
  if(color.isValid()) {
    str+="Color="+color.name()+"\n";
  }
  if(!title.isEmpty()) {
    str+="ButtonText="+title+"\n";
  }
  QByteArray data(str.length());
  data.duplicate(str,str.length());
  QMimeData::setData(RDMIMETYPE_CART,data);
}
