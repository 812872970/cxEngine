//
//  cxAssetsStream.c
//  cxEngine
//
//  Created by xuhua on 9/27/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <unistd.h>
#include <sys/stat.h>
#include <core/cxUtil.h>
#include "cxAssetsStream.h"

static cxBool cxAssetsStreamOpen(cxAny this)
{
    cxAssetsStream asserts = this;
    CX_ASSERT(asserts->super.isOpen == false,"stream repeat open");
    cxConstChars path = cxStringBody(asserts->super.path);
    asserts->fd = cxAssertsOpen(path, &asserts->start, &asserts->super.length);
    if(asserts->fd < 0){
        CX_ERROR("open assets %s stream failed",path);
        return false;
    }
    lseek(asserts->fd, asserts->start, SEEK_SET);
    asserts->super.canRead = true;
    asserts->super.canSeek = true;
    asserts->super.canWrite = false;
    asserts->super.isOpen = true;
    return true;
}

static cxInt cxAssetsStreamRead(cxAny this,cxPointer buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canRead){
        return 0;
    }
    return read(asserts->fd, buffer, size);
}

static cxInt cxAssetsStreamWrite(cxAny this,cxPointer buffer,cxInt size)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canWrite){
        return 0;
    }
    return 0;
}

static cxOff cxAssetsStreamPosition(cxAny this)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canRead){
        return 0;
    }
    return (cxOff)lseek(asserts->fd, 0, SEEK_CUR);
}

static cxBool cxAssetsStreamSeek(cxAny this,cxOff off,cxInt flags)
{
    cxAssetsStream asserts = this;
    if(!asserts->super.canSeek){
        return false;
    }
    return lseek(asserts->fd, off + asserts->start, flags) > 0;
}

static cxString cxAssetsStreamAllBytes(cxAny this)
{
    cxStream asserts = this;
    if(!asserts->canRead){
        cxStreamOpen(asserts);
    }
    if(!asserts->canRead){
        CX_ERROR("file stream can't read");
        return NULL;
    }
    cxStreamSeek(asserts,0,SEEK_SET);
    cxChar *bytes = allocator->malloc(asserts->length + 1);
    cxStreamRead(asserts,bytes,asserts->length);
    bytes[asserts->length] = '\0';
    cxString data = cxStringAttach(bytes, asserts->length);
    cxStreamClose(asserts);
    return data;
}

static void cxAssetsStreamClose(cxAny this)
{
    cxAssetsStream asserts = this;
    close(asserts->fd);
    cxStreamBaseClose(this);
}

CX_OBJECT_INIT(cxAssetsStream, cxStream)
{
    CX_METHOD_SET(this->super.Open, cxAssetsStreamOpen);
    CX_METHOD_SET(this->super.Read, cxAssetsStreamRead);
    CX_METHOD_SET(this->super.Write, cxAssetsStreamWrite);
    CX_METHOD_SET(this->super.Seek, cxAssetsStreamSeek);
    CX_METHOD_SET(this->super.Close, cxAssetsStreamClose);
    CX_METHOD_SET(this->super.Position,cxAssetsStreamPosition);
    CX_METHOD_SET(this->super.AllBytes,cxAssetsStreamAllBytes);
}
CX_OBJECT_FREE(cxAssetsStream, cxStream)
{
    cxAssetsStreamClose(this);
}
CX_OBJECT_TERM(cxAssetsStream, cxStream)

cxString cxAssertsData(cxConstChars file)
{
    cxStream stream = cxAssetsStreamCreate(file);
    return cxStreamAllBytes(stream);
}

cxStream cxAssetsStreamCreate(cxConstChars file)
{
    cxString path = cxAssetsPath(file);
    cxAssetsStream rv = CX_CREATE(cxAssetsStream);
    CX_RETAIN_SWAP(rv->super.path, path);
    rv->super.file = cxStringAllocChars(file);
    return (cxStream)rv;
}







