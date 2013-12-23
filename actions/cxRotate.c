//
//  cxRotate.c
//  cxEngine
//
//  Created by xuhua on 10/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include "cxRotate.h"

const luaL_Reg cxRotateInstanceMethods[] = {
    
    CX_LUA_SUPER(cxAction)
};

const luaL_Reg cxRotateTypeMethods[] = {
    
    CX_LUA_TYPE(cxRotate)
};

void cxRotateTypeInit()
{
    CX_LUA_LOAD_TYPE(cxRotate);
}

static void cxRotateInit(cxAny pav)
{
    cxRotate this = pav;
    CX_ASSERT(this->super.view != NULL, "view not set");
    this->oldAngle = this->super.view->angle;
    this->delta = this->newAngle - this->oldAngle;
    cxViewSetRaxis(this->super.view, this->raxis);
}

static void cxRotateStep(cxAny pav,cxFloat dt,cxFloat time)
{
    cxRotate this = pav;
    cxFloat angle = this->oldAngle + this->delta * time;
    cxViewSetAngle(this->super.view, angle);
}

static void cxRotateReadAttr(cxAny rootAction,cxAny mAction, xmlTextReaderPtr reader)
{
    cxActionReadAttr(rootAction, mAction, reader);
    cxRotate this = mAction;
    cxConstChars sx = cxXMLAttr("cxRotate.x");
    cxConstChars sy = cxXMLAttr("cxRotate.y");
    cxConstChars sz = cxXMLAttr("cxRotate.z");
    if(sx != NULL){
        this->raxis = cxVec3fv(1.0f, 0.0f, 0.0f);
        this->newAngle = kmDegreesToRadians(atof(sx));
    }else if(sy != NULL){
        this->raxis = cxVec3fv(0.0f, 1.0f, 0.0f);
        this->newAngle = kmDegreesToRadians(atof(sy));
    }else if(sz != NULL){
        this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
        this->newAngle = kmDegreesToRadians(atof(sz));
    }
}

CX_OBJECT_INIT(cxRotate, cxAction)
{
    this->raxis = cxVec3fv(0.0f, 0.0f, 1.0f);
    cxObjectSetReadAttrFunc(this, cxRotateReadAttr);
    CX_METHOD_SET(this->super.Init, cxRotateInit);
    CX_METHOD_SET(this->super.Step, cxRotateStep);
}
CX_OBJECT_FREE(cxRotate, cxAction)
{

}
CX_OBJECT_TERM(cxRotate, cxAction)

cxRotate cxRotateCreate(cxFloat duration,cxVec3f raxis,cxFloat newAngle)
{
    cxRotate this = CX_CREATE(cxRotate);
    this->super.duration = duration;
    this->raxis = raxis;
    this->newAngle = newAngle;
    return this;
}


