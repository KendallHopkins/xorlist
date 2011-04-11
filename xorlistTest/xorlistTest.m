//
//  xorlistTest.m
//  xorlistTest
//
//  Created by Kendall Hopkins on 3/9/11.
//  Copyright 2011 SoftwareElves. All rights reserved.
//

#import "xorlistTest.h"

#import "xorlist.h"

@implementation xorlistTest

- (void)setUp
{
    [super setUp];
}

- (void)tearDown
{
    // Tear-down code here.
    
    [super tearDown];
}

- (void)testStack
{
    XORList * myList = XORList_new();
    XORList_pushFront(myList, (void*)5);
    XORList_pushFront(myList, (void*)4);
    XORList_pushFront(myList, (void*)3);
    XORList_pushFront(myList, (void*)2);
    XORList_pushFront(myList, (void*)1);
    
    void * data;
    STAssertTrue( XORList_popFront(myList, &data), @"must pop value" );
    STAssertEquals( 1, (int)data, @"element should be 1" );
    STAssertTrue( XORList_popFront(myList, &data), @"must pop value" );
    STAssertEquals( 2, (int)data, @"element should be 2" );
    STAssertTrue( XORList_popFront(myList, &data), @"must pop value" );
    STAssertEquals( 3, (int)data, @"element should be 3" );
    STAssertTrue( XORList_popFront(myList, &data), @"must pop value" );
    STAssertEquals( 4, (int)data, @"element should be 4" );
    STAssertTrue( XORList_popFront(myList, &data), @"must pop value" );
    STAssertEquals( 5, (int)data, @"element should be 5" );
    
    XORList_delete(myList);
}

- (void)testQueue
{
    XORList * myList = XORList_new();
    XORList_pushFront(myList, (void*)5);
    XORList_pushFront(myList, (void*)4);
    XORList_pushFront(myList, (void*)3);
    XORList_pushFront(myList, (void*)2);
    XORList_pushFront(myList, (void*)1);
    
    void * data;
    STAssertTrue( XORList_popBack(myList, &data), @"must pop value" );
    STAssertEquals( 5, (int)data, @"element should be 5" );
    STAssertTrue( XORList_popBack(myList, &data), @"must pop value" );
    STAssertEquals( 4, (int)data, @"element should be 4" );
    STAssertTrue( XORList_popBack(myList, &data), @"must pop value" );
    STAssertEquals( 3, (int)data, @"element should be 3" );
    STAssertTrue( XORList_popBack(myList, &data), @"must pop value" );
    STAssertEquals( 2, (int)data, @"element should be 2" );
    STAssertTrue( XORList_popBack(myList, &data), @"must pop value" );
    STAssertEquals( 1, (int)data, @"element should be 1" );
    
    XORList_delete(myList);
}

- (void)testSeek
{
    XORList * myList = XORList_new();
    XORList_pushFront(myList, (void*)5);
    XORList_pushFront(myList, (void*)4);
    XORList_pushFront(myList, (void*)3);
    XORList_pushFront(myList, (void*)2);
    XORList_pushFront(myList, (void*)1);
    
    void * data;
    XORListIter * myListIter = XORListIter_newFromFront( myList );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 1, (int)data, @"element should be 1" );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 2, (int)data, @"element should be 2" );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 3, (int)data, @"element should be 3" );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 4, (int)data, @"element should be 4" );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 5, (int)data, @"element should be 5" );
    
    STAssertFalse( XORListIter_seekForward( myListIter, &data ), @"must NOT get value" );
    
    XORListIter_delete(myListIter);
    XORList_delete(myList);
}

- (void)testComplexSeek
{
    XORList * myList = XORList_new();
    XORList_pushFront(myList, (void*)2);
    XORList_pushFront(myList, (void*)1);
    
    void * data;
    XORListIter * myListIter = XORListIter_newFromFront( myList );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 1, (int)data, @"element should be 1" );
    
    STAssertFalse( XORListIter_seekBackward( myListIter, &data ), @"must NOT get value" );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 1, (int)data, @"element should be 1" );
    
    STAssertTrue( XORListIter_seekForward( myListIter, &data ), @"must get value" );
    STAssertEquals( 2, (int)data, @"element should be 2" );
    
    STAssertTrue( XORListIter_seekBackward( myListIter, &data ), @"must get value" );
    STAssertEquals( 1, (int)data, @"element should be 1" );
    
    STAssertFalse( XORListIter_seekBackward( myListIter, &data ), @"must NOT get value" );
    
    XORListIter_delete(myListIter);
    XORList_delete(myList);
}

#define LARGE_TEST_SIZE 100000

- (void)testLargeStack
{
    XORList * myList = XORList_new();
    
    int i;
    for( i = 0; i < LARGE_TEST_SIZE; i++ ) {
        XORList_pushBack(myList, (void*)i);
    } i--;
    
    void * data;
    for( ; i >= 0; i-- ) {
        STAssertTrue( XORList_popBack(myList, &data), @"must pop value" );
        STAssertEquals( i, (int)data, @"element should be %d", i );
    }
    
    XORList_delete(myList);
}

- (void)testLargeSeek
{
    XORList * myList = XORList_new();
    
    int i;
    for( i = 0; i < LARGE_TEST_SIZE; i++ ) {
        XORList_pushBack(myList, (void*)i);
    }
    
    XORListIter * myListIter = XORListIter_newFromFront( myList );
    
    i = 0;
    void * data;
    while( XORListIter_seekForward( myListIter, &data ) ) {
        STAssertEquals( i, (int)data, @"element should be %d", i );
        i++;
    }
    
    XORListIter_delete(myListIter);
    XORList_delete(myList);
}

@end
