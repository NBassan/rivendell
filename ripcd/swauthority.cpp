// swauthority.cpp
//
// A Rivendell switcher driver for systems using Software Authority Protocol
//
//   (C) Copyright 2002-2015 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>

#include <qstringlist.h>

#include <rdescape_string.h>
#include <rddb.h>
#include <globals.h>
#include <swauthority.h>

SoftwareAuthority::SoftwareAuthority(RDMatrix *matrix,QObject *parent,const char *name)
  : Switcher(matrix,parent,name)
{
  swa_matrix=matrix->matrix();
  swa_ptr=0;
  swa_istate=0;

  //
  // Get Matrix Parameters
  //
  swa_username=matrix->username(RDMatrix::Primary);
  swa_password=matrix->password(RDMatrix::Primary);
  swa_porttype=matrix->portType(RDMatrix::Primary);
  swa_ipaddress=matrix->ipAddress(RDMatrix::Primary);
  swa_ipport=matrix->ipPort(RDMatrix::Primary);
  swa_inputs=0;
  swa_outputs=0;
  swa_gpis=matrix->gpis();
  swa_gpos=matrix->gpos();
  swa_start_cart=matrix->startCart(RDMatrix::Primary);
  swa_stop_cart=matrix->stopCart(RDMatrix::Primary);

  //
  // Reconnection Timer
  //
  swa_reconnect_timer=new QTimer(this,"swa_reconnect_timer");
  connect(swa_reconnect_timer,SIGNAL(timeout()),this,SLOT(ipConnect()));

  //
  // Initialize the connection
  //
  swa_socket=new QSocket(this,"swa_socket");
  connect(swa_socket,SIGNAL(connected()),this,SLOT(connectedData()));
  connect(swa_socket,SIGNAL(connectionClosed()),
	  this,SLOT(connectionClosedData()));
  connect(swa_socket,SIGNAL(readyRead()),
	  this,SLOT(readyReadData()));
  connect(swa_socket,SIGNAL(error(int)),this,SLOT(errorData(int)));
  ipConnect();
}


RDMatrix::Type SoftwareAuthority::type()
{
  return RDMatrix::SoftwareAuthority;
}


unsigned SoftwareAuthority::gpiQuantity()
{
  return swa_gpis;
}


unsigned SoftwareAuthority::gpoQuantity()
{
  return swa_gpos;
}


bool SoftwareAuthority::primaryTtyActive()
{
  return false;
}


bool SoftwareAuthority::secondaryTtyActive()
{
  return false;
}


void SoftwareAuthority::processCommand(RDMacro *cmd)
{
  switch(cmd->command()) {
    /*
      case RDMacro::FS:
	if((cmd->arg(1).toInt()<1)||(cmd->arg(1).toInt()>256)||
	   (swa_porttype!=RDMatrix::TcpPort)) {
	  cmd->acknowledge(false);
	  emit rmlEcho(cmd);
	  return;
	}
	snprintf(str,256,"%c1%03d\x0D\x0A",0x13,cmd->arg(1).toInt());
	SendCommand(str);
	cmd->acknowledge(true);
	emit rmlEcho(cmd);
	break;

    */
      case RDMacro::ST:
	if((cmd->arg(1).toInt()<0)||(cmd->arg(1).toInt()>swa_inputs)||
	   (cmd->arg(2).toInt()<1)||(cmd->arg(2).toInt()>swa_outputs)) {
	  cmd->acknowledge(false);
	  emit rmlEcho(cmd);
	  return;
	}
	SendCommand(QString().sprintf("activateroute 1 %d %d",
				      cmd->arg(2).toInt(),cmd->arg(1).toInt()));
	cmd->acknowledge(true);
	emit rmlEcho(cmd);
	break;
	/*
      case RDMacro::GO:
	if((cmd->arg(1).toString().lower()!="o")||
	   (cmd->arg(2).toInt()<1)||(cmd->arg(2).toInt()>swa_gpos)) {
	  cmd->acknowledge(false);
	  emit rmlEcho(cmd);
	  return;
	}
	if(cmd->arg(4).toInt()==0) {   // Latch
	  if(cmd->arg(3).toInt()==0) {   // Off
	    cmd_byte=0x03;
	  }
	  else {
	    cmd_byte=0x02;
	  }
	}
	else {
	  if(cmd->arg(3).toInt()==0) {
	    cmd->acknowledge(false);
	    emit rmlEcho(cmd);
	    return;
	  }
	  cmd_byte=0x01;
	}
	if(cmd->arg(2).toUInt()<swa_relay_numbers.size()) {
	  if(swa_relay_numbers[cmd->arg(2).toUInt()-1]>=0) {
	    snprintf(str,256,"\x05R%d%04d\x0D\x0A",cmd_byte,
		    swa_relay_numbers[cmd->arg(2).toUInt()-1]);
	    SendCommand(str);
	    cmd->acknowledge(true);
	    emit rmlEcho(cmd);
	  }
	  else {
	    if((swa_console_numbers[cmd->arg(2).toUInt()-1]>=0)&&
	       (swa_source_numbers[cmd->arg(2).toUInt()-1]>=0)) {
	      if(cmd->arg(3).toInt()==0) {   // Off
		cmd_byte=0;
	      }
	      else {
		cmd_byte=1;
	      }
	      snprintf(str,256,"\x1A%s%d%03d%04d\x0D\x0A","20",cmd_byte,
		      swa_console_numbers[cmd->arg(2).toUInt()-1],
		      swa_source_numbers[cmd->arg(2).toUInt()-1]);
	      SendCommand(str);
	      cmd->acknowledge(true);
	      emit rmlEcho(cmd);
	    }
	  }
	}
	break;
    */
      default:
	cmd->acknowledge(false);
	emit rmlEcho(cmd);
	break;
  }
}


