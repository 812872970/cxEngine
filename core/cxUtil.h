//
//  cxUtil.h
//  cxEngine
//  引擎工具方法
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxUtil_h
#define cxEngine_cxUtil_h

#include "cxConfig.h"
#include "cxString.h"
#include "cxHash.h"

CX_C_BEGIN

// -1 <-> 1
#define CX_RAND_11f() ((2.0f*((cxFloat)rand()/(cxFloat)RAND_MAX))-1.0f)

// 0 <-> 1
#define CX_RAND_01f() ((cxFloat)rand()/(cxFloat)RAND_MAX)

void cxSetRandSeed();

cxBool cxParseKeyValue(cxChar *query,cxChar *key,cxChar *value);

cxInt cxParseURL(cxString url,cxChar *path,cxChar *query);

cxString cxAssetsPath(cxConstChars file);

cxDouble cxTimestamp();

cxULong cxNextPot(cxULong x);

cxString cxLocaleLang();

CX_C_END

#endif