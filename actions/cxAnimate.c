//
//  cxAnimate.c
//  cxEngine
//
//  Created by xuhua on 11/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <views/cxSprite.h>
#include <textures/cxTextureFactory.h>
#include <core/cxTexture.h>
#include <core/cxActionRoot.h>
#include <core/cxUtil.h>
#include <core/cxUrlPath.h>
#include "cxAnimate.h"

CX_OBJECT_DEF(cxAnimateItem, cxObject)
    cxTexture texture;
    cxString key;
    cxFloat delay;
    cxFloat time;
CX_OBJECT_END(cxAnimateItem)

CX_OBJECT_INIT(cxAnimateItem, cxObject)
{
}
CX_OBJECT_FREE(cxAnimateItem, cxObject)
{
    CX_RELEASE(this->key);
    CX_RELEASE(this->texture);
}
CX_OBJECT_TERM(cxAnimateItem, cxObject)

void cxAnimateItemAppend(cxArray list,cxConstChars file,cxConstChars key,cxFloat delay,cxBool cache)
{
    cxAnimateItem this = CX_CREATE(cxAnimateItem);
    if(file != NULL){
        this->texture = cache ? cxTextureFactoryLoadFile(file) : cxTextureCreate(file);
        CX_ASSERT(this->texture != NULL, "%s load failed", file);
        CX_RETAIN(this->texture);
    }
    if(key != NULL){
        this->key = cxStringAllocChars(key);
    }
    this->delay = delay;
    cxArrayAppend(list, this);
}

static void cxAnimateInit(cxAny pav)
{
    cxAnimate this = pav;
    this->index = 0;
    this->super.duration = this->duration;
    cxFloat dt = this->duration / (cxFloat)cxArrayLength(this->list);
    cxFloat i = 0;
    CX_ARRAY_FOREACH(this->list, e){
        cxAnimateItem item = cxArrayObject(e);
        this->super.duration += item->delay;
        i += dt  + item->delay;
        item->time = i;
    }
}

static void cxAnimateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxAnimate this = pav;
    cxInt i = 0;
    CX_RETURN(this->index >= cxArrayLength(this->list));
    CX_ARRAY_FOREACH(this->list, e){
        i++;
        cxAnimateItem item = cxArrayObject(e);
        if(this->super.durationElapsed > item->time){
            continue;
        }
        if(this->index == i){
            break;
        }
        if(item->texture != NULL){
            cxSpriteSetTexture(this->super.view, item->texture);
        }
        if(item->texture != NULL && item->key != NULL){
            cxSpriteSetTextureKey(this->super.view, cxStringBody(item->key), false);
        }
        this->index = i;
        CX_EVENT_FIRE(this, onFrame);
        break;
    }
}

static void cxAnimateXMLAppend(cxArray list,cxConstChars file,cxConstChars key,cxInt from,cxInt to,cxBool cache)
{
    for(cxInt i = from; i <= to ; i++){
        cxConstChars sfile = CX_CONST_STRING(file,i);
        cxConstChars skey = (key != NULL) ? CX_CONST_STRING(key,i) : NULL;
        cxAnimateItemAppend(list, sfile, skey, 0, cache);
    }
}

static void cxAnimateReadAttr(cxAny rootAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionRoot root = rootAction;
    cxAnimate this = mAction;
    cxActionReadAttr(rootAction, mAction, reader);
    int depth = xmlTextReaderDepth(reader);
    while(xmlTextReaderRead(reader) && depth != xmlTextReaderDepth(reader)){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        cxConstChars temp = cxXMLReadElementName(reader);
        if(!ELEMENT_IS_TYPE(cxFrame)){
            continue;
        }
        cxBool cache = cxXMLReadBoolAttr(reader, root->functions, "cache", false);
        cxConstChars file = cxXMLAttr("file");
        cxConstChars key = cxXMLAttr("key");
        cxInt from = cxXMLReadIntAttr(reader, root->functions, "from", 0);
        cxInt to = cxXMLReadIntAttr(reader, root->functions, "to", 0);
        if(from > 0 && to > 0){
            cxAnimateXMLAppend(this->list, file, key, from, to, cache);
        }else if(file != NULL){
            cxUrlPath path = cxUrlPathParse(file);
            cxFloat delay = cxXMLReadFloatAttr(reader, root->functions, "delay", 0);
            cxAnimateItemAppend(this->list, path->path, path->count >= 2 ? path->key : NULL, delay, cache);
        }
    }
    this->duration = this->super.duration;
    cxXMLAppendEvent(root->events, this, cxAnimate, onFrame);
}

static void cxAnimateReset(cxAny pav)
{
    cxAnimate this = pav;
    CX_EVENT_RELEASE(this->onFrame);
}

CX_OBJECT_INIT(cxAnimate, cxAction)
{
    cxObjectSetReadAttrFunc(this, cxAnimateReadAttr);
    CX_METHOD_SET(this->super.Init, cxAnimateInit);
    CX_METHOD_SET(this->super.Step, cxAnimateStep);
    CX_METHOD_SET(this->super.Reset, cxAnimateReset);
    this->list = CX_ALLOC(cxArray);
}
CX_OBJECT_FREE(cxAnimate, cxAction)
{
    CX_EVENT_RELEASE(this->onFrame);
    CX_RELEASE(this->list);
}
CX_OBJECT_TERM(cxAnimate, cxAction)

cxAnimate cxAnimateCreate(cxFloat duration,cxArray list)
{
    cxAnimate this = CX_CREATE(cxAnimate);
    CX_RETAIN_SWAP(this->list, list);
    this->duration = duration;
    return this;
}










