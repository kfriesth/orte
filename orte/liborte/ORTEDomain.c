/*
 *  $Id: ORTEDomain.c,v 0.0.0.1         2003/08/21
 *
 *  DEBUG:  section 30                  Domain functions
 *  AUTHOR: Petr Smolik                 petr.smolik@wo.cz
 *
 *  ORTE - OCERA Real-Time Ethernet     http://www.ocera.org/
 *  --------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 */ 

#include "orte.h"

/*****************************************************************************/
Boolean
ORTEDomainPropDefaultGet(ORTEDomainProp *prop) {
  sock_t        sock;

  memset(prop, 0, sizeof(*prop));

  prop->multicast.enabled=ORTE_FALSE;
  prop->multicast.ttl=1;
  prop->multicast.loopBackEnabled=ORTE_TRUE;

  //IFProp
  sock_init_udp(&sock);
  sock_bind(&sock,0);
  sock_get_local_interfaces(&sock,prop->IFProp,&prop->IFCount);
  sock_cleanup(&sock); 

  prop->mgrs=NULL;
  prop->appLocalManager=StringToIPAddress("127.0.0.1");
  prop->mgrAddKey=0;
  sprintf(prop->version,PACKAGE_STRING\
                        ", compiled: "\
                        __DATE__\
                        " "\
                        __TIME__);
                        
  prop->recvBuffSize=0x4000;
  prop->sendBuffSize=0x4000; 
  prop->wireProp.metaBytesPerPacket=0x2000;
  prop->wireProp.metaBytesPerFastPacket=0x1000;
  prop->wireProp.metabitsPerACKBitmap=32;
  prop->wireProp.userMaxSerDeserSize=0x4000;
  
  //domainBaseProp
  NTPTIME_BUILD(prop->baseProp.expirationTime,180); //180s
  NTPTIME_BUILD(prop->baseProp.refreshPeriod,72);    //72s - refresh self parameters
  NTPTIME_BUILD(prop->baseProp.purgeTime,60);        //60s - purge time of parameters
  NTPTIME_BUILD(prop->baseProp.repeatAnnounceTime,72);//72s - announcement by HB
  NTPTIME_BUILD(prop->baseProp.repeatActiveQueryTime,72);//72s - announcement by ACK
  NtpTimeAssembFromMs(prop->baseProp.delayResponceTimeACKMin,0,10);//10ms - delay before send ACK
  NtpTimeAssembFromMs(prop->baseProp.delayResponceTimeACKMax,1,0);//1s
  NtpTimeAssembFromMs(prop->baseProp.maxBlockTime,20,0);//20s
  prop->baseProp.ACKMaxRetries=10;
  prop->baseProp.HBMaxRetries=10;
  
  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTEDomainInitEvents(ORTEDomainAppEvents *events) {
  memset(events,0,sizeof(ORTEDomainAppEvents));
  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTEPublPropDefaultGet(ORTEDomain *d,ORTEPublProp *pp) {
  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTEPublPropDefaultSet(ORTEDomain *d,ORTEPublProp *pp) {
  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTESubsPropDefaultGet(ORTEDomain *d,ORTESubsProp *sp) {
  return ORTE_TRUE;
}

/*****************************************************************************/
Boolean
ORTESubsPropDefaultSet(ORTEDomain *d,ORTESubsProp *sp) {
  return ORTE_TRUE;
}
