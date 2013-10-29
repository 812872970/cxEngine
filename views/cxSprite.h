//
//  cxSprite.h
//  cxEngine
//
//  Created by xuhua on 10/11/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxSprite_h
#define cxEngine_cxSprite_h

#include <core/cxView.h>
#include <core/cxShader.h>
#include <core/cxTexture.h>

CX_C_BEGIN

CX_OBJECT_DEF(cxSprite, cxView)
    cxShader        shader;
    cxTexture       texture;
    GLenum          sfactor;
    GLenum          dfactor;
    cxBoxVec3f      vbox;
    cxBoxColor4f    cbox;
    cxBoxTex2f      tbox;
    cxBool          isFlipX;
    cxBool          isFlipY;
    cxBoxTex2f      texCoord;
CX_OBJECT_END(cxSprite)

cxBool cxSpriteXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader);

cxBoxTex2f cxSpriteBoxTex(cxAny pview);

cxSprite cxSpriteCreateWithXML(cxConstChars xml,cxConstChars key);

void cxSpriteSetBlendFactor(cxAny pview,GLenum sfactor, GLenum dfactor);

void cxSpriteSetFlipX(cxAny pview,cxBool flipx);

void cxSpriteSetFlipY(cxAny pview,cxBool flipy);

void cxSpriteTransform(cxAny pview);

void cxSpriteDraw(cxAny pview);

void cxSpriteSetShader(cxAny pview,cxConstChars key);

void cxSpriteSetTexture(cxAny pview,cxTexture texture);

void cxSpriteSetTextureKey(cxAny pview,cxConstChars key,cxBool equSize);

CX_C_END

#endif

