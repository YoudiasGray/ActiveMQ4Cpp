/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "DoubleArrayBufferTest.h"
#include <decaf/lang/Integer.h>
#include <decaf/lang/Double.h>
#include <decaf/lang/Float.h>

using namespace std;
using namespace decaf;
using namespace decaf::nio;
using namespace decaf::internal::nio;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

////////////////////////////////////////////////////////////////////////////////
const int DoubleArrayBufferTest::testData1Size = 100;
const int DoubleArrayBufferTest::SMALL_TEST_LENGTH = 5;
const int DoubleArrayBufferTest::BUFFER_LENGTH = 250;

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::test() {

    // Check that we have setup the array and our initial assumptions on state
    // are correct.  This is the first test run.
    CPPUNIT_ASSERT( testBuffer1 != NULL );
    CPPUNIT_ASSERT( testBuffer1->capacity() == testData1Size );
    CPPUNIT_ASSERT( testBuffer1->hasRemaining() == true );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == false );
    CPPUNIT_ASSERT( testBuffer1->toString() != "" );
    CPPUNIT_ASSERT( testBuffer1->hasArray() == true );
    CPPUNIT_ASSERT( testBuffer1->array() != NULL );
    CPPUNIT_ASSERT( testBuffer1->arrayOffset() == 0 );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testArray() {

    double* array = testBuffer1->array();

    testBuffer1->put( 0, 10.0 );
    CPPUNIT_ASSERT( array[0] == 10.0 );

    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );

    loadTestData1( array, testBuffer1->arrayOffset(), testBuffer1->capacity() );
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity()) ;

    loadTestData2( array, testBuffer1->arrayOffset(), testBuffer1->capacity());
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );

    loadTestData1( testBuffer1 );
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );

    loadTestData2( testBuffer1 );
    assertContentEquals(
        testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity() );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testArrayOffset() {

    double* array = testBuffer1->array();

    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData1(array, testBuffer1->arrayOffset(), testBuffer1->capacity());
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData2(array, testBuffer1->arrayOffset(), testBuffer1->capacity());
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData1(testBuffer1);
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());

    loadTestData2(testBuffer1);
    assertContentEquals(testBuffer1, array, testBuffer1->arrayOffset(), testBuffer1->capacity());
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testReadOnlyArray() {

    DoubleBuffer* readOnly = testBuffer1->asReadOnlyBuffer();

    CPPUNIT_ASSERT( readOnly != NULL );
    CPPUNIT_ASSERT( readOnly->isReadOnly() == true );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw UnsupportedOperationException",
        readOnly->array(),
        UnsupportedOperationException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw UnsupportedOperationException",
        readOnly->arrayOffset(),
        UnsupportedOperationException );

    delete readOnly;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testAsReadOnlyBuffer() {

    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position( testBuffer1->limit() );

    // readonly's contents should be the same as testBuffer1
    DoubleBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    CPPUNIT_ASSERT( testBuffer1 != readOnly );
    CPPUNIT_ASSERT( readOnly->isReadOnly() );
    CPPUNIT_ASSERT( testBuffer1->position() == readOnly->position() );
    CPPUNIT_ASSERT( testBuffer1->limit() == readOnly->limit() );

    for( int i = 0; i < testBuffer1->capacity(); ++i ) {
        CPPUNIT_ASSERT( testBuffer1->get( i ) == readOnly->get( i ) );
    }

    // readOnly's position, mark, and limit should be independent to testBuffer1
    readOnly->reset();
    CPPUNIT_ASSERT( readOnly->position() == 0 );
    readOnly->clear();
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->limit() );
    testBuffer1->reset();
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    delete readOnly;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testCompact() {

    loadTestData1( testBuffer1 );

    // case: buffer is full
    testBuffer1->clear();
    testBuffer1->mark();

    DoubleBuffer& ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    assertContentLikeTestData1( testBuffer1, 0, 0.0, testBuffer1->capacity() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );

    // case: buffer is empty
    testBuffer1->position(0);
    testBuffer1->limit(0);
    testBuffer1->mark();
    ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1 );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    assertContentLikeTestData1(testBuffer1, 0, 0.0, testBuffer1->capacity());

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );

    // case: normal
    CPPUNIT_ASSERT( testBuffer1->capacity() > 5 );

    testBuffer1->position(1);
    testBuffer1->limit(5);
    testBuffer1->mark();
    ret = testBuffer1->compact();
    CPPUNIT_ASSERT( &ret == testBuffer1);
    CPPUNIT_ASSERT( testBuffer1->position() == 4 );
    CPPUNIT_ASSERT( testBuffer1->limit() == testBuffer1->capacity() );

    assertContentLikeTestData1(testBuffer1, 0, 1.0, 4);

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        testBuffer1->reset(),
        InvalidMarkException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testCompareTo() {

    DoubleBuffer* other = DoubleBuffer::allocate( testBuffer1->capacity() );

    loadTestData1(testBuffer1);
    loadTestData1(other);

    CPPUNIT_ASSERT( 0 == testBuffer1->compareTo( *other ) );
    CPPUNIT_ASSERT( 0 == other->compareTo( *testBuffer1 ) );
    testBuffer1->position(1);
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) > 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) < 0 );
    other->position( 2 );
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) < 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) > 0 );
    testBuffer1->position(2);
    other->limit(5);
    CPPUNIT_ASSERT( testBuffer1->compareTo( *other ) > 0 );
    CPPUNIT_ASSERT( other->compareTo( *testBuffer1 ) < 0 );

    std::vector<double> array1( 1, Double::NaN );
    std::vector<double> array2( 1, Double::NaN );
    std::vector<double> array3( 1, 42.0 );

    DoubleBuffer* dbuffer1 = DoubleBuffer::wrap( array1 );
    DoubleBuffer* dbuffer2 = DoubleBuffer::wrap( array2 );
    DoubleBuffer* dbuffer3 = DoubleBuffer::wrap( array3 );

    CPPUNIT_ASSERT_MESSAGE(
        "Failed equal comparison with NaN entry",
        dbuffer1->compareTo( *dbuffer2 ) );
    CPPUNIT_ASSERT_MESSAGE(
        "Failed greater than comparison with NaN entry",
        dbuffer3->compareTo( *dbuffer1 ) );
    CPPUNIT_ASSERT_MESSAGE(
        "Failed greater than comparison with NaN entry",
        dbuffer1->compareTo( *dbuffer3 ) );

    delete other;
    delete dbuffer1;
    delete dbuffer2;
    delete dbuffer3;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testDuplicate() {
    testBuffer1->clear();
    testBuffer1->mark();
    testBuffer1->position(testBuffer1->limit());

    // duplicate's contents should be the same as testBuffer1
    DoubleBuffer* duplicate = testBuffer1->duplicate();
    CPPUNIT_ASSERT( testBuffer1 != duplicate );
    CPPUNIT_ASSERT( testBuffer1->position() == duplicate->position() );
    CPPUNIT_ASSERT( testBuffer1->limit() == duplicate->limit() );
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == duplicate->isReadOnly() );
    assertContentEquals( testBuffer1, duplicate );

    // duplicate's position, mark, and limit should be independent to testBuffer1
    duplicate->reset();
    CPPUNIT_ASSERT( duplicate->position() == 0 );
    duplicate->clear();
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->limit() );
    testBuffer1->reset();
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    delete duplicate;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testEquals() {

    // equal to self
    CPPUNIT_ASSERT( testBuffer1->equals( *testBuffer1 ) );
    DoubleBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    CPPUNIT_ASSERT( testBuffer1->equals( *readOnly ) );
    DoubleBuffer* duplicate = testBuffer1->duplicate();
    CPPUNIT_ASSERT( testBuffer1->equals( *duplicate ) );

    CPPUNIT_ASSERT( testBuffer1->capacity() > 5 );

    testBuffer1->limit( testBuffer1->capacity() ).position(0);
    readOnly->limit( readOnly->capacity() ).position( 1 );
    CPPUNIT_ASSERT( !testBuffer1->equals( *readOnly ) );

    testBuffer1->limit( testBuffer1->capacity() - 1).position(0);
    duplicate->limit( duplicate->capacity() ).position( 0 );
    CPPUNIT_ASSERT( !testBuffer1->equals( *duplicate ) );

    delete readOnly;
    delete duplicate;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testGet() {

    testBuffer1->clear();
    for( int i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CPPUNIT_ASSERT( testBuffer1->get() == testBuffer1->get(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get(),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testGetDoubleArray() {

    std::vector<double> array(1);
    testBuffer1->clear();

    for( int i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        DoubleBuffer& ret = testBuffer1->get( array );
        CPPUNIT_ASSERT( array[0] == testBuffer1->get(i) );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get( array ),
        BufferUnderflowException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testGetDoubleArray2() {

    testBuffer1->clear();
    double* array1 = new double[testBuffer1->capacity()];
    double* array2 = new double[testBuffer1->capacity() + 1];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferUnderflowException",
        testBuffer1->get( array2, testBuffer1->capacity() + 1, 0, testBuffer1->capacity() + 1 ),
        BufferUnderflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->get( array1, testBuffer1->capacity(), 10, 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( array1, testBuffer1->capacity(), -1, testBuffer1->capacity() ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( array1, testBuffer1->capacity(), testBuffer1->capacity() + 1, 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( array1, testBuffer1->capacity(), 2, -1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( array1, testBuffer1->capacity(), 2, testBuffer1->capacity() ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( array1, testBuffer1->capacity(), 1, Integer::MAX_VALUE ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( array1, testBuffer1->capacity(), Integer::MAX_VALUE, 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw NullPointerException",
        testBuffer1->get( NULL, testBuffer1->capacity(), 1, Integer::MAX_VALUE ),
        NullPointerException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->clear();
    DoubleBuffer& ret = testBuffer1->get( array1, testBuffer1->capacity(), 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    assertContentEquals( testBuffer1, array1, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete [] array1;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testGet2() {

    testBuffer1->clear();
    for( int i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        CPPUNIT_ASSERT( testBuffer1->get() == testBuffer1->get(i) );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( -1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->get( testBuffer1->limit() ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testHasArray() {
    CPPUNIT_ASSERT( testBuffer1->hasArray() );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testPutDouble() {

    testBuffer1->clear();

    for( int i = 0; i < testBuffer1->capacity(); i++) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        DoubleBuffer& ret = testBuffer1->put( (double)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (double)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( 0 ),
        BufferOverflowException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testPutDoubleArray() {

    double* array = new double[1];

    testBuffer1->clear();
    for( int i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == i );
        array[0] = (double) i;
        DoubleBuffer& ret = testBuffer1->put( array, 1, 0, 1 );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (double)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( array, 1, 0, 1 ),
        BufferOverflowException );

    delete [] array;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testPutDoubleArray2() {

    testBuffer1->clear();
    double* array1 = new double[ testBuffer1->capacity() ];
    double* array2 = new double[ testBuffer1->capacity() + 1 ];

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( array2, testBuffer1->capacity() + 1, 0, testBuffer1->capacity() + 1 ),
        BufferOverflowException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    testBuffer1->put( array1, testBuffer1->capacity(), testBuffer1->capacity() + 1, 0 );
    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( array1, testBuffer1->capacity(), -1, testBuffer1->capacity() ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( array1, testBuffer1->capacity(), testBuffer1->capacity() + 1, 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( array1, testBuffer1->capacity(), 2, -1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( array1, testBuffer1->capacity(), 2, testBuffer1->capacity() ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( array1, testBuffer1->capacity(), 1, Integer::MAX_VALUE ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( array1, testBuffer1->capacity(), Integer::MAX_VALUE, 1 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw NullPointerException",
        testBuffer1->put( NULL, testBuffer1->capacity(), 1, Integer::MAX_VALUE ),
        NullPointerException );

    CPPUNIT_ASSERT( testBuffer1->position() == 0 );

    loadTestData2( array1, 0, testBuffer1->capacity() );
    DoubleBuffer& ret = testBuffer1->put( array1, testBuffer1->capacity(), 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    assertContentEquals( testBuffer1, array1, 0, testBuffer1->capacity() );
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete [] array1;
    delete [] array2;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testPutDoubleBuffer() {

    DoubleBuffer* other = DoubleBuffer::allocate( testBuffer1->capacity() );
    DoubleBuffer* other1 = DoubleBuffer::allocate( testBuffer1->capacity() + 1 );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IllegalArgumentException",
        testBuffer1->put( *testBuffer1 ),
        IllegalArgumentException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw BufferOverflowException",
        testBuffer1->put( *other1 ),
        BufferOverflowException );

    loadTestData2(other);
    other->clear();
    testBuffer1->clear();
    DoubleBuffer& ret = testBuffer1->put( *other );

    CPPUNIT_ASSERT( other->position() == other->capacity() );
    CPPUNIT_ASSERT( testBuffer1->position() == testBuffer1->capacity() );
    assertContentEquals( other, testBuffer1 );
    CPPUNIT_ASSERT( &ret == testBuffer1 );

    delete other;
    delete other1;
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testGetWithIndex() {

    testBuffer1->clear();

    for( int i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == 0 );
        DoubleBuffer& ret = testBuffer1->put( i, (double)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == (double)i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( -1, 0 ),
        IndexOutOfBoundsException );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw IndexOutOfBoundsException",
        testBuffer1->put( testBuffer1->limit(), 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testPutIndexed() {

    DoubleBuffer* readOnly = testBuffer1->asReadOnlyBuffer();
    readOnly->clear();
    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a ReadOnlyBufferException",
        readOnly->put( 0, 0 ),
        ReadOnlyBufferException );
    delete readOnly;

    testBuffer1->clear();

    for( int i = 0; i < testBuffer1->capacity(); i++ ) {
        CPPUNIT_ASSERT( testBuffer1->position() == 0 );
        DoubleBuffer& ret = testBuffer1->put( i, (double)i );
        CPPUNIT_ASSERT( testBuffer1->get(i) == i );
        CPPUNIT_ASSERT( &ret == testBuffer1 );
    }

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw a IndexOutOfBoundsException",
        testBuffer1->put( testBuffer1->limit(), 0 ),
        IndexOutOfBoundsException );
}

////////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testSlice() {

    CPPUNIT_ASSERT( testBuffer1->capacity() > 5 );
    testBuffer1->position(1);
    testBuffer1->limit(testBuffer1->capacity() - 1);

    DoubleBuffer* slice = testBuffer1->slice();
    CPPUNIT_ASSERT( testBuffer1->isReadOnly() == slice->isReadOnly() );
    CPPUNIT_ASSERT( slice->position() == 0 );
    CPPUNIT_ASSERT( slice->limit() == testBuffer1->remaining() );
    CPPUNIT_ASSERT( slice->capacity() == testBuffer1->remaining() );

    CPPUNIT_ASSERT_THROW_MESSAGE(
        "Should throw InvalidMarkException",
        slice->reset(),
        InvalidMarkException );

    // slice share the same content with testBuffer1
    // FIXME:
    loadTestData1(slice);
    assertContentLikeTestData1(testBuffer1, 1, 0, slice->capacity());
    testBuffer1->put( 2, 500 );
    CPPUNIT_ASSERT( slice->get(1) == 500 );

    delete slice;
}

///////////////////////////////////////////////////////////////////////////////
void DoubleArrayBufferTest::testToString() {

    std::string str = testBuffer1->toString();
    CPPUNIT_ASSERT( str.find("Double") != string::npos );
    CPPUNIT_ASSERT( str.find( Integer::toString( (int)testBuffer1->position() ) ) != string::npos );
    CPPUNIT_ASSERT( str.find( Integer::toString( (int)testBuffer1->limit() ) ) != string::npos );
    CPPUNIT_ASSERT( str.find( Integer::toString( (int)testBuffer1->capacity() ) ) != string::npos );
}
