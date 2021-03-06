//
//  cxFade.h
//  cxEngine
//
//  Created by xuhua on 10/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxFade_h
#define cxEngine_cxFade_h

#include <core/cxAction.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxFade, cxAction)
    cxFloat alpha;
    cxFloat delta;
    cxFloat start;
CX_OBJECT_END(cxFade)

cxFade cxFadeCreate(cxFloat duration,cxFloat alpha);

CX_C_END

#endif
