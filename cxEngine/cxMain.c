//
//  cxMain.c
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <core/cxHash.h>
#include <streams/cxAssetsStream.h>
#include <textures/cxTexturePVR.h>
#include <textures/cxTexturePNG.h>
#include <textures/cxTextureFactory.h>
#include "cxMain.h"
#include <core/cxDB.h>


#include <actions/cxMove.h>
#include <actions/cxCurve.h>
#include <core/cxActionXML.h>
#include <views/cxSprite.h>
#include <core/cxViewXML.h>
#include <views/cxButton.h>
#include <json/jansson.h>
#include <views/cxAtlas.h>
#include <views/cxParticle.h>
#include <views/cxTable.h>
#include <core/cxIconv.h>
#include <core/cxFreeType.h>
#include <views/cxLabel.h>
#include <core/cxDB.h>
#include <core/cxPlayer.h>
#include <socket/cxHttpGet.h>

static void cxEventTest(cxEvent *event)
{
    //cxActionXMLAttachView(view, "actions.xml", "move");
}

static void viewLoad(cxEvent *event)
{
    //pview == cxViewXML
    //add view custom event
    cxViewXMLRegisteEvent(event->sender, "cxEventTest",cxEventTest);
}

static void view2Load(cxEvent *event)
{
    //pview == cxViewXML
    //add view custom event
//    cxViewXMLRegisteEvent(xmlView, "cxEventTest",cxEventTest);
}

static void actionsLoad(cxEvent *event)
{
    CX_LOGGER("load actions.xml");
    //pav == cxActionXML
    //add action custom event
}

void cxEngineInit(cxEngine engine)
{
    //prepare load
    cxPlayerOpen(0, 0);
    //open db
    cxDBEnvOpen();
    engine->isShowBorder = false;
    cxEngineRegisteEvent("viewLoad", viewLoad);
    cxEngineRegisteEvent("view2Load", view2Load);
    cxEngineRegisteEvent("actionsLoad", actionsLoad);
}

cxLabel l= NULL;

static void t1(cxEvent *event)
{
    cxHttp this = event->sender;
    cxLabelSetText(l, cxStringCreate("%lld-%lld",this->readBytes,this->bodyBytes));
}

void cxEngineMain(cxEngine engine)
{

    cxActionXMLGet("actions.xml","move");
    
    cxHttp get1 = cxHttpGetCreate("http://s2.remword.cn/mongodb-linux-x86_64-2.4.1.tgz", true);
    CX_EVENT_QUICK(get1->onChunked, t1);
    CX_RETAIN(get1);
    
    //load items.xml
    cxEngineDataSet("items.xml");
    
    l = cxLabelCreate(cxStringConstChars("%"), cxStringConstChars("banana.ttf"), 50);
    cxViewAppend(engine->window, l);
    
    cxWindowPushXML("main.xml");
    
}

void cxEngineFree(cxEngine engine)
{
    
}








