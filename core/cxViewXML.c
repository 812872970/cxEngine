//
//  cxViewXML.c
//  cxEngine
//  use attr x y w h ax ay al ar at ab texture file
//  Created by xuhua on 10/12/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <ctype.h>
#include <core/cxEngine.h>
#include <core/cxStack.h>
#include <core/cxAutoPool.h>
#include <views/cxButton.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>
#include <views/cxLabel.h>
#include <views/cxClipping.h>
#include <views/cxTable.h>
#include "cxViewXML.h"
#include "cxHashXML.h"
#include "cxActionXML.h"

void cxViewRootXMLReadAttr(cxAny pxml,cxAny view, xmlTextReaderPtr reader)
{
    cxViewXMLReadAttr(pxml, view, reader);
    cxViewXML this = view;
    cxViewXML xml = pxml;
    cxXMLAppendEvent(xml->events, this, cxViewXML, onLoad);
}

void cxViewXMLRegisteEvent(cxAny pview,cxConstChars name,cxEventFunc func)
{
    cxViewXML this = pview;
    if(func == NULL){
        cxHashDel(this->events, cxHashStrKey(name));
    }else{
        cxEventItem event = cxHashGet(this->events, cxHashStrKey(name));
        CX_ASSERT(event == NULL, "name %s event registered",name);
        event = CX_ALLOC(cxEventItem);
        event->func = func;
        cxHashSet(this->events, cxHashStrKey(name), event);
        CX_RELEASE(event);
    }
}

CX_OBJECT_INIT(cxViewXML, cxView)
{
    cxObjectSetXMLReadFunc(this, cxViewRootXMLReadAttr);
    CX_METHOD_SET(this->Make, cxViewXMLMakeElement);
    this->items = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
}
CX_OBJECT_FREE(cxViewXML, cxView)
{
    CX_EVENT_RELEASE(this->onLoad);
    CX_RELEASE(this->items);
    CX_RELEASE(this->events);
    CX_RELEASE(this->actions);
}
CX_OBJECT_TERM(cxViewXML, cxView)

cxAny cxViewXMLGet(cxAny pview,cxConstChars key)
{
    cxAny root = cxObjectRoot(pview);
    CX_RETURN(root == NULL, NULL);
    cxViewXML xml = root;
    return cxHashGet(xml->items, cxHashStrKey(key));
}

void cxViewXMLRemove(cxAny pview,cxConstChars key)
{
    cxViewXML this = pview;
    cxAny view = cxHashGet(this->items, cxHashStrKey(key));
    CX_RETURN(view == NULL);
    cxViewRemoved(view);
    cxHashDel(this->items, cxHashStrKey(key));
}

void cxViewReplaceViewEvent(cxAny pview,cxAny arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxString url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxConstChars file = cxStringBody(url);
    cxViewXML view = cxViewXMLCreate(file);
    CX_ASSERT(view != NULL, "create xml view %s falied ",file);
    cxWindowReplaceView(view,arg);
}

void cxViewPushViewEvent(cxAny pview,cxAny arg)
{
    CX_ASSERT(arg != NULL, "args error");
    cxString url = cxEventArgToString(arg);
    CX_ASSERT(url != NULL, "args error");
    cxConstChars file = cxStringBody(url);
    cxViewXML view = cxViewXMLCreate(file);
    CX_ASSERT(view != NULL, "create xml view %s falied ",file);
    cxWindowPushView(view,arg);
}

void cxViewPopViewEvent(cxAny pview,cxAny arg)
{
    cxWindowPopView(arg);
}

cxViewXML cxViewXMLCreate(cxConstChars xml)
{
    cxViewXML this = CX_CREATE(cxViewXML);
    if(!cxViewXMLLoad(this, xml)){
        return NULL;
    }
    cxObjectSetRoot(this, this);
    return this;
}

static void cxViewXMLSet(cxAny pview,cxAny cview,xmlTextReaderPtr reader)
{
    cxViewXML this = pview;
    cxChar *id = cxXMLAttr("id");
    CX_RETURN(id == NULL);
    cxHashSet(this->items, cxHashStrKey(id), cview);
    xmlFree(id);
}

