//
//  xorlinkedlist.h
//  xorlinkedlist
//
//  Created by Kendall Hopkins on 3/7/11.
//  Copyright 2011 SoftwareElves. All rights reserved.
//

#include <stdlib.h>
#include <stdbool.h>

typedef struct XORList XORList;
typedef struct XORListIter XORListIter;

//init
XORList * XORList_new();
void XORList_delete( XORList * list );
size_t XORList_size( const XORList * list );

//edit
void XORList_mergeAndDelete( XORList * base, XORList * mergedAndDeleted );
bool XORList_popFront( XORList * list, void ** data );
bool XORList_popBack( XORList * list, void ** data );
void XORList_pushFront( XORList * list, void * data );
void XORList_pushBack( XORList * list, void * data );

//init
XORListIter * XORListIter_newFromFront( const XORList * list );
XORListIter * XORListIter_newFromBack( const XORList * list );
void XORListIter_delete( XORListIter * iter );

//seeking
bool XORListIter_seekForward( XORListIter * iter, void ** data );
bool XORListIter_seekBackward( XORListIter * iter, void ** data );
bool XORListIter_getCurrentData( XORListIter * iter, void ** data );
