//
//  cxList.h
//  cxEngine
//
//  Created by xuhua on 9/25/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//

#ifndef cxEngine_cxList_h
#define cxEngine_cxList_h

#include <utlist.h>
#include "cxBase.h"


CX_C_BEGIN

#define CX_LIST_FOREACH(_list_,_ele_)               \
cxListElement *_ele_ = NULL;                        \
DL_FOREACH((_list_)->listptr,_ele_)

#define CX_LIST_FOREACH_SAFE(_list_,_ele_,_tmp_)    \
cxListElement *_ele_ = NULL,*_tmp_=NULL;            \
DL_FOREACH_SAFE((_list_)->listptr,_ele_,_tmp_)

typedef struct cxListElement cxListElement;
struct cxListElement {
    cxListElement *prev, *next;
    cxAny any;
};

typedef int (*cxListCmpFunc)(cxListElement *lp,cxListElement *rp);

CX_OBJECT_DEF(cxList, cxObject)
    cxListElement *listptr;
CX_OBJECT_END(cxList)

#define cxListSort(_list_,_cmp_)    DL_SORT((_list_)->listptr, _cmp_)

cxInt cxListLength(cxList list);

cxListElement *cxListAppend(cxList list,cxAny any);

cxListElement *cxListPrepend(cxList list,cxAny any);

cxListElement *cxListFirst(cxList list);

cxListElement *cxListLast(cxList list);

void cxListRemove(cxList list,cxListElement *element);

void cxListClean(cxList list);

CX_C_END

#endif
