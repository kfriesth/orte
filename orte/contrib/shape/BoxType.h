/*
 * This file was generated by orte-idl - DO NOT EDIT!
 */

#ifndef BoxType_H
#define BoxType_H 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef EXCLUDE_ORTE_H
#include <orte.h>
#endif /* EXCLUDE_ORTE_H */

/** typedefs **/
#ifndef CL_BLUE
#define CL_BLUE 0
#endif /* !CL_BLUE */

#ifndef CL_GREEN
#define CL_GREEN 1
#endif /* !CL_GREEN */

#ifndef CL_RED
#define CL_RED 2
#endif /* !CL_RED */

#ifndef CL_BLACK
#define CL_BLACK 3
#endif /* !CL_BLACK */

#ifndef CL_YELLOW
#define CL_YELLOW 4
#endif /* !CL_YELLOW */

#if !defined(_BoxRect_defined)
#define _BoxRect_defined 1
typedef struct BoxRect_type BoxRect;
struct BoxRect_type {
CORBA_short top_left_x;
CORBA_short top_left_y;
CORBA_short bottom_right_x;
CORBA_short bottom_right_y;
};

#endif
#if !defined(_BoxType_defined)
#define _BoxType_defined 1
typedef struct BoxType_type BoxType;
struct BoxType_type {
CORBA_octet color;
CORBA_long shape;
BoxRect rectangle;
};

#endif

/** impls declarations **/
void BoxRect_serialize(CDR_Codec *cdrCodec,BoxRect *object);
void BoxRect_deserialize(CDR_Codec *cdrCodec,BoxRect *object);
int BoxRect_get_max_size(ORTEGetMaxSizeParam *gms);
Boolean BoxRect_type_register(ORTEDomain *d);

void BoxType_serialize(CDR_Codec *cdrCodec,BoxType *object);
void BoxType_deserialize(CDR_Codec *cdrCodec,BoxType *object);
int BoxType_get_max_size(ORTEGetMaxSizeParam *gms);
Boolean BoxType_type_register(ORTEDomain *d);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