void SoftwareAuthority::ipConnect()
{
  swa_socket->connectToHost(swa_ipaddress.toString(),swa_ipport);
}


void SoftwareAuthority::connectedData()
{
  SendCommand(QString("login")+" "+swa_username+" "+swa_password);
}


void SoftwareAuthority::connectionClosedData()
{
  LogLine(RDConfig::LogNotice,QString().
	  sprintf("Connection to SoftwareAuthority device at %s:%d closed unexpectedly, attempting reconnect",
		  (const char *)swa_ipaddress.toString(),
		  swa_ipport));
  if(swa_stop_cart>0) {
    ExecuteMacroCart(swa_stop_cart);
  }
  swa_reconnect_timer->start(SWAUTHORITY_RECONNECT_INTERVAL,true);
}


void SoftwareAuthority::readyReadData()
{
  char buffer[256];
  unsigned n;

  while((n=swa_socket->readBlock(buffer,255))>0) {
    buffer[n]=0;
    ///    printf("RECV: %s\n",buffer);
    for(unsigned i=0;i<n;i++) {
      if(buffer[i]==10) {  // End of line
	swa_buffer[--swa_ptr]=0;
	DispatchCommand();
	swa_ptr=0;
      }
      else {
	if(swa_ptr==SWAUTHORITY_MAX_LENGTH) {  // Buffer overflow
	  swa_ptr=0;
	}
	swa_buffer[swa_ptr++]=buffer[i];
      }
    }
  }
}


void SoftwareAuthority::errorData(int err)
{
  switch((QSocket::Error)err) {
      case QSocket::ErrConnectionRefused:
	LogLine(RDConfig::LogNotice,QString().sprintf(
	  "Connection to SoftwareAuthority device at %s:%d refused, attempting reconnect",
		  (const char *)swa_ipaddress.toString(),
		  swa_ipport));
	swa_reconnect_timer->start(SWAUTHORITY_RECONNECT_INTERVAL,true);
	break;

      case QSocket::ErrHostNotFound:
	LogLine(RDConfig::LogWarning,QString().sprintf(
	  "Error on connection to SoftwareAuthority device at %s:%d: Host Not Found",
		  (const char *)swa_ipaddress.toString(),
		  swa_ipport));
	break;

      case QSocket::ErrSocketRead:
	LogLine(RDConfig::LogWarning,QString().sprintf(
	  "Error on connection to SoftwareAuthority device at %s:%d: Socket Read Error",
				  (const char *)swa_ipaddress.toString(),
				  swa_ipport));
	break;
  }
}


void SoftwareAuthority::SendCommand(const char *str)
{
  //  LogLine(RDConfig::LogDebug,QString().sprintf("sending SA cmd: %s",(const char *)PrettifyCommand(str)));
  QString cmd=QString().sprintf("%s\x0d\x0a",(const char *)str);
  swa_socket->writeBlock((const char *)cmd,strlen(cmd));
}


