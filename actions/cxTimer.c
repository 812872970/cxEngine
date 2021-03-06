//
//  cxTimer.c
//  cxEngine
//
//  Created by xuhua on 10/26/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEventArg.h>
#include <core/cxActionRoot.h>
#include "cxTimer.h"

static cxInt cxTimerLuaAppendEvent(lua_State *L)
{
    CX_LUA_EVENT_BEG(cxTimer);
    CX_LUA_EVENT_APPEND(onArrive);
    CX_LUA_EVENT_END(cxTimer);
}

static cxInt cxTimerLuaGetRepeat(lua_State *L)
{
    CX_LUA_DEF_THIS(cxTimer);
    lua_pushinteger(L, this->repeat);
    return 1;
}

static cxInt cxTimerLuaSetRepeat(lua_State *L)
{
    CX_LUA_DEF_THIS(cxTimer);
    this->repeat = luaL_checkinteger(L, 2);
    return 1;
}

CX_LUA_METHOD_BEG(cxTimer)
    {"AppendEvent",cxTimerLuaAppendEvent},
    CX_LUA_PROPERTY(cxTimer, Repeat),
CX_LUA_METHOD_END(cxTimer)

void cxTimerTypeInit()
{
    CX_LUA_LOAD_TYPE(cxTimer);
}

static void cxTimerInit(cxAny pav)
{
    cxTimer this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    CX_UNUSED_PARAM(this);
}

static void cxTimerStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxTimer this = pav;
    CX_UNUSED_PARAM(this);
}

static void cxTimerReadAttr(cxReaderAttrInfo *info)
{
    cxActionReadAttr(info);
    cxTimer this = info->object;
    this->repeat = cxXMLReadIntAttr(info, "cxTimer.repeat", this->repeat);
    cxXMLAppendEvent(info, this, cxTimer, onArrive);
}

static cxBool cxTimerExit(cxAny pav)
{
    cxTimer this = pav;
    if(this->repeat > 0){
        this->repeat --;
    }
    CX_EVENT_FIRE(this, onArrive);
    return this->repeat == 0;
}

CX_OBJECT_INIT(cxTimer, cxAction)
{
    cxActionSetStepHide(this, true);
    cxObjectSetReadAttrFunc(this, cxTimerReadAttr);
    CX_METHOD_OVERRIDE(this->super.Init, cxTimerInit);
    CX_METHOD_OVERRIDE(this->super.Step, cxTimerStep);
    CX_METHOD_OVERRIDE(this->super.Exit, cxTimerExit);
}
CX_OBJECT_FREE(cxTimer, cxAction)
{
    CX_EVENT_RELEASE(this->onArrive);
}
CX_OBJECT_TERM(cxTimer, cxAction)

cxTimer cxTimerCreate(cxFloat time,cxInt repeat)
{
    cxTimer this = CX_CREATE(cxTimer);
    this->super.duration  = time;
    this->repeat = repeat;
    return this;
}