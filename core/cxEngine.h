//
//  cxEngine.h
//  cxEngine
//  引擎主要定义
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxEngine_h
#define cxEngine_cxEngine_h

#include <views/cxWindow.h>
#include <actions/cxCurve.h>
#include "cxBase.h"
#include "cxStack.h"
#include "cxTypes.h"
#include "cxInput.h"
#include "cxXMLScript.h"
#include "cxEventItem.h"
#include "cxEventArg.h"
#include "cxUtil.h"
#include "cxMessage.h"

CX_C_BEGIN

#define GL_ASSERT() CX_ASSERT(glGetError() == GL_NO_ERROR,"OpenGL error")

CX_OBJECT_DEF(cxEngine, cxObject)
    cxHash events;
    cxHash scripts;
    cxHash datasets;
    cxHash actions;
    cxStack autopool;
    cxFloat frameInterval;
    cxSize2f winsize;   //screen size
    cxSize2f dessize;   //design size
    cxVec2f scale;
    cxBool isInit;
    cxBool isShowBorder;
    cxWindow window;
    cxDouble lastTime;
    cxFloat frameDelta;
    cxFloat contentScaleFactor;
    CX_SIGNAL_ALLOC(onPause);
    CX_SIGNAL_ALLOC(onResume);
    CX_SIGNAL_ALLOC(onMemory);
    CX_SIGNAL_ALLOC(onUpdate);
    CX_EVENT_ALLOC(onFree);
    cxTouch touch;
    cxString lang;
CX_OBJECT_END(cxEngine)

cxEngine cxEngineInstance();

cxStack cxEngineAutoStack();

void cxEngineRegisteEvent(cxConstChars name,cxEventFunc func);

cxEventItem cxEngineGetEvent(cxConstChars name);

void cxEngineRemoveScript(cxConstChars file);

void cxEngineTimeReset();

cxXMLScript cxEngineGetXMLScript(cxConstChars file);

void cxEngineSetLocalLang(cxString lang);

cxTypes cxEngineDataSet(cxConstChars url);

cxString cxEngineTypesText(cxConstChars xml,cxConstChars key);

void cxEngineSystemInit();

//extern
void cxEngineInit(cxEngine engine);
void cxEngineMain(cxEngine engine);
void cxEngineFree(cxEngine engine);
//

cxVec2f cxEngineTouchToWindow(cxVec2f pos);

cxVec2f cxEngineWindowToTouch(cxVec2f pos);

cxBool cxEngineFireTouch(cxTouchType type,cxVec2f pos);

void cxEngineLookAt(cxMatrix4f *matrix,const cxVec2f point);

void cxEngineExit();

void cxEnginePause();

void cxEngineResume();

void cxEngineMemory();

void cxEngineDraw();

void cxEngineLayout(cxInt width,cxInt height);

void cxEngineDestroy();

CX_C_END

#endif
