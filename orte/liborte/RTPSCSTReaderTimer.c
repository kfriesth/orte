/*
 *  $Id: RTPSCSTReaderTimer.c,v 0.0.0.1 2003/11/03 
 *
 *  DEBUG:  section 55                  CSTReader timer functions
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
int 
CSTReaderResponceTimer(ORTEDomain *d,void *vcstRemoteWriter) {
  CSTRemoteWriter *cstRemoteWriter=(CSTRemoteWriter*)vcstRemoteWriter;
  int32_t         len;
  
  if (!d->mbSend.containsInfoReply) { 
    len=RTPSInfoREPLYCreate(
        d->mbSend.cdrStream.bufferPtr,
        getMaxMessageLength(d),
        IPADDRESS_INVALID,
        ((AppParams*)cstRemoteWriter->cstReader->objectEntryOID->attributes)->metatrafficUnicastPort);
    if (len<0) {
      d->mbSend.needSend=ORTE_TRUE;
      return 1;
    }
    d->mbSend.containsInfoReply=ORTE_TRUE;  
    d->mbSend.cdrStream.bufferPtr+=len;
    d->mbSend.cdrStream.length+=len;
    debug(55,3) ("sent: RTPS_InfoREPLY(0x%x) to 0x%x-0x%x\n",
                  cstRemoteWriter->cstReader->guid.oid,
                  cstRemoteWriter->guid.hid,
                  cstRemoteWriter->guid.aid);
  }
  len=RTPSAckCreate(
       d->mbSend.cdrStream.bufferPtr,
       getMaxMessageLength(d),
       &cstRemoteWriter->sn,
       cstRemoteWriter->cstReader->guid.oid,
       cstRemoteWriter->guid.oid,
       ORTE_TRUE);
  if (len<0) {
    //not enought space in sending buffer
    d->mbSend.needSend=ORTE_TRUE;
    return 1;
  }
  d->mbSend.cdrStream.bufferPtr+=len;
  d->mbSend.cdrStream.length+=len;
  debug(55,3) ("sent: RTPS_ACKF(0x%x) to 0x%x-0x%x\n",
                cstRemoteWriter->cstReader->guid.oid,
                cstRemoteWriter->guid.hid,
                cstRemoteWriter->guid.aid);
  if (cstRemoteWriter->commStateACK==PULLING) {
    eventDetach(d,
        cstRemoteWriter->objectEntryOID->objectEntryAID,
        &cstRemoteWriter->delayResponceTimer,
        1);  //metatraffic timer
    if (cstRemoteWriter->ACKRetriesCounter<
        cstRemoteWriter->cstReader->params.ACKMaxRetries) {
      cstRemoteWriter->ACKRetriesCounter++;
      eventAdd(d,
          cstRemoteWriter->objectEntryOID->objectEntryAID,
          &cstRemoteWriter->delayResponceTimer,
          1,   //metatraffic timer
          "CSTReaderResponceTimer",
          CSTReaderResponceTimer,
          &cstRemoteWriter->cstReader->lock,
          cstRemoteWriter,
          &cstRemoteWriter->cstReader->params.delayResponceTimeMin);
    } else {
      debug(55,3) ("sent: maxRetries ritch upper level (%d).\n",
                    cstRemoteWriter->cstReader->params.ACKMaxRetries);
    }
  }
  if (cstRemoteWriter->commStateACK==ACKPENDING) { 
    cstRemoteWriter->commStateACK=WAITING;
    eventDetach(d,
        cstRemoteWriter->objectEntryOID->objectEntryAID,
        &cstRemoteWriter->repeatActiveQueryTimer,
        1);   //metatraffic timer
    if (NtpTimeCmp(cstRemoteWriter->cstReader->
                   params.repeatActiveQueryTime,iNtpTime)!=0) {
      eventAdd(d,
          cstRemoteWriter->objectEntryOID->objectEntryAID,
          &cstRemoteWriter->repeatActiveQueryTimer,
          1,   //metatraffic timer
          "CSTReaderQueryTimer",
          CSTReaderQueryTimer,
          &cstRemoteWriter->cstReader->lock,
          cstRemoteWriter,
          &cstRemoteWriter->cstReader->params.repeatActiveQueryTime);
    }
  }
  return 0;
}

/*****************************************************************************/
int
CSTReaderQueryTimer(ORTEDomain *d,void *vcstRemoteWriter) {
  CSTRemoteWriter *cstRemoteWriter=(CSTRemoteWriter*)vcstRemoteWriter;
  int32_t         len;
  
  if (!d->mbSend.containsInfoReply) { 
    len=RTPSInfoREPLYCreate(
        d->mbSend.cdrStream.bufferPtr,
        getMaxMessageLength(d),
        IPADDRESS_INVALID,
        ((AppParams*)cstRemoteWriter->cstReader->objectEntryOID->attributes)->metatrafficUnicastPort);
    if (len<0) {
      d->mbSend.needSend=ORTE_TRUE;
      return 1;
    }
    d->mbSend.containsInfoReply=ORTE_TRUE;  
    d->mbSend.cdrStream.bufferPtr+=len;
    d->mbSend.cdrStream.length+=len;
    debug(55,3) ("sent: RTPS_InfoREPLY(0x%x) to 0x%x-0x%x\n",
                  cstRemoteWriter->cstReader->guid.oid,
                  cstRemoteWriter->guid.hid,
                  cstRemoteWriter->guid.aid);
  }
  len=RTPSAckCreate(
      d->mbSend.cdrStream.bufferPtr,
      getMaxMessageLength(d),
      &cstRemoteWriter->sn,
      cstRemoteWriter->cstReader->guid.oid,
      cstRemoteWriter->guid.oid,
      ORTE_FALSE);
  if (len<0) {
    //not enought space in sending buffer
    d->mbSend.needSend=ORTE_TRUE;
    return 1;
  }
  debug(55,3) ("sent: RTPS_ACKf(0x%x) to 0x%x-0x%x\n",
                cstRemoteWriter->cstReader->guid.oid,
                cstRemoteWriter->guid.hid,
                cstRemoteWriter->guid.aid);
  d->mbSend.cdrStream.bufferPtr+=len;
  d->mbSend.cdrStream.length+=len;
  eventDetach(d,
      cstRemoteWriter->objectEntryOID->objectEntryAID,
      &cstRemoteWriter->repeatActiveQueryTimer,
      1);   //metatraffic timer
  eventAdd(d,
      cstRemoteWriter->objectEntryOID->objectEntryAID,
      &cstRemoteWriter->repeatActiveQueryTimer,
      1,   //metatraffic timer
      "CSTReaderQueryTimer",
      CSTReaderQueryTimer,
      &cstRemoteWriter->cstReader->lock,
      cstRemoteWriter,
      &cstRemoteWriter->cstReader->params.repeatActiveQueryTime);
  return 0; 
}


