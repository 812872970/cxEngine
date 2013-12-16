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
#include "cxPlayer.h"

CX_C_BEGIN

cxInt cxRand(cxInt min,cxInt max);

cxUInt cxHexToUInt(const cxChar *bs);

cxUInt cxBinaryToUInt(const cxChar *bs);

void cxSetRandSeed();

cxAudioFileType cxAudioGetType(cxConstChars file);

cxBool cxConstCharsHasChar(cxConstChars sp,cxChar c);

cxString cxCompressed(cxString data);

cxString cxDecompress(cxString data);

cxString cxCreateTXTTextureData(cxConstChars txt,cxConstChars font,cxTextAttr attr);

cxString cxWAVSamplesWithFile(cxConstChars file,cxUInt *format,cxUInt *freq);

cxHash cxParseKeyValue(cxChar *query);

cxString cxAssetsPath(cxConstChars file);

cxDouble cxTimestamp();

cxString cxLocaleLang();

cxString cxDocumentPath(cxConstChars file);

CX_C_END

#endif