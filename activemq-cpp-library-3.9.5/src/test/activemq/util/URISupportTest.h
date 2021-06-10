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

#ifndef _ACTIVEMQ_UTIL_URISUPPORTTEST_H_
#define _ACTIVEMQ_UTIL_URISUPPORTTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace activemq{
namespace util{

    class URISupportTest : public CppUnit::TestFixture {

       CPPUNIT_TEST_SUITE( URISupportTest );
       CPPUNIT_TEST( test );
       CPPUNIT_TEST( testURIParseEnv );
       CPPUNIT_TEST( testParseComposite );
       CPPUNIT_TEST( testApplyParameters );
       CPPUNIT_TEST( testCreateWithQuery );
       CPPUNIT_TEST_SUITE_END();

    public:

        URISupportTest() {}
        virtual ~URISupportTest() {}

        void test();
        void testURIParseEnv();
        void testParseComposite();
        void testApplyParameters();
        void testCreateWithQuery();

    };

}}

#endif /*_ACTIVEMQ_UTIL_URISUPPORTTEST_H_*/
