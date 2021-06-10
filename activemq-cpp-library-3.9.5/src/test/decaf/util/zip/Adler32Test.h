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

#ifndef _DECAF_UTIL_ZIP_ADLER32TEST_H_
#define _DECAF_UTIL_ZIP_ADLER32TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace decaf {
namespace util {
namespace zip {

    class Adler32Test : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( Adler32Test );
        CPPUNIT_TEST( testConstructor );
        CPPUNIT_TEST( testGetValue );
        CPPUNIT_TEST( testReset );
        CPPUNIT_TEST( testUpdateI );
        CPPUNIT_TEST( testUpdateArray );
        CPPUNIT_TEST( testUpdateArrayIndexed );
        CPPUNIT_TEST_SUITE_END();

    public:

        Adler32Test();
        virtual ~Adler32Test();

        void testConstructor();
        void testGetValue();
        void testReset();
        void testUpdateI();
        void testUpdateArray();
        void testUpdateArrayIndexed();

    };

}}}

#endif /* _DECAF_UTIL_ZIP_ADLER32TEST_H_ */
