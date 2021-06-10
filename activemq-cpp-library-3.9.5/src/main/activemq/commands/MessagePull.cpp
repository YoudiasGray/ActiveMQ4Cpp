/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <activemq/commands/MessagePull.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/state/CommandVisitor.h>
#include <decaf/lang/exceptions/NullPointerException.h>

using namespace std;
using namespace activemq;
using namespace activemq::exceptions;
using namespace activemq::commands;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;

/*
 *
 *  Command code for OpenWire format for MessagePull
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
MessagePull::MessagePull() :
    BaseCommand(), consumerId(NULL), destination(NULL), timeout(0), correlationId(""), messageId(NULL) {

}

////////////////////////////////////////////////////////////////////////////////
MessagePull::~MessagePull() {
}

////////////////////////////////////////////////////////////////////////////////
MessagePull* MessagePull::cloneDataStructure() const {
    std::auto_ptr<MessagePull> messagePull(new MessagePull());

    // Copy the data from the base class or classes
    messagePull->copyDataStructure(this);

    return messagePull.release();
}

////////////////////////////////////////////////////////////////////////////////
void MessagePull::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const MessagePull* srcPtr = dynamic_cast<const MessagePull*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "MessagePull::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setConsumerId(srcPtr->getConsumerId());
    this->setDestination(srcPtr->getDestination());
    this->setTimeout(srcPtr->getTimeout());
    this->setCorrelationId(srcPtr->getCorrelationId());
    this->setMessageId(srcPtr->getMessageId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char MessagePull::getDataStructureType() const {
    return MessagePull::ID_MESSAGEPULL;
}

////////////////////////////////////////////////////////////////////////////////
std::string MessagePull::toString() const {

    ostringstream stream;

    stream << "MessagePull { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "ConsumerId = ";
    if (this->getConsumerId() != NULL) {
        stream << this->getConsumerId()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Destination = ";
    if (this->getDestination() != NULL) {
        stream << this->getDestination()->toString();
    } else {
        stream << "NULL";
    }
    stream << ", ";
    stream << "Timeout = " << this->getTimeout();
    stream << ", ";
    stream << "CorrelationId = " << this->getCorrelationId();
    stream << ", ";
    stream << "MessageId = ";
    if (this->getMessageId() != NULL) {
        stream << this->getMessageId()->toString();
    } else {
        stream << "NULL";
    }
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool MessagePull::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const MessagePull* valuePtr = dynamic_cast<const MessagePull*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getConsumerId() != NULL) {
        if (!this->getConsumerId()->equals(valuePtr->getConsumerId().get())) {
            return false;
        }
    } else if (valuePtr->getConsumerId() != NULL) {
        return false;
    }
    if (this->getDestination() != NULL) {
        if (!this->getDestination()->equals(valuePtr->getDestination().get())) {
            return false;
        }
    } else if (valuePtr->getDestination() != NULL) {
        return false;
    }
    if (this->getTimeout() != valuePtr->getTimeout()) {
        return false;
    }
    if (this->getCorrelationId() != valuePtr->getCorrelationId()) {
        return false;
    }
    if (this->getMessageId() != NULL) {
        if (!this->getMessageId()->equals(valuePtr->getMessageId().get())) {
            return false;
        }
    } else if (valuePtr->getMessageId() != NULL) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ConsumerId>& MessagePull::getConsumerId() const {
    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ConsumerId>& MessagePull::getConsumerId() {
    return consumerId;
}

////////////////////////////////////////////////////////////////////////////////
void MessagePull::setConsumerId(const decaf::lang::Pointer<ConsumerId>& consumerId) {
    this->consumerId = consumerId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<ActiveMQDestination>& MessagePull::getDestination() const {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<ActiveMQDestination>& MessagePull::getDestination() {
    return destination;
}

////////////////////////////////////////////////////////////////////////////////
void MessagePull::setDestination(const decaf::lang::Pointer<ActiveMQDestination>& destination) {
    this->destination = destination;
}

////////////////////////////////////////////////////////////////////////////////
long long MessagePull::getTimeout() const {
    return timeout;
}

////////////////////////////////////////////////////////////////////////////////
void MessagePull::setTimeout(long long timeout) {
    this->timeout = timeout;
}

////////////////////////////////////////////////////////////////////////////////
const std::string& MessagePull::getCorrelationId() const {
    return correlationId;
}

////////////////////////////////////////////////////////////////////////////////
std::string& MessagePull::getCorrelationId() {
    return correlationId;
}

////////////////////////////////////////////////////////////////////////////////
void MessagePull::setCorrelationId(const std::string& correlationId) {
    this->correlationId = correlationId;
}

////////////////////////////////////////////////////////////////////////////////
const decaf::lang::Pointer<MessageId>& MessagePull::getMessageId() const {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<MessageId>& MessagePull::getMessageId() {
    return messageId;
}

////////////////////////////////////////////////////////////////////////////////
void MessagePull::setMessageId(const decaf::lang::Pointer<MessageId>& messageId) {
    this->messageId = messageId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> MessagePull::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processMessagePull(this);
}
