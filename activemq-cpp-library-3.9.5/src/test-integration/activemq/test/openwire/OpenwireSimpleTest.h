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

#ifndef _ACTIVEMQ_TEST_OPENWIRE_OPENWIRESIMPLETEST_H_
#define _ACTIVEMQ_TEST_OPENWIRE_OPENWIRESIMPLETEST_H_

#include <activemq/test/SimpleTest.h>

namespace activemq{
namespace test{
namespace openwire{

    class OpenwireSimpleTest : public SimpleTest {

        CPPUNIT_TEST_SUITE( OpenwireSimpleTest );
        CPPUNIT_TEST( testAutoAck );
        CPPUNIT_TEST( testClientAck );
        CPPUNIT_TEST( testProducerWithNullDestination );
        CPPUNIT_TEST( testProducerSendWithNullDestination );
        CPPUNIT_TEST( testProducerSendToNonDefaultDestination );
        CPPUNIT_TEST( testSyncReceive );
        CPPUNIT_TEST( testSyncReceiveClientAck );
        CPPUNIT_TEST( testMultipleConnections );
        CPPUNIT_TEST( testMultipleSessions );
        CPPUNIT_TEST( testReceiveAlreadyInQueue );
        CPPUNIT_TEST( testBytesMessageSendRecv );
        CPPUNIT_TEST( testQuickCreateAndDestroy );
        CPPUNIT_TEST( testWithZeroConsumerPrefetch );
        CPPUNIT_TEST( testWithZeroConsumerPrefetchAndNoMessage );
        CPPUNIT_TEST( testWithZeroConsumerPrefetch2 );
        CPPUNIT_TEST( testWithZeroConsumerPrefetchAndNoMessage2 );
        CPPUNIT_TEST( testWithZeroConsumerPrefetchAndZeroRedelivery );
        CPPUNIT_TEST( testWithZeroConsumerPrefetchWithInFlightExpiration );
        CPPUNIT_TEST( testMapMessageSendToQueue );
        CPPUNIT_TEST( testMapMessageSendToTopic );
        CPPUNIT_TEST( testDestroyDestination );
        CPPUNIT_TEST( tesstStreamMessage );
        CPPUNIT_TEST( testLibraryInitShutdownInit );
        CPPUNIT_TEST( testBytesMessageSendRecvAsync );
        CPPUNIT_TEST( testMessageIdSetOnSend );
        CPPUNIT_TEST( testReceiveWithSessionSyncDispatch );
        CPPUNIT_TEST_SUITE_END();

    public:

        OpenwireSimpleTest();
        virtual ~OpenwireSimpleTest();

        virtual std::string getBrokerURL() const {
            return activemq::util::IntegrationCommon::getInstance().getOpenwireURL();
        }

        void testWithZeroConsumerPrefetch();
        void testWithZeroConsumerPrefetchAndNoMessage();
        void testWithZeroConsumerPrefetch2();
        void testWithZeroConsumerPrefetchAndNoMessage2();
        void testWithZeroConsumerPrefetchAndZeroRedelivery();
        void testWithZeroConsumerPrefetchWithInFlightExpiration();
        void testMapMessageSendToQueue();
        void testMapMessageSendToTopic();
        void tesstStreamMessage();
        void testDestroyDestination();
        void testMessageIdSetOnSend();
        void testReceiveWithSessionSyncDispatch();

    };

}}}

#endif /*_ACTIVEMQ_TEST_OPENWIRE_OPENWIRESIMPLETEST_H_*/