/*****************************************************************************/
int
CSTReaderDeadlineTimer(ORTEDomain *d,void *vcstReader) {
  CSTReader            *cstReader=(CSTReader*)vcstReader;
  ORTESubsProp         *sp;
  ORTERecvInfo         info;  
  
  sp=(ORTESubsProp*)cstReader->objectEntryOID->attributes;
  memset(&info,0,sizeof(info));
  info.status=DEADLINE;
  info.topic=sp->topic;
  info.type=sp->typeName;
  cstReader->objectEntryOID->recvCallBack(&info,
      cstReader->objectEntryOID->instance,
      cstReader->objectEntryOID->callBackParam);
  eventDetach(d,
      cstReader->objectEntryOID->objectEntryAID,
      &cstReader->deadlineTimer,
      0);
  eventAdd(d,
      cstReader->objectEntryOID->objectEntryAID,
      &cstReader->deadlineTimer,
      0,   //common timer
      "CSTReaderDeadlineTimer",
      CSTReaderDeadlineTimer,
      &cstReader->lock,
      cstReader,
      &sp->deadline);
  return 0;
}

/*****************************************************************************/
int
CSTReaderPersistenceTimer(ORTEDomain *d,void *vcstReader) {
  CSTReader            *cstReader=(CSTReader*)vcstReader;
  CSTRemoteWriter      *cstRemoteWriter;
  CSChangeFromWriter   *csChangeFromWriter;
  ORTESubsProp         *sp;
  ORTEPublProp         *pp;
  int32_t            strength;
  
  if (cstReader->cstRemoteWriterSubscribed!=NULL) {
    //keep only one csChange (last)
    while (cstReader->cstRemoteWriterSubscribed->csChangesCounter>1) {
      csChangeFromWriter=
        CSChangeFromWriter_first(cstReader->cstRemoteWriterSubscribed);
      if (csChangeFromWriter) {  
        CSTReaderDestroyCSChangeFromWriter(
            cstReader->cstRemoteWriterSubscribed,
            csChangeFromWriter,
            ORTE_FALSE);
      }
    }
  }
  cstReader->cstRemoteWriterSubscribed=NULL;
  sp=(ORTESubsProp*)cstReader->objectEntryOID->attributes;
  strength=0;
  gavl_cust_for_each(CSTRemoteWriter,cstReader,cstRemoteWriter) {
    pp=(ORTEPublProp*)cstRemoteWriter->objectEntryOID->attributes;
    csChangeFromWriter=CSChangeFromWriter_last(cstRemoteWriter);
    if ((pp->strength>strength) && (csChangeFromWriter!=NULL)){
      NtpTime persistence,persistenceExpired,actTime;
      actTime=getActualNtpTime();
      NtpTimeAdd(persistenceExpired,
                 csChangeFromWriter->csChange->localTimeReceived,
                 pp->persistence);
      if (NtpTimeCmp(persistenceExpired,actTime)>0) {
        NtpTimeSub(persistence,
                   persistenceExpired,
                   actTime);
        eventDetach(d,
            cstReader->objectEntryOID->objectEntryAID,
            &cstReader->persistenceTimer,
            0);   //common timer
        eventAdd(d,
            cstReader->objectEntryOID->objectEntryAID,
            &cstReader->persistenceTimer,
            0,   //common timer
            "CSTReaderPersistenceTimer",
            CSTReaderPersistenceTimer,
            &cstReader->lock,
            cstReader,
            &persistence);
        cstReader->cstRemoteWriterSubscribed=cstRemoteWriter;
      }
    }
  }
  if ((cstReader->cstRemoteWriterSubscribed!=NULL) && 
      (sp->mode==IMMEDIATE)) {
    CSTReaderProcCSChangesIssue(
        cstReader->cstRemoteWriterSubscribed,ORTE_FALSE);
  }
  return 0;
}
