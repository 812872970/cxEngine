//
//  cxEngine.c
//  cxEngine
//
//  Created by xuhua on 9/24/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <kazmath/matrix.h>
#include <streams/cxAssetsStream.h>
#include <core/cxActionXML.h>
#include "cxEngine.h"
#include "cxAutoPool.h"
#include "cxOpenGL.h"
#include "cxUtil.h"

static cxEngine instance = NULL;

void cxEngineExit()
{
    cxEngine engine = cxEngineInstance();
    CX_EVENT_FIRE(engine, onFree);
    cxEnginePause();
    cxEngineDestroy();
    exit(0);
}

void cxEnginePause()
{
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onPause, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineResume()
{
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onResume, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineMemory()
{
    cxEngine engine = cxEngineInstance();
    CX_SIGNAL_FIRE(engine->onMemory, CX_FUNC_TYPE(cxAny),CX_SLOT_OBJECT);
}

void cxEngineDraw()
{
    cxEngine engine = cxEngineInstance();
    CX_RETURN(!engine->isInit);
    cxOpenGLClear();
    cxAutoPoolBegin();
    
    cxDouble now = cxTimestamp();
    engine->frameDelta = now - engine->lastTime;
    CX_SIGNAL_FIRE(engine->onUpdate, CX_FUNC_TYPE(cxAny,cxFloat),CX_SLOT_OBJECT,engine->frameDelta);
    
    kmGLPushMatrix();
    cxViewDraw(engine->window);
    kmGLPopMatrix();

    engine->lastTime = now;
    
    cxAutoPoolClean();
}

void cxEngineLookAt(cxMatrix4f *matrix,const cxVec2f point)
{
    cxEngine engine = cxEngineInstance();
    cxFloat zeye = engine->winsize.h / 1.1566f;
    cxVec3f eye;
    cxVec3f center;
    cxVec3f up;
    kmVec3Fill(&eye, point.x, point.y, zeye);
    kmVec3Fill(&center, point.x, point.y, 0.0f);
    kmVec3Fill(&up, 0.0f, 1.0f, 0.0f);
    kmMat4Identity(matrix);
    kmMat4LookAt(matrix, &eye, &center, &up);
}

void cxEngineLayout(cxInt width,cxInt height)
{
    CX_LOGGER("openGL layout width=%d height=%d",width,height);
    cxEngine engine = cxEngineInstance();
    engine->winsize = cxSize2fv(width, height);
    cxViewSetSize(engine->window, engine->winsize);
    //
    if(!engine->isInit){
        cxOpenGLCheckFeature();
        cxEngineMain(engine);
    }
    //
    cxFloat zeye = engine->winsize.h / 1.1566f;
    kmMat4 perspective={0};
    kmGLMatrixMode(KM_GL_PROJECTION);
    kmGLLoadIdentity();
    //
    kmMat4PerspectiveProjection(&perspective, 60.0f, engine->winsize.w / engine->winsize.h, 0.0f, zeye * 2);
    kmGLMultMatrix(&perspective);
    kmGLMatrixMode(KM_GL_MODELVIEW);
    kmGLLoadIdentity();
    //init gl
    cxOpenGLViewport(cxBox4fv(0, engine->winsize.w, 0, engine->winsize.h));
    //
    cxMatrix4f matrix;
    cxEngineLookAt(&matrix,cxVec2fv(0, 0));
    kmGLMultMatrix(&matrix);
    engine->lastTime = cxTimestamp();
    if(!engine->isInit){
        cxViewEnter(engine->window);
    }
    cxViewLayout(engine->window);
    engine->isInit = true;
}

CX_OBJECT_INIT(cxEngine, cxObject)
{
    kmGLInitialize();
    this->autoStack = CX_ALLOC(cxStack);
    this->frameInterval = 1.0f/60.0f;
    this->isShowBorder = true;
    this->contentScaleFactor = 1.0f;
    this->window = CX_ALLOC(cxWindow);
    this->scripts = CX_ALLOC(cxHash);
    this->events = CX_ALLOC(cxHash);
    this->actions = CX_ALLOC(cxHash);
    this->curve = CX_ALLOC(cxCurve);
}
CX_OBJECT_FREE(cxEngine, cxObject)
{
    CX_RELEASE(this->curve);
    CX_RELEASE(this->actions);
    CX_RELEASE(this->events);
    CX_RELEASE(this->scripts);
    CX_EVENT_RELEASE(this->onFree);
    CX_SIGNAL_RELEASE(this->onUpdate);
    CX_SIGNAL_RELEASE(this->onPause);
    CX_SIGNAL_RELEASE(this->onResume);
    CX_SIGNAL_RELEASE(this->onMemory);
    CX_RELEASE(this->window);
    cxOpenGLDestroy();
    CX_RELEASE(this->autoStack);
    kmGLFreeAll();
}
CX_OBJECT_TERM(cxEngine, cxObject)

cxStack cxEngineAutoStack()
{
    return cxEngineInstance()->autoStack;
}

cxEngine cxEngineInstance()
{
    if(instance == NULL) {
        instance = CX_ALLOC(cxEngine);
        cxEngineRegisteSystemEvent();
    }
    return instance;
}

void cxEngineDestroy()
{
    if(instance != NULL) {
        cxEngineFree(instance);
        CX_RELEASE(instance);
        instance = NULL;
    }
}

void cxEngineRegisteEvent(cxConstChars name,cxEventFunc func)
{
    cxEngine this = cxEngineInstance();
    cxEventItem event = cxHashGet(this->events, cxHashStrKey(name));
    CX_ASSERT(event == NULL, "name %s event registered",name);
    event = CX_ALLOC(cxEventItem);
    event->func = func;
    cxHashSet(this->events, cxHashStrKey(name), event);
    CX_RELEASE(event);
}

cxEventItem cxEngineGetEvent(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    return cxHashGet(this->events, cxHashStrKey(name));
}

void cxEngineRegisteSystemEvent()
{
    cxEngineRegisteEvent("cxViewRun", cxViewRunActionEvent);
    cxEngineRegisteEvent("cxActionRun", cxActionRunActionEvent);
    cxEngineRegisteEvent("cxPlay", cxPlaySoundEvent);
    cxEngineRegisteEvent("cxLogger", cxPrintMessageEvent);
    cxEngineRegisteEvent("cxActionRemoveView", cxActionRemoveViewEvent);
}

cxXMLScript cxEngineGetXMLScript(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    CX_RETURN(file == NULL,NULL);
    cxXMLScript script = cxHashGet(this->scripts, cxHashStrKey(file));
    if(script != NULL){
        return script;
    }
    script = CX_ALLOC(cxXMLScript);
    cxStream stream = cxAssetsStreamCreate(file);
    if(stream != NULL){
        cxString data = stream->interface->AllBytes(stream);
        CX_RETAIN_SWAP(script->bytes, data);
    }
    if(script->bytes != NULL){
        cxHashSet(this->scripts, cxHashStrKey(file), script);
    }else{
        CX_ERROR("file %s can't load bytes",file);
    }
    CX_RELEASE(script);
    return script;
}

cxCurveItem cxEngineGetCurve(cxConstChars name)
{
    cxEngine this = cxEngineInstance();
    CX_RETURN(name == NULL,NULL);
    return cxHashGet(this->curve->curves, cxHashStrKey(name));
}

void cxEngineRemoveScript(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    CX_RETURN(file == NULL);
    cxHashDel(this->scripts, cxHashStrKey(file));
}

cxAny cxEngineLoadActionXML(cxConstChars file)
{
    cxEngine this = cxEngineInstance();
    cxActionXML action = cxHashGet(this->actions, cxHashStrKey(file));
    CX_RETURN(action != NULL,action);
    //
    action = CX_ALLOC(cxActionXML);
    if(cxActionXMLLoad(action, file)){
        cxHashSet(this->actions, cxHashStrKey(file), action);
    }
    CX_RELEASE(action);
    return action;
}

cxBool cxEngineFireTouch(cxTouchType type,cxVec2f pos)
{
    cxEngine this = cxEngineInstance();
    double now = cxTimestamp();
    cxVec2f cpos = cxGLPointToWindowPoint(pos);
    if(type == cxTouchTypeDown){
        this->touch.delta = cxVec2fv(0, 0);
        this->touch.previous = cpos;
    }else if(type == cxTouchTypeMove){
        this->touch.delta = cxVec2fv(cpos.x - this->touch.previous.x, cpos.y - this->touch.previous.y);
        this->touch.previous = cpos;
    }
    this->touch.current = cpos;
    this->touch.type = type;
    this->touch.timestamp = now;
    return cxViewTouch(this->window, &this->touch);
}










