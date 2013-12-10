//
//  cxParticle.c
//  cxEngine
//
//  Created by xuhua on 10/18/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#include <streams/cxAssetsStream.h>
#include <core/cxEngine.h>
#include <core/cxUtil.h>
#include "cxParticle.h"

/*
 <particleEmitterConfig>
     <texture name="texture.png"/>
     <sourcePosition x="300.00" y="300.00"/>
     <sourcePositionVariance x="0.00" y="0.00"/>
     <speed value="100.00"/>
     <speedVariance value="30.00"/>
     <particleLifeSpan value="2.0000"/>
     <particleLifespanVariance value="1.9000"/>
     <angle value="270.00"/>
     <angleVariance value="2.00"/>
     <gravity x="0.00" y="0.00"/>
     <radialAcceleration value="0.00"/>
     <tangentialAcceleration value="0.00"/>
     <radialAccelVariance value="0.00"/>
     <tangentialAccelVariance value="0.00"/>
     <startColor red="1.00" green="0.31" blue="0.00" alpha="0.62"/>
     <startColorVariance red="0.00" green="0.00" blue="0.00" alpha="0.00"/>
     <finishColor red="1.00" green="0.31" blue="0.00" alpha="0.00"/>
     <finishColorVariance red="0.00" green="0.00" blue="0.00" alpha="0.00"/>
     <maxParticles value="500"/>
     <startParticleSize value="70.00"/>
     <startParticleSizeVariance value="49.53"/>
     <finishParticleSize value="10.00"/>
     <FinishParticleSizeVariance value="5.00"/>
     <particleToDir value="false" />
     <duration value="-1.00"/>
     <emitterType value="0"/>
     <emitterRate value="-1"/>
     <maxRadius value="100.00"/>
     <maxRadiusVariance value="0.00"/>
     <minRadius value="0.00"/>
     <rotatePerSecond value="0.00"/>
     <rotatePerSecondVariance value="0.00"/>
     <blendFuncSource value="770"/>
     <blendFuncDestination value="1"/>
     <rotationStart value="0.00"/>
     <rotationStartVariance value="0.00"/>
     <rotationEnd value="0.00"/>
     <rotationEndVariance value="0.00"/>
 </particleEmitterConfig>
 */

static void cxParticleXMLReaderError(void *arg,const char *msg,xmlParserSeverities severity,xmlTextReaderLocatorPtr locator)
{
    cxParticle this = arg;
    CX_ERROR("%s",msg);
    this->isError = true;
}

