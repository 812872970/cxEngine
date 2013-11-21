//
//  cxRunner.c
//  cxEngineIOS
//
//  Created by xuhua on 11/15/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxEventArg.h>
#include "cxRunner.h"

static void cxRunnerInit(cxAny pav)
{
    cxRunner this = pav;
    CX_METHOD_RUN(this->Init,this);
}

static cxBool cxRunnerExit(cxAny pav)
{
    cxRunner this = pav;
    if(this->count == 0){
        //can add action at exit
        this->step ++;
        CX_METHOD_RUN(this->Exit,this);
    }
    return this->count == 0;
}

CX_OBJECT_INIT(cxRunner, cxAction)
{
    CX_METHOD_SET(this->super.Init, cxRunnerInit);
    CX_METHOD_SET(this->super.Exit, cxRunnerExit);
}
CX_OBJECT_FREE(cxRunner, cxAction)
{
    
}
CX_OBJECT_TERM(cxRunner, cxAction)

static void cxRunnerItemStop(cxEvent *event)
{
    cxRunner this = cxEventArgToWeakRef(event->args);
    this->count --;
}

void cxRunnerAppend(cxAny runner,cxAny pav, cxAny pview)
{
    cxAction action = pav;
    cxRunner this = runner;
    this->count ++;
    CX_EVENT_APPEND(action->onStop, cxRunnerItemStop, cxEventArgWeakRef(this));
    cxViewAppendAction(pview, pav);
}