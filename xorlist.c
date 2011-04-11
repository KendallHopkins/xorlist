//
//  xorlinkedlist.c
//  xorlinkedlist
//
//  Created by Kendall Hopkins on 3/7/11.
//  Copyright 2011 SoftwareElves. All rights reserved.
//

#include "xorlist.h"

#include <stdint.h>
#include <assert.h>

#define XORPointers( p1, p2 ) ((XORListNode *)((intptr_t)p1 ^ (intptr_t)p2))

typedef enum {
    XORLISTITER_DIRECTION_FORWARD,
    XORLISTITER_DIRECTION_BACKWARD
} XORListIterDirection;

typedef enum {
    XORLISTITER_LOCATION_FRONT,
    XORLISTITER_LOCATION_BACK
} XORListIterLocation;

typedef struct XORListNode XORListNode;

struct XORList {
    XORListNode * start;
    XORListNode * end;
    size_t size;
};

struct XORListNode {
    XORListNode * XORPointer;
    void * data;
};

struct XORListIter {
    const XORList * list;
    XORListNode * next;
    XORListNode * current;
    XORListIterDirection direction;
};

XORListIter * _XORListIter_new( const XORList * list, XORListIterLocation location );
bool _XORList_pop( XORList * list, void ** data, XORListIterLocation location );
void _XORList_push( XORList * list, void * data, XORListIterLocation location );
void _XORListIter_flipDirection( XORListIter * iter );
bool _XORListIter_seak( XORListIter * iter, void ** data, XORListIterLocation direction );

XORList * XORList_new()
{
    XORList * newList = (XORList *)malloc(sizeof(XORList));
    newList->start = NULL;
    newList->end = NULL;
    newList->size = 0;
    return newList;
}

void XORList_delete( XORList * list )
{
    assert(list);
  
    while( list->start )
        XORList_popFront( list, NULL );
    
    free( list );
}

size_t XORList_size( const XORList * list )
{
    assert(list);
    return list->size;
}

void XORList_mergeAndDelete( XORList * base, XORList * mergedAndDeleted )
{
    assert(base);
    assert(mergedAndDeleted);
    
    if( base->end && mergedAndDeleted->start ) {
        base->end->XORPointer = XORPointers( base->end->XORPointer, mergedAndDeleted->start );
        mergedAndDeleted->start->XORPointer = XORPointers( mergedAndDeleted->start->XORPointer, base->end );        
    }
    
    if( ! base->start )
        base->start = mergedAndDeleted->start;
    
    base->end = mergedAndDeleted->end;
    base->size += mergedAndDeleted->size;
    free( mergedAndDeleted );
}

inline bool _XORList_pop( XORList * list, void ** data, XORListIterLocation location )
{
    if( list->size == 0 )
        return false;
    
    XORListNode ** popEnd;
    XORListNode ** otherEnd;
    if( location == XORLISTITER_LOCATION_FRONT ) {
        popEnd = &list->start;
        otherEnd = &list->end;
    } else {
        popEnd = &list->end;
        otherEnd = &list->start;
    }
    
    assert( *popEnd );
    
    XORListNode * newPopEnd = (*popEnd)->XORPointer;
    if( newPopEnd ) {
        newPopEnd->XORPointer = XORPointers( newPopEnd->XORPointer, *popEnd );        
    } else {
        *otherEnd = NULL;
    }
    
    if( data )
        *data = (*popEnd)->data;
    
    free( *popEnd );
    *popEnd = newPopEnd;
    
    list->size--;
    return true;
}

