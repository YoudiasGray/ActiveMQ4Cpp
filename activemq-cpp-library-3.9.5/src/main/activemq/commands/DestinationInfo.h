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

#ifndef _ACTIVEMQ_COMMANDS_DESTINATIONINFO_H_
#define _ACTIVEMQ_COMMANDS_DESTINATIONINFO_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/ActiveMQDestination.h>
#include <activemq/commands/BaseCommand.h>
#include <activemq/commands/BrokerId.h>
#include <activemq/commands/ConnectionId.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq {
namespace commands {

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for DestinationInfo
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API DestinationInfo : public BaseCommand {
    protected:

        Pointer<ConnectionId> connectionId;
        Pointer<ActiveMQDestination> destination;
        unsigned char operationType;
        long long timeout;
        std::vector< decaf::lang::Pointer<BrokerId> > brokerPath;

    public:

        const static unsigned char ID_DESTINATIONINFO = 8;

    private:

        DestinationInfo(const DestinationInfo&);
        DestinationInfo& operator= (const DestinationInfo&);

    public:

        DestinationInfo();

        virtual ~DestinationInfo();

        virtual unsigned char getDataStructureType() const;

        virtual DestinationInfo* cloneDataStructure() const;

        virtual void copyDataStructure(const DataStructure* src);

        virtual std::string toString() const;

        virtual bool equals(const DataStructure* value) const;

        virtual const Pointer<ConnectionId>& getConnectionId() const;
        virtual Pointer<ConnectionId>& getConnectionId();
        virtual void setConnectionId(const Pointer<ConnectionId>& connectionId);

        virtual const Pointer<ActiveMQDestination>& getDestination() const;
        virtual Pointer<ActiveMQDestination>& getDestination();
        virtual void setDestination(const Pointer<ActiveMQDestination>& destination);

        virtual unsigned char getOperationType() const;
        virtual void setOperationType(unsigned char operationType);

        virtual long long getTimeout() const;
        virtual void setTimeout(long long timeout);

        virtual const std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath() const;
        virtual std::vector< decaf::lang::Pointer<BrokerId> >& getBrokerPath();
        virtual void setBrokerPath(const std::vector< decaf::lang::Pointer<BrokerId> >& brokerPath);

        virtual Pointer<Command> visit(activemq::state::CommandVisitor* visitor);

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_DESTINATIONINFO_H_*/
