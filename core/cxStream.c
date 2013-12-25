//
//  cxStream.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <libgen.h>
#include "cxStream.h"

const luaL_Reg cxStreamInstanceMethods[] = {
    
    CX_LUA_SUPER(cxObject)
};

const luaL_Reg cxStreamTypeMethods[] = {
    CX_LUA_TYPE(cxStream)
};

void cxStreamTypeInit()
{
    CX_LUA_LOAD_TYPE(cxStream);
}

CX_OBJECT_INIT(cxStream, cxObject)
{
}
CX_OBJECT_FREE(cxStream, cxObject)
{
    if(this->isOpen){
        cxStreamClose(this);
    }
    CX_RELEASE(this->path);
    CX_RELEASE(this->file);
    CX_METHOD_RELEASE(this->Read);
    CX_METHOD_RELEASE(this->Write);
    CX_METHOD_RELEASE(this->Open);
    CX_METHOD_RELEASE(this->Seek);
    CX_METHOD_RELEASE(this->AllBytes);
    CX_METHOD_RELEASE(this->Close);
    CX_METHOD_RELEASE(this->Position);
}
CX_OBJECT_TERM(cxStream, cxObject)

cxBool cxStreamOpen(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_RUN(false, stream->Open, CX_METHOD_TYPE(cxBool,cxAny), this);
}

cxInt cxStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    cxStream stream = this;
    return CX_METHOD_RUN(0, stream->Read, CX_METHOD_TYPE(cxInt,cxAny,cxPointer,cxInt),this,buffer,size);
}

cxInt cxStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    cxStream stream = this;
    return CX_METHOD_RUN(0, stream->Write, CX_METHOD_TYPE(cxInt,cxAny,cxPointer,cxInt),this,buffer,size);
}

cxBool cxStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxStream stream = this;
    return CX_METHOD_RUN(false, stream->Seek, CX_METHOD_TYPE(cxBool,cxAny,cxOff,cxInt),this,off,flags);
}

cxOff cxStreamPosition(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_RUN(0, stream->Position, CX_METHOD_TYPE(cxOff,cxAny), this);
}

cxInt cxStreamLength(cxAny this)
{
    cxStream stream = this;
    return stream->length;
}

void cxStreamClose(cxAny this)
{
    cxStream stream = this;
    CX_METHOD_RUN(NULL, stream->Close, CX_METHOD_TYPE(void,cxAny),this);
}

cxString cxStreamAllBytes(cxAny this)
{
    cxStream stream = this;
    return CX_METHOD_RUN(NULL, stream->AllBytes, CX_METHOD_TYPE(cxString,cxAny),this);
}

void cxStreamBaseClose(cxAny this)
{
    cxStream stream = this;
    stream->length = 0;
    stream->canRead = false;
    stream->canWrite = false;
    stream->canSeek = false;
    stream->isOpen = false;
}

cxString cxStreamFileDir(cxAny this)
{
    cxStream stream = this;
    if(stream->path == NULL){
        return cxStringCreate(".");
    }
    char *dir = dirname((char *)cxStringBody(stream->file));
    return cxStringCreate("%s",dir);
}












