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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIREMAPMESSAGETEST_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIREMAPMESSAGETEST_H_

#include <activemq/test/MapMessageTest.h>

namespace activemq {
namespace test {
namespace openwire {

    class OpenwireMapMessageTest : public MapMessageTest {
    private:

        CPPUNIT_TEST_SUITE( OpenwireMapMessageTest );
        CPPUNIT_TEST( testEmptyMapSendReceive );
        CPPUNIT_TEST( testMapWithEmptyStringValue );
        CPPUNIT_TEST( testMapSetEmptyBytesVector );
        CPPUNIT_TEST( testMapWithSingleCharEntry );
        CPPUNIT_TEST( testMapWithCharAndStringEntry );
        CPPUNIT_TEST_SUITE_END();

    public:

        OpenwireMapMessageTest();
        virtual ~OpenwireMapMessageTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }
    };

}}}

#endif /* _ACTIVEMQ_TEST_OPENWIRE_OPENWIREMAPMESSAGETEST_H_ */
