//
//  cxViewRoot.h
//  cxEngine
//
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxViewRoot_h
#define cxEngine_cxViewRoot_h

#include <core/cxView.h>
#include <core/cxEventItem.h>
#include <core/cxEventArg.h>
#include <streams/cxAssetsStream.h>
#include <textures/cxTextureFactory.h>
#include <views/cxSprite.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxViewRoot, cxView)
    cxBool isError;
    cxHash items;
    cxHash events;
    cxHash functions;
    cxHash actions;
    CX_METHOD_DEF(cxXMLScriptMakeElementFunc, Make);
    CX_EVENT_ALLOC(onBegin);
    CX_EVENT_ALLOC(onEnd);
CX_OBJECT_END(cxViewRoot)

cxAny cxViewRootLoadSubviewBegin(cxViewRoot root,cxConstChars temp,xmlTextReaderPtr reader);

void cxViewRootSet(cxAny pview,cxAny cview,xmlTextReaderPtr reader);

void cxViewRootSetItem(cxAny pview,cxConstChars key,cxAny item);

cxAny cxViewRootMakeElement(cxConstChars temp,xmlTextReaderPtr reader);

void cxViewRootReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader);

void cxViewRootRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func);

void cxViewRootRegisteFunc(cxAny pview,cxConstChars name,cxAny func);

cxViewRoot cxViewRootCreate(cxConstChars xml);

void cxViewRootRemove(cxAny pview,cxConstChars key);

cxAny cxViewRootGet(cxAny pview,cxConstChars key);

void cxViewReplaceViewEvent(cxEvent *event);

void cxViewPushViewEvent(cxEvent *event);

void cxViewPopViewEvent(cxEvent *event);

cxBool cxViewRootLoad(cxAny pview,cxConstChars xml);

cxBool cxViewRootLoadWithReader(cxAny pview,xmlTextReaderPtr reader);

CX_C_END

#endif







