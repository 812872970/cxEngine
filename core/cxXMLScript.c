//
//  cxXMLScript.c
//  cxEngine
//
//  Created by xuhua on 10/14/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxXMLScript.h"
#include "cxHash.h"
#include "cxEventItem.h"
#include "cxEngine.h"

CX_OBJECT_INIT(cxXMLScript, cxObject)
{
    
}
CX_OBJECT_FREE(cxXMLScript, cxObject)
{
    CX_RELEASE(this->bytes);
}
CX_OBJECT_TERM(cxXMLScript, cxObject)

cxChar *cxXMLAttrAuto(xmlTextReaderPtr reader,cxConstChars name)
{
    xmlChar *txt = xmlTextReaderGetAttribute(reader, BAD_CAST(name));
    if(txt != NULL && xmlStrlen(txt) > 0){
        return (cxChar *)txt;
    }
    xmlFree(txt);
    return NULL;
}

xmlTextReaderPtr cxXMLReaderForString(cxString code)
{
    return xmlReaderForMemory(cxStringBody(code), cxStringLength(code), NULL, "UTF-8", 0);
}

xmlTextReaderPtr cxXMLReaderForScript(cxXMLScript this)
{
    return cxXMLReaderForString(this->bytes);
}

//method({json}
cxEventItem cxXMLReadEvent(cxHash events,cxConstChars name,xmlTextReaderPtr reader)
{
    //xml view event event(arg)
    cxChar *eventName = cxXMLAttr(name);
    CX_RETURN(eventName == NULL,NULL);
    cxConstChars ptr = eventName;
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt lpos = 0;
    cxInt rpos = 0;
    for(int i=0; i < len; i++){
        if(ptr[i] == '('){
            lpos = i;
        }
        if(ptr[i] == ')'){
            rpos = i;
        }
    }
    //parse simple json object arg
    cxBool hasArgs = (lpos > 0 && rpos > 0);
    cxChar eName[len];
    cxChar eArg[len];
    if(hasArgs){
        int len = rpos - lpos - 1;
        memcpy(eArg, ptr + lpos + 1, len);
        eArg[len] = '\0';
        //char ' -> "
        for(int i=0; i < len;i++){
            if(eArg[i] == '\''){
                eArg[i] = '\"';
            }
        }
        //copy name
        len = lpos;
        memcpy(eName, ptr, len);
        eName[len] = '\0';
    }else{
        memcpy(eName, ptr, len);
    }
    //get event item or from global events
    cxEventItem event = cxHashGet(events, cxHashStrKey(eName));
    if(event == NULL){
        event = cxEngineGetEvent(eName);
    }
    //set event arg
    if(event != NULL && hasArgs){
        CX_RETAIN_SWAP(event->arg, cxEventArgCreate(eArg));
    }
    xmlFree(eventName);
    return event;
}

cxVec2f cxXMLReadVec2fAttr(xmlTextReaderPtr reader,cxConstChars name,cxVec2f value)
{
    cxXMLReadFloatsAttr(reader, name, &value.x);
    return value;
}

cxFloat cxXMLParseFloat(const cxChar *txt,cxFloat value)
{
    cxEngine engine = cxEngineInstance();
    CX_RETURN(txt == NULL, value);
    cxChar *svalue = allocator->strdup(txt);
    cxInt len = strlen(svalue);
    if(svalue[len - 1] == 'w'){
        svalue[len - 1] = '\0';
        value = atof(svalue) * engine->winsize.w;
    }else if(svalue[len -1] == 'h'){
        svalue[len - 1] = '\0';
        value = atof(svalue) * engine->winsize.h;
    }else{
        value = atof(svalue);
    }
    allocator->free(svalue);
    return value;
}

cxFloat cxXMLReadFloatAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat value)
{
    cxChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL, value);
    value = cxXMLParseFloat(svalue, value);
    xmlFree(svalue);
    return value;
}