void SoftwareAuthority::DispatchCommand()
{
  char buffer[SWAUTHORITY_MAX_LENGTH];
  QString cmd;
  QString label;
  QString sql;
  RDSqlQuery *q;
  QStringList f0;
  QString name;

  //  LogLine(RDConfig::LogNotice,QString().sprintf("RECEIVED: %s",(const char *)swa_buffer));

  QString line_in=swa_buffer;
  QString section=line_in.lower().replace(">>","");

  //
  // Startup Sequence.  Get the input and output lists.
  //
  if(section=="login successful") {
    sprintf(buffer,"sourcenames 1\x0D\x0A");  // Request Input List
    SendCommand(buffer);
    sprintf(buffer,"destnames 1\x0D\x0A");    // Request Output List
    SendCommand(buffer);
    return;
  }
  if(section=="login failure") {
    LogLine(RDConfig::LogWarning,QString().sprintf(
	    "Error on connection to SoftwareAuthority device at %s:%d: Login Failure",
	    (const char *)swa_ipaddress.toString(),
	    swa_ipport));
    swa_socket->close();
    return;
  }

  switch(swa_istate) {
  case 0:   // No section selected
    if(section=="begin sourcenames - 1") {
      swa_istate=1;
      swa_inputs=0;
      return;
    }
    if(section=="begin destnames - 1") {
      swa_istate=2;
      swa_outputs=0;
      return;
    }
    break;

  case 1:   // Source List
    if(section=="end sourcenames - 1") {
      swa_istate=0;
      sql=QString("update MATRICES set ")+
	QString().sprintf("INPUTS=%d ",swa_inputs)+
	"where (STATION_NAME=\""+RDEscapeString(rdstation->name())+"\")&&"+
	QString().sprintf("(MATRIX=%d)",swa_matrix);
      q=new RDSqlQuery(sql);
      delete q;
      return;
    }
    swa_inputs++;
    f0=f0.split("\t",line_in);
    name=f0[1];
    if(f0.size()>=7) {
      name=f0[6]+": "+f0[2];
    }
    sql=QString().sprintf("select NUMBER from INPUTS where \
                           (STATION_NAME=\"%s\")&&	   \
                           (MATRIX=%d)&&(NUMBER=%d)",
			  (const char *)rdstation->name(),
			  swa_matrix,f0[0].toInt());
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString().sprintf("update INPUTS set NAME=\"%s\" where \
                            (STATION_NAME=\"%s\")&&\
                            (MATRIX=%d)&&(NUMBER=%d)",
			    (const char *)name,
			    (const char *)rdstation->name(),
			    swa_matrix,f0[0].toInt());
    }
    else {
      sql=QString().sprintf("insert into INPUTS set NAME=\"%s\",\
                            STATION_NAME=\"%s\",MATRIX=%d,NUMBER=%d",
			    (const char *)name,
			    (const char *)rdstation->name(),
			    swa_matrix,f0[0].toInt());
    }
    delete q;
    q=new RDSqlQuery(sql);
    delete q;
    break;

  case 2:   // Destinations List
    if(section=="end destnames - 1") {
      swa_istate=0;
      sql=QString("update MATRICES set ")+
	QString().sprintf("OUTPUTS=%d ",swa_outputs)+
	"where (STATION_NAME=\""+RDEscapeString(rdstation->name())+"\")&&"+
	QString().sprintf("(MATRIX=%d)",swa_matrix);
      q=new RDSqlQuery(sql);
      delete q;

      LogLine(RDConfig::LogInfo,QString().
	      sprintf("Connection to SoftwareAuthority device at %s:%d established",
		      (const char *)swa_ipaddress.toString(),
		      swa_ipport));
      if(swa_start_cart>0) {
	ExecuteMacroCart(swa_start_cart);
      }
      return;
    }
    swa_outputs++;
    f0=f0.split("\t",line_in);
    name=f0[1];
    if(f0.size()>=6) {
      name=f0[3]+"/"+f0[5]+": "+f0[2];
    }
    sql=QString().sprintf("select NUMBER from OUTPUTS where \
                           (STATION_NAME=\"%s\")&&\
                           (MATRIX=%d)&&(NUMBER=%d)",
			  (const char *)rdstation->name(),
			  swa_matrix,f0[0].toInt());
    q=new RDSqlQuery(sql);
    if(q->first()) {
      sql=QString().sprintf("update OUTPUTS set NAME=\"%s\" where \
                             (STATION_NAME=\"%s\")&&\
                             (MATRIX=%d)&&(NUMBER=%d)",
			    (const char *)name,
			    (const char *)rdstation->name(),
			    swa_matrix,f0[0].toInt());
    }
    else {
      sql=QString().sprintf("insert into OUTPUTS set NAME=\"%s\",\
                             STATION_NAME=\"%s\",MATRIX=%d,NUMBER=%d",
			    (const char *)name,
			    (const char *)rdstation->name(),
			    swa_matrix,f0[0].toInt());
    }
    delete q;
    q=new RDSqlQuery(sql);
    delete q;
    break;
  }
}


void SoftwareAuthority::ExecuteMacroCart(unsigned cartnum)
{
  RDMacro rml;
  rml.setRole(RDMacro::Cmd);
  rml.setCommand(RDMacro::EX);
  rml.setAddress(rdstation->address());
  rml.setEchoRequested(false);
  rml.setArgQuantity(1);
  rml.setArg(0,cartnum);
  emit rmlEcho(&rml);
}


QString SoftwareAuthority::PrettifyCommand(const char *cmd) const
{
  QString ret;
  if(cmd[0]<26) {
    ret=QString().sprintf("^%c%s",'@'+cmd[0],cmd+1);
  }
  else {
    ret=cmd;
  }
  return ret;
}