cxAny cxViewXMLMakeElement(const xmlChar *temp,xmlTextReaderPtr reader)
{
    cxView cview = NULL;
    if(ELEMENT_IS_TYPE(cxSprite)){
        cview = CX_CREATE(cxSprite);
    }else if(ELEMENT_IS_TYPE(cxViewXML)){
        cxChar *src = cxXMLAttr("src");
        cview = (cxView)cxViewXMLCreate(src);
        xmlFree(src);
    }else if(ELEMENT_IS_TYPE(cxView)){
        cview = CX_CREATE(cxView);
    }else if(ELEMENT_IS_TYPE(cxButton)){
        cview = CX_CREATE(cxButton);
    }else if(ELEMENT_IS_TYPE(cxAtlas)){
        cview = CX_CREATE(cxAtlas);
    }else if(ELEMENT_IS_TYPE(cxParticle)){
        cview = CX_CREATE(cxParticle);
    }else if(ELEMENT_IS_TYPE(cxLabel)){
        cview = CX_CREATE(cxLabel);
    }else if(ELEMENT_IS_TYPE(cxClipping)){
        cview = CX_CREATE(cxClipping);
    }else if(ELEMENT_IS_TYPE(cxTable)){
        cview = CX_CREATE(cxTable);
    }else{
        return NULL;
    }
    return cview;
}

static void cxViewXMLMakeActionElement(cxViewXML this,cxView pview,const xmlChar *temp,xmlTextReaderPtr reader)
{
    cxAny action = cxActionXMLMakeElement(temp, reader);
    CX_RETURN(action == NULL);
    cxChar *id = cxXMLAttr("id");
    //save and wait run
    if(id != NULL){
        cxHashSet(this->actions, cxHashStrKey(id), action);
    }else{
        cxViewAppendAction(pview, action);
    }
    xmlFree(id);
}

static void cxViewXMLLoadSubviews(cxAny pview,xmlTextReaderPtr reader,cxStack stack)
{
    cxViewXML this = pview;
    while(xmlTextReaderRead(reader)){
        int type = xmlTextReaderNodeType(reader);
        if(type == XML_READER_TYPE_ELEMENT){
            cxAny parent = cxStackTop(stack);
            if(parent == NULL){
                CX_ERROR("parse xml ui parent null");
                break;
            }
            //
            const xmlChar *temp = xmlTextReaderConstName(reader);
            if(temp == NULL){
                continue;
            }
            cxView cview = CX_METHOD_GET(NULL, this->Make, temp, reader);
            if(cview != NULL){
                cxViewAppend(parent, cview);
                //set root xmlview
                cxObjectSetRoot(cview, this);
                //read attr
                cxObjectXMLReadRun(cview, this, cview, reader);
                //save to hash
                cxViewXMLSet(this, cview, reader);
            }else{
                cxViewXMLMakeActionElement(this, parent, temp, reader);
            }
            if(xmlTextReaderIsEmptyElement(reader)){
                continue;
            }
            cxStackPush(stack, cview);
        }else if(type == XML_READER_TYPE_END_ELEMENT){
            cxStackPop(stack);
        }
    }
}

static void cxViewXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxViewXML this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

cxBool cxViewXMLLoadWithReader(cxAny pview,xmlTextReaderPtr reader)
{
    cxBool ret = false;
    cxViewXML xmlView = pview;
    cxStack stack = CX_ALLOC(cxStack);
    while(xmlTextReaderRead(reader)){
        if(xmlView->isError){
            break;
        }
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(cxViewXML)){
            ret = true;
            break;
        }
    }
    if(ret){
        cxAutoPoolPush();
        cxObjectXMLReadRun(xmlView, xmlView, xmlView, reader);
        cxStackPush(stack, xmlView);
        CX_EVENT_FIRE(xmlView, onLoad);
        
        cxViewXMLLoadSubviews(xmlView,reader, stack);
        
        cxStackPop(stack);
        cxAutoPoolPop();
    }
    CX_RELEASE(stack);
    return ret;
}

cxBool cxViewXMLLoad(cxAny pview,cxConstChars xml)
{
    cxXMLScript script = cxEngineGetXMLScript(xml);
    if(script == NULL){
        CX_ERROR("%s script not register",xml);
        return false;
    }
    if(script->bytes == NULL){
        CX_ERROR("xml script not load bytes");
        return false;
    }
    cxBool ret = false;
    xmlTextReaderPtr reader = xmlReaderForMemory(cxStringBody(script->bytes), cxStringLength(script->bytes), NULL, "UTF-8", 0);
    if(reader == NULL){
        CX_ERROR("create xml reader failed");
        return false;
    }
    xmlTextReaderSetErrorHandler(reader, cxViewXMLReaderError, pview);
    ret = cxViewXMLLoadWithReader(pview, reader);
    xmlFreeTextReader(reader);
    return ret;
}











