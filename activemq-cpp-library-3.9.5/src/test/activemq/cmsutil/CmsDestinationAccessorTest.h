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

#ifndef _ACTIVEMQ_CMSUTIL_CMSDESTINATIONACCESSORTEST_H_
#define _ACTIVEMQ_CMSUTIL_CMSDESTINATIONACCESSORTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <activemq/cmsutil/CmsDestinationAccessor.h>

namespace activemq{
namespace cmsutil{

    class DummyConnectionFactory;

    class CmsDestinationAccessorTest : public CppUnit::TestFixture {

        CPPUNIT_TEST_SUITE( CmsDestinationAccessorTest );
        CPPUNIT_TEST( test );
        CPPUNIT_TEST_SUITE_END();

        class MyAccessor : public CmsDestinationAccessor {
        public:

            virtual ~MyAccessor() {
                try {
                    destroy();
                } catch( ... ) {
                }
            }

            virtual cms::Connection* createConnection() {
                return CmsDestinationAccessor::createConnection();
            }

            virtual cms::Session* createSession(cms::Connection* con) {
                return CmsDestinationAccessor::createSession(con);
            }
            virtual cms::Destination* resolveDestinationName( cms::Session* session,
                                                              const std::string& destName ) {
                return CmsDestinationAccessor::resolveDestinationName(session,destName);
            }
            virtual void init() {
                CmsDestinationAccessor::init();
            }
            virtual void destroy() {
                CmsDestinationAccessor::destroy();
            }
        };

        MyAccessor* accessor;
        DummyConnectionFactory* cf;

    private:

        CmsDestinationAccessorTest(const CmsDestinationAccessorTest&);
        CmsDestinationAccessorTest& operator= (const CmsDestinationAccessorTest&);

    public:

        CmsDestinationAccessorTest() : accessor(), cf() {}
        virtual ~CmsDestinationAccessorTest() {}

        virtual void setUp();
        virtual void tearDown();

        void test();
    };

}}

#endif /*_ACTIVEMQ_CMSUTIL_CMSDESTINATIONACCESSORTEST_H_*/
