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

#include "TransactionTest.h"

#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/util/CMSListener.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <stdexcept>

using namespace std;
using namespace cms;
using namespace activemq;
using namespace activemq::core;
using namespace activemq::test;
using namespace activemq::util;
using namespace activemq::exceptions;

////////////////////////////////////////////////////////////////////////////////
TransactionTest::TransactionTest() {
}

////////////////////////////////////////////////////////////////////////////////
TransactionTest::~TransactionTest() {
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendReceiveTransactedBatches() {

    // Create CMS Object for Comms
    cms::Session* session = cmsProvider->getSession();
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();

    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

    for (int j = 0; j < batchCount - 8; j++) {

        auto_ptr<TextMessage> message(session->createTextMessage("Batch Message"));

        for (int i = 0; i < batchSize; i++) {
            CPPUNIT_ASSERT_NO_THROW_MESSAGE("Send should not throw an exception here.", producer->send(message.get()));
        }

        CPPUNIT_ASSERT_NO_THROW_MESSAGE("Session Commit should not throw an exception here:", session->commit());

        for (int i = 0; i < batchSize; i++) {
            CPPUNIT_ASSERT_NO_THROW_MESSAGE("Receive Shouldn't throw a Message here:",
                message.reset(dynamic_cast<TextMessage*>(consumer->receive(1000 * 5))));

            CPPUNIT_ASSERT_MESSAGE("Failed to receive all messages in batch", message.get() != NULL);
            CPPUNIT_ASSERT(string("Batch Message") == message->getText());
        }

        CPPUNIT_ASSERT_NO_THROW_MESSAGE("Session Commit should not throw an exception here:", session->commit());
    }
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendRollback() {

    // Create CMS Object for Comms
    cms::Session* session = cmsProvider->getSession();
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();

    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

    auto_ptr<TextMessage> outbound1(session->createTextMessage("First Message"));
    auto_ptr<TextMessage> outbound2(session->createTextMessage("Second Message"));

    // sends a message
    producer->send(outbound1.get());
    session->commit();

    // sends a message that gets rollbacked
    auto_ptr<Message> rollback(session->createTextMessage("I'm going to get rolled back."));
    producer->send(rollback.get());
    session->rollback();

    // sends a message
    producer->send(outbound2.get());
    session->commit();

    // receives the first message
    auto_ptr<TextMessage> inbound1(dynamic_cast<TextMessage*>(consumer->receive(1500)));

    // receives the second message
    auto_ptr<TextMessage> inbound2(dynamic_cast<TextMessage*>(consumer->receive(4000)));

    // validates that the rollbacked was not consumed
    session->commit();

    CPPUNIT_ASSERT(outbound1->getText() == inbound1->getText());
    CPPUNIT_ASSERT(outbound2->getText() == inbound2->getText());
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendRollbackCommitRollback() {

    // Create CMS Object for Comms
    cms::Session* session = cmsProvider->getSession();
    cms::MessageConsumer* consumer = cmsProvider->getConsumer();
    cms::MessageProducer* producer = cmsProvider->getProducer();

    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

    auto_ptr<TextMessage> outbound1(session->createTextMessage("First Message"));
    auto_ptr<TextMessage> outbound2(session->createTextMessage("Second Message"));

    // sends them and then rolls back.
    producer->send(outbound1.get());
    producer->send(outbound2.get());
    session->rollback();

    // Send one and commit.
    producer->send(outbound1.get());
    session->commit();

    // receives the first message
    auto_ptr<TextMessage> inbound1(dynamic_cast<TextMessage*>(consumer->receive(1500)));

    CPPUNIT_ASSERT(NULL == consumer->receive( 1500 ));
    CPPUNIT_ASSERT(outbound1->getText() == inbound1->getText());

    session->rollback();

    inbound1.reset(dynamic_cast<TextMessage*>(consumer->receive(1500)));

    CPPUNIT_ASSERT(NULL == consumer->receive( 1500 ));
    CPPUNIT_ASSERT(outbound1->getText() == inbound1->getText());

    // validates that the rollbacked was not consumed
    session->commit();
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSendSessionClose() {

    Pointer<ActiveMQConnectionFactory> connectionFactory(
        new ActiveMQConnectionFactory(getBrokerURL()));

    Pointer<Connection> connection(connectionFactory->createConnection());
    Pointer<ActiveMQConnection> amqConnection = connection.dynamicCast<ActiveMQConnection>();

    connection->start();
    Pointer<Session> session(connection->createSession(Session::SESSION_TRANSACTED));
    Pointer<Queue> destination(session->createQueue("testSendSessionClose"));

    // Create the messages used for this test
    auto_ptr<TextMessage> outbound1(session->createTextMessage("First Message"));
    auto_ptr<TextMessage> outbound2(session->createTextMessage("Second Message"));

    Pointer<MessageConsumer> consumer(session->createConsumer(destination.get()));
    Pointer<MessageProducer> producer(session->createProducer(destination.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

    // Send Message #1
    producer->send(outbound1.get());
    session->commit();

    // Send a Message but roll it back by closing the session that owns the resources
    auto_ptr<cms::Message> rollback(session->createTextMessage("I'm going to get rolled back."));
    producer->send(outbound2.get());
    session->close();

    session.reset(connection->createSession(Session::SESSION_TRANSACTED));
    destination.reset(session->createQueue("testSendSessionClose"));
    consumer.reset(session->createConsumer(destination.get()));
    producer.reset(session->createProducer(destination.get()));
    producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);

    // Send Message #2
    producer->send(outbound2.get());
    session->commit();

    // receives the first message
    auto_ptr<TextMessage> inbound1(dynamic_cast<TextMessage*>(consumer->receive(1500)));

    // receives the second message
    auto_ptr<TextMessage> inbound2(dynamic_cast<TextMessage*>(consumer->receive(4000)));

    // validates that the rolled back was not consumed
    session->commit();

    CPPUNIT_ASSERT_MESSAGE("Failed to receive first message", inbound1.get() != NULL);
    CPPUNIT_ASSERT_MESSAGE("Failed to receive second message", inbound2.get() != NULL);

    CPPUNIT_ASSERT_MESSAGE("First messages aren't equal", outbound1->getText() == inbound1->getText());
    CPPUNIT_ASSERT_MESSAGE("Second messages aren't equal", outbound2->getText() == inbound2->getText());

    session->close();
    amqConnection->destroyDestination(destination.get());
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testWithTTLSet() {

    cmsProvider->getProducer()->setDeliveryMode(DeliveryMode::PERSISTENT);

    cms::MessageConsumer* consumer = cmsProvider->getConsumer();

    auto_ptr<TextMessage> outbound1(cmsProvider->getSession()->createTextMessage("First Message"));

    const std::size_t NUM_MESSAGES = 50;

    // sends a message
    for (std::size_t i = 0; i < NUM_MESSAGES; ++i) {
        cmsProvider->getProducer()->send(outbound1.get(), cms::DeliveryMode::PERSISTENT,
                cmsProvider->getProducer()->getPriority(), 120 * 1000);
    }

    cmsProvider->getSession()->commit();

    for (std::size_t i = 0; i < NUM_MESSAGES; ++i) {

        // receives the second message
        auto_ptr<TextMessage> inbound1(dynamic_cast<TextMessage*>(consumer->receive(600000)));
        CPPUNIT_ASSERT(inbound1.get() != NULL);
        CPPUNIT_ASSERT(outbound1->getText() == inbound1->getText());
    }

    cmsProvider->getSession()->commit();
}

////////////////////////////////////////////////////////////////////////////////
void TransactionTest::testSessionCommitAfterConsumerClosed() {

    ActiveMQConnectionFactory factory(getBrokerURL());
    auto_ptr<cms::Connection> connection(factory.createConnection());

    {
        auto_ptr<cms::Session> session(connection->createSession(cms::Session::AUTO_ACKNOWLEDGE));
        auto_ptr<cms::Queue> queue(session->createQueue("testSessionCommitAfterConsumerClosed"));
        auto_ptr<cms::MessageProducer> producer(session->createProducer(queue.get()));

        auto_ptr<cms::Message> message(session->createTextMessage("Hello"));
        producer->send(message.get());
        producer->close();
        session->close();
    }

    auto_ptr<cms::Session> session(connection->createSession(cms::Session::SESSION_TRANSACTED));
    auto_ptr<cms::Queue> queue(session->createQueue("testSessionCommitAfterConsumerClosed"));
    auto_ptr<cms::MessageConsumer> consumer(session->createConsumer(queue.get()));

    connection->start();

    auto_ptr<cms::Message> message(consumer->receive(5000));
    CPPUNIT_ASSERT(message.get() != NULL);

    consumer->close();
    session->commit();
}
