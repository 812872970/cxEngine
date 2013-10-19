//
//  cxAtlas.h
//  cxEngine
//
//  Created by xuhua on 10/17/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxAtlas_h
#define cxEngine_cxAtlas_h

#include "cxSprite.h"

CX_C_BEGIN

typedef struct {
    cxBool  enable;
    cxBox4f box;
}cxAtlasScale9;

CX_OBJECT_DEF(cxAtlas, cxSprite)
    GLuint vaoid;
    GLuint vboid[2];
    cxInt boxNumber;
    cxInt number;
    cxBoxPoint *boxs;
    GLushort *indices;
    cxBool isDirty;
    cxBool isInit;
    cxAtlasScale9 scale9;
CX_OBJECT_END(cxAtlas)

void cxAtlasDraw(void *pview);

void cxAtlasDrawInit(cxAny pview);

void cxAtlasResize(cxAny pview,cxAny arg);

cxBoxPoint cxAtlasXMLReadBoxPoint(cxAny pview,xmlTextReaderPtr reader);

cxBoxPoint cxAtlasCreateBoxPoint(cxVec2f pos,cxSize2f size,cxBoxTex2f tex,cxColor4f color);

void cxAtlasSetNumber(cxAny pview,cxInt boxNumber);

void cxAtlasSetScale9(cxAny pview,cxBox4f box);

void cxAtlasUpdateScale9(cxAny pview);

void cxAtlasClean(cxAny pview);

void cxAtlasAppend(cxAny pview,cxBoxPoint point);

void cxAtlasUpdate(cxAny pview,cxInt index, cxBoxPoint point);

void cxAtlasXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

CX_C_END

#endif
