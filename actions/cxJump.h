//
//  cxJump.h
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxJump_h
#define cxEngine_cxJump_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxJump, cxAction)
    cxVec2f startPos;
    cxVec2f position;
    cxFloat height;
    cxUInt  jumps;
    cxVec2f prevPos;
CX_OBJECT_END(cxJump)

cxJump cxJumpCreate(cxFloat duration,cxVec2f position,cxFloat height,cxUInt jumps);

CX_C_END

#endif
