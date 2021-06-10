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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIREDURABLETESTER_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIREDURABLETESTER_H_

#include <activemq/test/DurableTest.h>

namespace activemq{
namespace test{
namespace openwire{

    class OpenwireDurableTest : public DurableTest {

        CPPUNIT_TEST_SUITE( OpenwireDurableTest );
        CPPUNIT_TEST( testDurableConsumer );
        CPPUNIT_TEST_SUITE_END();

    public:

        OpenwireDurableTest();
        virtual ~OpenwireDurableTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }

        virtual std::string getSubscriptionName() const;

    };

}}}

#endif /*_ACTIVEMQ_TEST_OPENWIRE_OPENWIREDURABLETESTER_H_*/