void cxParticleInitFromFile(cxAny pview,cxConstChars file)
{
    cxParticle this = pview;
    cxString data = cxAssertsData(file);
    CX_RETURN(data == NULL);
    xmlTextReaderPtr reader = cxXMLReaderForString(data);
    xmlTextReaderSetErrorHandler(reader, cxParticleXMLReaderError, this);
    while(xmlTextReaderRead(reader) && !this->isError){
        if(xmlTextReaderNodeType(reader) != XML_READER_TYPE_ELEMENT){
            continue;
        }
        const xmlChar *temp = xmlTextReaderConstName(reader);
        if(ELEMENT_IS_TYPE(texture)){
            cxChar *name = cxXMLAttr("name");
            cxSpriteSetTextureURL(this, name, true, true);
            xmlFree(name);
        }else if(ELEMENT_IS_TYPE(sourcePosition)){
            //this->position.v.x = cxXMLReadFloatAttr(reader, "x", 0);
            //this->position.v.y = cxXMLReadFloatAttr(reader, "y", 0);
        }else if(ELEMENT_IS_TYPE(sourcePositionVariance)){
            this->position.r.x = cxXMLReadFloatAttr(reader, "x", 0);
            this->position.r.y = cxXMLReadFloatAttr(reader, "y", 0);
        }else if(ELEMENT_IS_TYPE(speed)){
            this->speed.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if(ELEMENT_IS_TYPE(speedVariance)){
            this->speed.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if(ELEMENT_IS_TYPE(particleLifeSpan)){
            this->life.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if(ELEMENT_IS_TYPE(particleLifespanVariance)){
            this->life.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if(ELEMENT_IS_TYPE(angle)){
            this->angle.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if(ELEMENT_IS_TYPE(angleVariance)){
            this->angle.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if(ELEMENT_IS_TYPE(gravity)){
            this->gravity.x = cxXMLReadFloatAttr(reader, "x", 0);
            this->gravity.y = cxXMLReadFloatAttr(reader, "y", 0);
        }else if (ELEMENT_IS_TYPE(radialAcceleration)){
            this->radaccel.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(radialAccelVariance)){
            this->radaccel.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(tangentialAcceleration)){
            this->tanaccel.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(tangentialAccelVariance)){
            this->tanaccel.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(startColor)){
            this->startcolor.v.r = cxXMLReadFloatAttr(reader, "red", 0);
            this->startcolor.v.g = cxXMLReadFloatAttr(reader, "green", 0);
            this->startcolor.v.b = cxXMLReadFloatAttr(reader, "blue", 0);
            this->startcolor.v.a = cxXMLReadFloatAttr(reader, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(startColorVariance)){
            this->startcolor.r.r = cxXMLReadFloatAttr(reader, "red", 0);
            this->startcolor.r.g = cxXMLReadFloatAttr(reader, "green", 0);
            this->startcolor.r.b = cxXMLReadFloatAttr(reader, "blue", 0);
            this->startcolor.r.a = cxXMLReadFloatAttr(reader, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(finishColor)){
            this->endcolor.v.r = cxXMLReadFloatAttr(reader, "red", 0);
            this->endcolor.v.g = cxXMLReadFloatAttr(reader, "green", 0);
            this->endcolor.v.b = cxXMLReadFloatAttr(reader, "blue", 0);
            this->endcolor.v.a = cxXMLReadFloatAttr(reader, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(finishColorVariance)){
            this->endcolor.r.r = cxXMLReadFloatAttr(reader, "red", 0);
            this->endcolor.r.g = cxXMLReadFloatAttr(reader, "green", 0);
            this->endcolor.r.b = cxXMLReadFloatAttr(reader, "blue", 0);
            this->endcolor.r.a = cxXMLReadFloatAttr(reader, "alpha", 0);
        }else if (ELEMENT_IS_TYPE(maxParticles)){
            cxInt number = cxXMLReadIntAttr(reader, "value", 0);
            CX_ASSERT(number > 0, "particle number must > 0");
            cxParticleInit(this, number);
        }else if (ELEMENT_IS_TYPE(startParticleSize)){
            this->startsize.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(startParticleSizeVariance)){
            this->startsize.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(finishParticleSize)){
            this->endsize.v = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(FinishParticleSizeVariance)){
            this->endsize.r = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(duration)){
            this->duration = cxXMLReadFloatAttr(reader, "value", 0);
        }else if (ELEMENT_IS_TYPE(emitterType)){
            this->type = cxXMLReadIntAttr(reader, "value", cxParticleEmitterGravity);
        }else if (ELEMENT_IS_TYPE(blendFuncSource)){
            this->super.super.sfactor = cxXMLReadIntAttr(reader, "value", GL_SRC_ALPHA);
        }else if (ELEMENT_IS_TYPE(blendFuncDestination)){
            this->super.super.dfactor = cxXMLReadIntAttr(reader, "value", GL_ONE_MINUS_SRC_ALPHA);
        }else if (ELEMENT_IS_TYPE(emitterRate)){
            this->rate = cxXMLReadFloatAttr(reader, "value", this->rate);
        }else if (ELEMENT_IS_TYPE(particleToDir)){
            this->todir = cxXMLReadBoolAttr(reader, "value", this->todir);
        }
    }
    xmlTextReaderClose(reader);
}

void cxParticleXMLReadAttr(cxAny xmlView,cxAny mView, xmlTextReaderPtr reader)
{
    cxAtlasXMLReadAttr(xmlView, mView, reader);
    cxParticle this = mView;
    cxChar *src = cxXMLAttr("cxParticle.src");
    if(src != NULL){
        cxParticleInitFromFile(mView, src);
        xmlFree(src);
        return;
    }
    cxInt number = cxXMLReadIntAttr(reader, "cxParticle.number", this->number);
    CX_ASSERT(number > 0, "cxParticle number must > 0");
    cxParticleInit(this, number);
    //mode add multiply
    cxChar *smode = cxXMLAttr("cxParticle.blend");
    if(cxConstCharsEqu(smode, "add")){
        cxParticleSetBlendMode(this, cxParticleBlendAdd);
    }else if(cxConstCharsEqu(smode, "multiple")){
        cxParticleSetBlendMode(this, cxParticleBlendMultiply);
    }else{//default mode
        cxParticleSetBlendMode(this, cxParticleBlendMultiply);
    }
    xmlFree(smode);
    //
    this->todir = cxXMLReadBoolAttr(reader, "cxParticle.todir", this->todir);
    cxXMLReadFloatsAttr(reader, "cxParticle.duration", &this->duration);
    cxXMLReadFloatsAttr(reader, "cxParticle.gravity", &this->gravity.x);
    cxXMLReadFloatsAttr(reader, "cxParticle.rate", &this->rate);
    cxXMLReadFloatsAttr(reader, "cxParticle.speed", &this->speed.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.tanaccel", &this->tanaccel.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.radaccel", &this->radaccel.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.position", &this->position.v.x);
    cxXMLReadFloatsAttr(reader, "cxParticle.life", &this->life.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.angle", &this->angle.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.startsize", &this->startsize.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.endsize", &this->endsize.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.startcolor", &this->startcolor.v.r);
    cxXMLReadFloatsAttr(reader, "cxParticle.endcolor", &this->endcolor.v.r);
    cxXMLReadFloatsAttr(reader, "cxParticle.startspin", &this->startspin.v);
    cxXMLReadFloatsAttr(reader, "cxParticle.endspin", &this->endspin.v);
}

void cxParticleStop(cxAny pview)
{
    cxParticle this = pview;
    this->isActive = false;
    this->emitcounter = 0;
    cxAtlasClean(pview);
}

void cxParticleReset(cxAny pview)
{
    cxParticle this = pview;
    this->isActive = true;
    this->emitcounter = 0;
    this->elapsed = 0;
    for(this->index = 0; this->index < this->count; ++this->index){
        cxParticleUnit *p = &this->units[this->index];
        p->life = 0;
    }
    this->position.r = cxVec2fv(0, 0);
    this->position.v = cxVec2fv(0, 0);
    cxAtlasClean(pview);
}

static void cxParticleUnitToBoxVec3f(cxParticleUnit *particle,cxBoxVec3f *vq)
{
    cxFloat sizeh = particle->size/2;
    if (particle->rotation){
        cxFloat x1 = -sizeh;
        cxFloat y1 = -sizeh;
        cxFloat x2 = sizeh;
        cxFloat y2 = sizeh;
        cxFloat r = -kmDegreesToRadians(particle->rotation);
        cxFloat cr = cosf(r);
        cxFloat sr = sinf(r);
        vq->lb.x = x1 * cr - y1 * sr + particle->position.x;
        vq->lb.y = x1 * sr + y1 * cr + particle->position.y;
        vq->rb.x = x2 * cr - y1 * sr + particle->position.x;
        vq->rb.y = x2 * sr + y1 * cr + particle->position.y;
        vq->rt.x = x2 * cr - y2 * sr + particle->position.x;
        vq->rt.y = x2 * sr + y2 * cr + particle->position.y;
        vq->lt.x = x1 * cr - y2 * sr + particle->position.x;
        vq->lt.y = x1 * sr + y2 * cr + particle->position.y;
    }else{
        vq->lb.x = particle->position.x - sizeh;
        vq->lb.y = particle->position.y - sizeh;
        vq->rb.x = particle->position.x + sizeh;
        vq->rb.y = particle->position.y - sizeh;
        vq->lt.x = particle->position.x - sizeh;
        vq->lt.y = particle->position.y + sizeh;
        vq->rt.x = particle->position.x + sizeh;
        vq->rt.y = particle->position.y + sizeh;
    }
}

static void cxParticleStep(cxAny pview,cxParticleUnit *particle)
{
    cxParticle this = pview;
    cxBoxVec3f vq={0};
    cxColor4f color = particle->color;
    cxParticleUnitToBoxVec3f(particle, &vq);
    cxBoxPoint *box = &this->super.boxes[this->index];
    box->lb.colors = color;
    box->rb.colors = color;
    box->lt.colors = color;
    box->rt.colors = color;
    box->lb.texcoords = this->super.super.texCoord.lb;
    box->rb.texcoords = this->super.super.texCoord.rb;
    box->lt.texcoords = this->super.super.texCoord.lt;
    box->rt.texcoords = this->super.super.texCoord.rt;
    box->lb.vertices = vq.lb;
    box->rb.vertices = vq.rb;
    box->lt.vertices = vq.lt;
    box->rt.vertices = vq.rt;
    CX_METHOD_RUN(this->UpdateBox,this,box);
    this->super.number = CX_MAX(this->index + 1, this->super.number);
}

static void cxParticleInitUnit(cxAny pview,cxParticleUnit *particle)
{
    cxParticle this = pview;
    particle->life = cxFloatValue(this->life);
    particle->position = cxVec2fValue(this->position);
    
    cxColor4f startcolor = cxColorValue(this->startcolor);
    cxColor4f endcolor = cxColorValue(this->endcolor);
    
    particle->color = startcolor;
    particle->deltacolor.r = (endcolor.r - startcolor.r) / particle->life;
    particle->deltacolor.g = (endcolor.g - startcolor.g) / particle->life;
    particle->deltacolor.b = (endcolor.b - startcolor.b) / particle->life;
    particle->deltacolor.a = (endcolor.a - startcolor.a) / particle->life;
    
    cxFloat startsize = cxFloatValue(this->startsize);
    particle->size = startsize;
    cxFloat endsize = cxFloatValue(this->endsize);
    particle->deltasize = (endsize - startsize) / particle->life;
    
    cxFloat startspin = cxFloatValue(this->startspin);
    cxFloat endspin = cxFloatValue(this->endspin);
    particle->rotation = startspin;
    particle->deltarotation = (endspin - startspin) / particle->life;
    
    cxFloat angle = kmDegreesToRadians(cxFloatValue(this->angle));
    cxVec2f v = cxVec2fv(cosf(angle), sinf(angle));
    cxFloat speed = cxFloatValue(this->speed);
    kmVec2Scale(&particle->dir, &v, speed);
    particle->radaccel = cxFloatValue(this->radaccel);
    particle->tanaccel = cxFloatValue(this->tanaccel);
    if(this->todir){
        particle->rotation = -kmRadiansToDegrees(cxVec2fAngle(particle->dir));
    }
}

static cxBool cxParticleAdd(cxAny pview)
{
    cxParticle this = pview;
    if(this->count >= this->number){
        return false;
    }
    cxParticleUnit *unit = &this->units[this->count];
    cxParticleInitUnit(pview, unit);
    unit->index = this->count;
    this->count ++;
    return true;
}

static void cxParticleUpdate(cxEvent *event)
{
    cxEngine engine = cxEngineInstance();
    cxFloat dt = engine->frameDelta;
    cxParticle this = event->sender;
    if(this->isActive){
        cxFloat rate = 1.0f / this->rate;
        if(this->count < this->number){
            this->emitcounter += dt;
        }
        while(this->count < this->number && this->emitcounter > rate && cxParticleAdd(this)){
            this->emitcounter -= rate;
        }
        this->elapsed += dt;
        if(this->duration != -1 && this->elapsed > this->duration){
            this->isActive = false;
            this->emitcounter = 0;
            cxAtlasClean(this);
            return;
        }
    }
    this->index = 0;
    while(this->index < this->count){
        cxParticleUnit *p = &this->units[this->index];
        p->life -= dt;
        if(p->life > 0){
            cxVec2f tmp;
            cxVec2f radial= cxVec2fv(0, 0);
            if(p->position.x || p->position.y){
                kmVec2Normalize(&radial, &p->position);
            }
            cxVec2f tangential = radial;
            kmVec2Scale(&radial, &radial, p->radaccel);
            //compute tangential
            float newy = tangential.x;
            tangential.x = -tangential.y;
            tangential.y = newy;
            kmVec2Scale(&tangential,&tangential,p->tanaccel);
            //compute position
            kmVec2Add(&tmp, &radial, &tangential);
            kmVec2Add(&tmp, &tmp, &this->gravity);
            kmVec2Scale(&tmp, &tmp, dt);
            kmVec2Add(&p->dir, &p->dir, &tmp);
            kmVec2Scale(&tmp, &p->dir, dt);
            kmVec2Add(&p->position, &p->position, &tmp);
            // color
            p->color.r += (p->deltacolor.r * dt);
            p->color.g += (p->deltacolor.g * dt);
            p->color.b += (p->deltacolor.b * dt);
            p->color.a += (p->deltacolor.a * dt);
            // size
            p->size = CX_MAX(0, p->size + p->deltasize * dt);
            // angle
            p->rotation += (p->deltarotation * dt);
            cxParticleStep(this,p);
            this->index ++;
        }else{
            if(this->index != this->count - 1){
                this->units[this->index] = this->units[this->count - 1];
            }
            this->count--;
        }
    }
    this->super.isDirty = true;
}

void cxParticleSetBlendMode(cxAny pview,cxParticleBlendMode mode)
{
    cxParticle this = pview;
    if(mode == cxParticleBlendAdd){
        cxSpriteSetBlendFactor(pview, GL_SRC_ALPHA, GL_ONE);
    }else{
        cxSpriteSetBlendFactor(pview, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    this->blend = mode;
}

CX_OBJECT_INIT(cxParticle, cxAtlas)
{
    this->rate = -1;
    this->duration = -1;
    this->isActive = true;
    this->type = cxParticleEmitterGravity;
    cxViewOnUpdate(this, cxParticleUpdate);
    cxSpriteSetBlendFactor(this, GL_SRC_ALPHA, GL_ONE);
    cxObjectSetXMLReadFunc(this, cxParticleXMLReadAttr);
}
CX_OBJECT_FREE(cxParticle, cxAtlas)
{
    allocator->free(this->units);
}
CX_OBJECT_TERM(cxParticle, cxAtlas)

void cxParticleInit(cxAny pview,cxInt number)
{
    cxParticle this = pview;
    this->number = number;
    this->units = allocator->calloc(number,sizeof(cxParticleUnit));
    cxAtlasSetNumber(this, number);
    cxSetRandSeed();
}

cxColor4f cxColorValue(cxColorRange cv)
{
    cxColor4f color;
    color.r = kmClamp(cv.v.r + cv.r.r * CX_RAND_11f(), 0.0f, 1.0f);
    color.g = kmClamp(cv.v.g + cv.r.g * CX_RAND_11f(), 0.0f, 1.0f);
    color.b = kmClamp(cv.v.b + cv.r.b * CX_RAND_11f(), 0.0f, 1.0f);
    color.a = kmClamp(cv.v.a + cv.r.a * CX_RAND_11f(), 0.0f, 1.0f);
    return color;
}

cxVec2f cxVec2fValue(cxVec2fRange rv)
{
    cxVec2f pos;
    pos.x = rv.v.x + rv.r.x * CX_RAND_11f();
    pos.y = rv.v.y + rv.r.y * CX_RAND_11f();
    return pos;
}