cxInt cxXMLReadFloatsAttr(xmlTextReaderPtr reader,cxConstChars name,cxFloat *values)
{
    cxChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL,0);
    cxInt c = cxReadFloats(svalue, values);
    xmlFree(svalue);
    return c;
}

cxInt cxReadFloats(cxConstChars ptr,cxFloat *values)
{
    CX_RETURN(ptr == NULL || values == NULL, 0);
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt argLen = 0;
    cxChar arg[16]={0};
    cxInt c = 0;
    CX_RETURN(len < 2, 0);
    for(int i=0; i < len; i++){
        if(ptr[i] == ',' || ptr[i] == '\0'){
            arg[argLen] = '\0';
            cxFloat ov = values[c];
            (*values++) = cxXMLParseFloat(arg,ov);
            c++;
            argLen = 0;
        }else{
            arg[argLen++] = ptr[i];
        }
    }
    return c;
}

cxInt cxReadInts(cxConstChars ptr,cxInt *values)
{
    CX_RETURN(ptr == NULL || values == NULL, 0);
    cxInt len = (cxInt)strlen(ptr) + 1;
    cxInt argLen = 0;
    cxChar arg[16]={0};
    cxInt c = 0;
    CX_RETURN(len < 2, 0);
    for(int i=0; i < len; i++){
        if(ptr[i] == ',' || ptr[i] == '\0'){
            arg[argLen] = '\0';
            c++;
            (*values++) = atoi(arg);
            argLen = 0;
        }else{
            arg[argLen++] = ptr[i];
        }
    }
    return c;
}

cxInt cxXMLReadIntsAttr(xmlTextReaderPtr reader,cxConstChars name,cxInt *values)
{
    cxChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL,0);
    cxInt c = cxReadInts(svalue, values);
    xmlFree(svalue);
    return c;
}

//r,g,b,a
cxColor4f cxXMLReadColorAttr(xmlTextReaderPtr reader,cxConstChars name,cxColor4f color)
{
    cxXMLReadFloatsAttr(reader, name, &color.r);
    return color;
}

cxChar *cxXMLReadString(xmlTextReaderPtr reader)
{
    return (cxChar *)xmlTextReaderReadString(reader);
}

cxInt cxXMLReadIntAttr(xmlTextReaderPtr reader,cxConstChars name,cxInt value)
{
    cxChar *svalue = cxXMLAttr(name);
    if(svalue != NULL){
        value = atoi(svalue);
    }
    xmlFree(svalue);
    return value;
}

cxString cxXMLReadLangStringAttr(xmlTextReaderPtr reader,cxConstChars name)
{
    cxString rv = NULL;
    cxChar *value = cxXMLAttr(name);
    CX_RETURN(value == NULL, NULL);
    cxChar src[128]={0};
    cxChar key[128]={0};
    if(cxParseURL(value, src, key) == 2){
        rv = cxEngineTypesText(src, key);
    }else{
        rv = cxStringConstChars(value);
    }
    xmlFree(value);
    return rv;
}

cxString cxXMLReaderReadOuterXml(xmlTextReaderPtr reader)
{
    xmlChar *xml = xmlTextReaderReadOuterXml(reader);
    cxString rv = NULL;
    if(xml != NULL){
        rv = cxStringConstChars((cxConstChars)xml);
    }
    xmlFree(xml);
    return rv;
}

cxBool cxXMLReadBoolAttr(xmlTextReaderPtr reader,cxConstChars name,cxBool value)
{
    cxBool ret = value;
    cxChar *svalue = cxXMLAttr(name);
    CX_RETURN(svalue == NULL,ret);
    if(cxConstCharsEqu(svalue, "true")){
        ret = true;
    }else if(cxConstCharsEqu(svalue, "false")){
        ret = false;
    }else{
        CX_ERROR("bool value is true or false");
    }
    xmlFree(svalue);
    return ret;
}