inline void _XORList_push( XORList * list, void * data, XORListIterLocation location )
{
    XORListNode ** pushEnd;
    XORListNode ** otherEnd;
    if( location == XORLISTITER_LOCATION_FRONT ) {
        pushEnd = &list->start;
        otherEnd = &list->end;
    } else {
        pushEnd = &list->end;
        otherEnd = &list->start;
    }
    
    XORListNode * newListNode = (XORListNode *)malloc(sizeof(XORListNode));
    newListNode->data = data;
    
    if( list->start ) {
        (*pushEnd)->XORPointer = XORPointers( (*pushEnd)->XORPointer, newListNode );        
    } else {
        *otherEnd = newListNode;
    }
    newListNode->XORPointer = *pushEnd;
    *pushEnd = newListNode;
    list->size++;
}

bool XORList_popFront( XORList * list, void ** data )
{
    return _XORList_pop(list, data, XORLISTITER_LOCATION_FRONT);
}

bool XORList_popBack( XORList * list, void ** data )
{
    return _XORList_pop(list, data, XORLISTITER_LOCATION_BACK);
}

void XORList_pushFront( XORList * list, void * data )
{
    return _XORList_push(list, data, XORLISTITER_LOCATION_FRONT);
}

void XORList_pushBack( XORList * list, void * data )
{
    return _XORList_push(list, data, XORLISTITER_LOCATION_BACK);
}

//init
inline XORListIter * _XORListIter_new( const XORList * list, XORListIterLocation location )
{
    XORListIter * newListIter = (XORListIter *)malloc(sizeof(XORListIter));
    newListIter->list = list;
    newListIter->current = NULL;
    
    switch( location ) {
        case XORLISTITER_LOCATION_FRONT:
            newListIter->next = list->start;
            newListIter->direction = XORLISTITER_DIRECTION_FORWARD;
            break;
        case XORLISTITER_LOCATION_BACK:
            newListIter->next = list->end;
            newListIter->direction = XORLISTITER_DIRECTION_BACKWARD;
            break;
    }
    
    return newListIter;
}

XORListIter * XORListIter_newFromFront( const XORList * list )
{
    return _XORListIter_new( list, XORLISTITER_LOCATION_FRONT );
}

XORListIter * XORListIter_newFromBack( const XORList * list )
{
    return _XORListIter_new( list, XORLISTITER_LOCATION_BACK );
}

void XORListIter_delete( XORListIter * iter )
{
    free( iter );
}

//seeking

inline void _XORListIter_flipDirection( XORListIter * iter )
{
    assert( iter->current );
    XORListNode * next = XORPointers( iter->current->XORPointer, iter->next );
    iter->next = next;
    iter->direction = iter->direction == XORLISTITER_DIRECTION_FORWARD
        ? XORLISTITER_DIRECTION_BACKWARD
        : XORLISTITER_DIRECTION_FORWARD;
}

inline bool _XORListIter_seak( XORListIter * iter, void ** data, XORListIterLocation direction )
{
    if( iter->direction != direction )
        _XORListIter_flipDirection( iter );
    
    if( ! iter->next ) {
        iter->next = iter->current;
        iter->current = NULL;
        iter->direction = iter->direction == XORLISTITER_DIRECTION_FORWARD
            ? XORLISTITER_DIRECTION_BACKWARD
            : XORLISTITER_DIRECTION_FORWARD;
        
        return false;
    }
    
    XORListNode * nextListNode = XORPointers( iter->current, iter->next->XORPointer );
    XORListNode * currentListNode = iter->next;
    iter->current = currentListNode;
    iter->next = nextListNode;
    
    if( data )
        *data = iter->current->data;
    
    return true;
}

bool XORListIter_seekForward( XORListIter * iter, void ** data )
{
    return _XORListIter_seak( iter, data, XORLISTITER_DIRECTION_FORWARD );    
}

bool XORListIter_seekBackward( XORListIter * iter, void ** data )
{
    return _XORListIter_seak( iter, data, XORLISTITER_DIRECTION_BACKWARD );    
}

bool XORListIter_getCurrentData( XORListIter * iter, void ** data )
{
    if( ! iter->current )
        return false;
    
    assert( data );
    *data = iter->current->data;
    
    return true;
}
