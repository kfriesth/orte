/* JORTEPublicationGetStatus.c */

/**
  * This code provides conversion between JAVA a C environments.
  * The C functions are calling here and results are send to JAVA
  * native functions. It uses the header pregenerated by JAVA
  * (by command 'javah -jni class_with_native_function')
  *
  * @author Lukas Pokorny (lukas_pokorny@centrum.cz)
  * @author CTU FEE Prague - Department of Control Engineering (dce.felk.cvut.cz)
  * @author Project ORTE - OCERA Real Time Ethernet (www.ocera.org)
  * @author dedication to Kj
  * @version 0.1
  *
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  * GNU General Public License for more details.
  *
  */

#include <string.h>
#include <stdlib.h>
// library header file's path
#include "orte.h"
// enable TEST_STAGE run level
#include "jorte/4all.h"
// pregenerated header
#include "jorte/org_ocera_orte_Publication.h"


JNIEXPORT jobject JNICALL
Java_org_ocera_orte_Publication_jORTEPublicationGetStatus
(JNIEnv *env, jobject obj, jint j_appDomain_handle)
{
  // jni types
  jclass           cls_ps = NULL; // class of PubStatus
  jobject          obj_ps = NULL; // instance of PubStatus
  jfieldID         fid    = NULL;
  jmethodID        mid    = NULL;
  //
  int              b = 0; // boolean
  int              flag_ok = 0;
  ORTEPublStatus   ps;
  ORTEPublStatus  *publ_stat = &ps;

  #ifdef TEST_STAGE
    printf(":c: ORTEPublicationGetStatus() called.. \n");
  #endif

  do
  {
    // call ORTE function
    b = ORTEPublicationGetStatus((ORTEPublication *)j_appDomain_handle, publ_stat);
    if(b == ORTE_BAD_HANDLE)
    {
     printf(":!c: publicationGetStatus() failed! [bad pub handle] \n");
     break;
    }
    // find cls
    cls_ps = (*env)->FindClass(env,"org/ocera/orte/types/Status");
    if(cls_ps == 0)
    {
      #ifdef TEST_STAGE
       printf(":!c: cls_ps = NULL \n");
      #endif
      break;
    }
    // call object constructor
    mid = (*env)->GetMethodID(env,cls_ps,"<init>","()V");
    if(mid == 0)
    {
      #ifdef TEST_STAGE
       printf(":!c: mid = NULL \n");
      #endif
      break;
    }
    // new object
    obj_ps = (*env)->NewObject(env, cls_ps, mid);
    if(obj_ps == 0)
    {
      #ifdef TEST_STAGE
       printf(":!c: obj_ps = NULL \n");
      #endif
      break;
    }
    // ///////////////////////////////////////////////
    // setting object's fields
    // fieldID - strict
    fid = (*env)->GetFieldID(env,
                             cls_ps,
                             "strict",
                             "J");
    if(fid == NULL)
    {
      #ifdef TEST_STAGE
       printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    (*env)->SetLongField(env,
                         obj_ps,
                         fid,
                         (jlong) publ_stat->strict);
    // fieldID - bestEffort
    fid = (*env)->GetFieldID(env,
                             cls_ps,
                             "bestEffort",
                             "J");
    if(fid == NULL)
    {
      #ifdef TEST_STAGE
       printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    (*env)->SetLongField(env,
                         obj_ps,
                         fid,
                         (jlong) publ_stat->bestEffort);
    // fieldID - issues
    fid = (*env)->GetFieldID(env,
                             cls_ps,
                             "issues",
                             "J");
    if(fid == NULL)
    {
      #ifdef TEST_STAGE
        printf(":!c: fid = NULL \n");
      #endif
      break;
    }
    (*env)->SetLongField(env,
                         obj_ps,
                         fid,
                         (jlong) publ_stat->issues);
    // set flag
    flag_ok = 1;

  } while(0);

  if(flag_ok == 0)
  {
    return NULL;
  }
  return obj_ps;

}
