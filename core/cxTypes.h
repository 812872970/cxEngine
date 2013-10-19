//
//  cxTypes.h
//  cxEngine
//
//  Created by xuhua on 9/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxTypes_h
#define cxEngine_cxTypes_h

#include <kazmath/kazmath.h>
#include "cxBase.h"

#define cxFloatEqu kmAlmostEqual

typedef struct {
    cxInt x;
    cxInt y;
}cxVec2i;
#define cxVec2iv(x,y)   (cxVec2i){x,y}

typedef struct {
    cxInt x;
    cxInt y;
    cxInt z;
}cxVec3i;
#define cxVec3iv(x,y,z)   (cxVec3i){x,y,z}

typedef kmMat4 cxMatrix4f;

typedef kmVec2 cxVec2f;
#define cxVec2fv(x,y)           (cxVec2f){x,y}
#define cxVec2fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y))
#define cxVec2fAngle(a)         atan2f((a).y, (a).x)

typedef kmVec3 cxVec3f;
#define cxVec3fv(x,y,z)         (cxVec3f){x,y,z}
#define cxVec3fEqu(p1,p2)       (cxFloatEqu((p1).x,(p2).x) && cxFloatEqu((p1).y,(p2).y) && cxFloatEqu((p1).z,(p2).z))

typedef struct {
    kmScalar r;
    kmScalar g;
    kmScalar b;
    kmScalar a;
} cxColor4f;
#define cxColor4fv(r,g,b,a)     (cxColor4f){r,g,b,a}
#define cxColor4fEqu(c1,c2)     (cxFloatEqu((c1).r,(c2).r)&&cxFloatEqu((c1).g,(c2).g)&&cxFloatEqu((c1).b,(c2).b)&&cxFloatEqu((c1).a,(c2).a))

typedef struct {
    kmScalar r;
    kmScalar g;
    kmScalar b;
} cxColor3f;
#define cxColor3fv(r,g,b)       (cxColor3f){r,g,b}
#define cxColor3fEqu(c1,c2)     (cxFloatEqu((c1).r,(c2).r) && cxFloatEqu((c1).g,(c2).g) && cxFloatEqu((c1).b,(c2).b))

//colors define

static const cxColor3f cxWHITE   = cxColor3fv(1.00f, 1.00f, 1.00f);
static const cxColor3f cxYELLOW  = cxColor3fv(1.00f, 1.00f, 0.00f);
static const cxColor3f cxBLUE    = cxColor3fv(0.00f, 0.00f, 1.00f);
static const cxColor3f cxGREEN   = cxColor3fv(0.00f, 1.00f, 0.00f);
static const cxColor3f cxRED     = cxColor3fv(1.00f, 0.00f, 0.00f);
static const cxColor3f cxMAGENTA = cxColor3fv(1.00f, 0.00f, 1.00f);
static const cxColor3f cxBLACK   = cxColor3fv(0.00f, 0.00f, 0.00f);
static const cxColor3f cxORANGE  = cxColor3fv(1.00f, 0.50f, 0.00f);
static const cxColor3f cxGRAY    = cxColor3fv(0.65f, 0.65f, 0.65f);
static const cxColor3f cxPURPLE  = cxColor3fv(0.63f, 0.13f, 0.94f);

typedef struct {
    kmScalar w;
    kmScalar h;
} cxSize2f;
#define cxSize2fv(w,h)          (cxSize2f){w,h}
#define cxSize2fEqu(s1,s2)      (cxFloatEqu((s1).w,(s2).w) &&  cxFloatEqu((s1).h,(s2).h))

typedef struct {
    kmScalar u;
    kmScalar v;
} cxTexCoord2f;
#define cxTex2fv(u,v)   (cxTexCoord2f){u,v}

typedef struct {
    kmScalar l;
    kmScalar r;
    kmScalar t;
    kmScalar b;
} cxBox4f;
#define cxBox4fv(l,r,t,b) (cxBox4f){l,r,t,b}

typedef struct {
    kmScalar x;
    kmScalar y;
    kmScalar w;
    kmScalar h;
} cxRect4f;
#define cxRect4fv(x,y,w,h)  (cxRect4f){x,y,w,h}

typedef struct {
    cxVec2f lt;
    cxVec2f rt;
    cxVec2f rb;
    cxVec2f lb;
} cxBoxVec2f;

typedef struct {
    cxVec2f *vs;
    cxInt num;
}cxPolygon;

typedef struct {
    cxVec3f lt;
    cxVec3f lb;
    cxVec3f rt;
    cxVec3f rb;
} cxBoxVec3f;

typedef struct {
    cxVec3f vertices;
    cxColor4f colors;
    cxTexCoord2f texcoords;
} cxPoint;

typedef struct {
    cxPoint lt;
    cxPoint lb;
    cxPoint rt;
    cxPoint rb;
} cxBoxPoint;

typedef struct {
    cxPoint p1;
    cxPoint p2;
} cxLinePoint;

typedef struct {
    cxTexCoord2f lt;
    cxTexCoord2f lb;
    cxTexCoord2f rt;
    cxTexCoord2f rb;
} cxBoxTex2f;
#define cxBoxTex2fDefault() (cxBoxTex2f){cxTex2fv(0.0f, 0.0f),cxTex2fv(0.0f, 1.0f),cxTex2fv(1.0f, 0.0f),cxTex2fv(1.0f, 1.0f)}

cxRect4f cxBoxTex2fToRect4f(cxBoxTex2f box);

cxBoxTex2f cxRect4fToBoxTex2f(cxRect4f rect);

typedef struct {
    cxColor4f lt;
    cxColor4f lb;
    cxColor4f rt;
    cxColor4f rb;
} cxBoxColor4f;

CX_C_BEGIN

cxVec2f cxCardinalSplineAt(cxVec2f p0, cxVec2f p1, cxVec2f p2, cxVec2f p3, cxFloat tension, cxFloat t);

cxFloat cxBezier2(cxFloat a, cxFloat b, cxFloat c, cxFloat t);

cxFloat cxBezier3(cxFloat a, cxFloat b, cxFloat c, cxFloat d, cxFloat t);

cxBool cxBox2fContainPoint(const cxBoxVec2f box,const cxVec2f pos);

cxBool cxPolygonContainPoint(const cxPolygon *polygon,const cxVec2f tp);

CX_C_END

#endif
