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

#ifndef _ACTIVEMQ_COMMANDS_MESSAGEDISPATCH_H_
#define _ACTIVEMQ_COMMANDS_MESSAGEDISPATCH_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/ConsumerId.h>
#include <activemq/commands/Message.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Exception.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq {
namespace commands {

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for MessageDispatch
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API MessageDispatch : public BaseCommand {
    protected:

        Pointer<ConsumerId> consumerId;
        Pointer<ActiveMQDestination> destination;
        Pointer<Message> message;
        int redeliveryCounter;

    public:

        const static unsigned char ID_MESSAGEDISPATCH = 21;

    private:

        decaf::lang::Exception rollbackCause;

    private:

        MessageDispatch(const MessageDispatch&);
        MessageDispatch& operator= (const MessageDispatch&);

    public:

        MessageDispatch();

        virtual ~MessageDispatch();

        virtual unsigned char getDataStructureType() const;

        virtual MessageDispatch* cloneDataStructure() const;

        virtual void copyDataStructure(const DataStructure* src);

        virtual std::string toString() const;

        virtual bool equals(const DataStructure* value) const;

        void setRollbackCause(const decaf::lang::Exception& cause);

        decaf::lang::Exception getRollbackCause() const;

        virtual const Pointer<ConsumerId>& getConsumerId() const;
        virtual Pointer<ConsumerId>& getConsumerId();
        virtual void setConsumerId(const Pointer<ConsumerId>& consumerId);

        virtual const Pointer<ActiveMQDestination>& getDestination() const;
        virtual Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination(const Pointer<ActiveMQDestination>& destination);

        virtual const Pointer<Message>& getMessage() const;
        virtual Pointer<Message>& getMessage();
        virtual void setMessage(const Pointer<Message>& message);

        virtual int getRedeliveryCounter() const;
        virtual void setRedeliveryCounter(int redeliveryCounter);

        /**
         * @return an answer of true to the isMessageDispatch() query.
         */
        virtual bool isMessageDispatch() const {
            return true;
        }

        virtual Pointer<Command> visit(activemq::state::CommandVisitor* visitor);

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_MESSAGEDISPATCH_H_*/
