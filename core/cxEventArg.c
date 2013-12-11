//
//  cxEventArg.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxEventArg.h"

CX_OBJECT_INIT(cxEventArg, cxObject)
{
    
}
CX_OBJECT_FREE(cxEventArg, cxObject)
{
    this->weakRef = NULL;
    if(this->strongRef != NULL){
        CX_RELEASE(this->strongRef);
    }
    if(this->json != NULL){
        json_decref(this->json);
    }
    CX_RELEASE(this->number);
}
CX_OBJECT_TERM(cxEventArg, cxObject)

cxAny cxEventArgToWeakRef(cxEventArg this)
{
    CX_RETURN(this == NULL,NULL);
    return this->weakRef;
}

cxEventArg cxEventArgWeakRef(cxAny weakRef)
{
    cxEventArg this = CX_CREATE(cxEventArg);
    this->weakRef = weakRef;
    return this;
}

cxAny cxEventArgToStrongRef(cxEventArg this)
{
    CX_RETURN(this == NULL,NULL);
    return this->strongRef;
}

cxEventArg cxEventArgStrongRef(cxAny strongRef)
{
    cxEventArg this = CX_CREATE(cxEventArg);
    CX_RETAIN_SWAP(this->strongRef, strongRef);
    return this;
}

cxEventArg cxEventArgCreateWithNumber(cxConstChars json,cxNumber number)
{
    cxEventArg this = cxEventArgCreate(json);
    cxEventArgSetNumber(this, number);
    return this;
}

cxEventArg cxEventArgCreate(cxConstChars json)
{
    CX_RETURN(json == NULL,NULL);
    cxEventArg this = CX_CREATE(cxEventArg);
    json_error_t error = {0};
    this->json = json_loadb(json, strlen(json), JSON_DECODE_ANY, &error);
    if(this->json == NULL){
        CX_ERROR("event json arg decode error (%d:%d) %s:%s",error.line,error.column,error.source,error.text);
        return NULL;
    }
    return this;
}

void cxEventArgSetNumber(cxEventArg this,cxNumber num)
{
    CX_RETAIN_SWAP(this->number, num);
}

cxEventArg cxEventArgNumber(cxNumber number)
{
    cxEventArg this = CX_CREATE(cxEventArg);
    CX_RETAIN_SWAP(this->number, number);
    return this;
}

cxBool cxEventArgToBool(cxEventArg this,cxBool dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    if(!json_is_integer(this->json)){
        return dv;
    }
    return (this->json->type == JSON_TRUE);
}

cxBool cxEventArgBool(cxEventArg this,cxConstChars key,cxBool dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    json_t *v = json_object_get(this->json, key);
    if(v == NULL){
        return dv;
    }
    CX_ASSERT(json_is_boolean(v), "key %s not bool type",key);
    return (v->type == JSON_TRUE);
}

cxInt cxEventArgToInt(cxEventArg this,cxInt dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    if(!json_is_integer(this->json)){
        return dv;
    }
    return (cxInt)json_integer_value(this->json);
}

cxInt cxEventArgInt(cxEventArg this,cxConstChars key,cxInt dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    json_t *v = json_object_get(this->json, key);
    if(v == NULL){
        return dv;
    }
    CX_ASSERT(json_is_integer(v), "key %s not int type",key);
    return (cxInt)json_integer_value(v);
}

cxDouble cxEventArgToDouble(cxEventArg this,cxDouble dv)
{
    CX_ASSERT(this->json != NULL, "args error");
    if(!json_is_real(this->json)){
        return dv;
    }
    return json_real_value(this->json);
}

cxDouble cxEventArgDouble(cxEventArg this,cxConstChars key,cxDouble dv)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    json_t *v = json_object_get(this->json, key);
    if(v == NULL){
        return dv;
    }
    CX_ASSERT(json_is_number(v), "key %s not real type",key);
    return json_real_value(v);
}

cxConstChars cxEventArgToString(cxEventArg this)
{
    CX_ASSERT(this->json != NULL, "args error");
    if(!json_is_string(this->json)){
        return NULL;
    }
    return json_string_value(this->json);
}

cxConstChars cxEventArgString(cxEventArg this,cxConstChars key)
{
    CX_ASSERT(this->json != NULL && key != NULL, "args error");
    json_t *v = json_object_get(this->json, key);
    if(v == NULL){
        return NULL;
    }
    CX_ASSERT(json_is_string(v), "key %s not string type",key);
    return json_string_value(v);
}







