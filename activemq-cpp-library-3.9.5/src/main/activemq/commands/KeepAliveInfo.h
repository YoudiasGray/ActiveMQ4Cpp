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

#ifndef _ACTIVEMQ_COMMANDS_KEEPALIVEINFO_H_
#define _ACTIVEMQ_COMMANDS_KEEPALIVEINFO_H_

// Turn off warning message for ignored exception specification
#ifdef _MSC_VER
#pragma warning( disable : 4290 )
#endif

#include <activemq/commands/BaseCommand.h>
#include <activemq/util/Config.h>
#include <decaf/lang/Pointer.h>
#include <string>
#include <vector>

namespace activemq {
namespace commands {

    using decaf::lang::Pointer;

    /*
     *
     *  Command code for OpenWire format for KeepAliveInfo
     *
     *  NOTE!: This file is auto generated - do not modify!
     *         if you need to make a change, please see the Java Classes
     *         in the activemq-cpp-openwire-generator module
     *
     */
    class AMQCPP_API KeepAliveInfo : public BaseCommand {
    protected:


    public:

        const static unsigned char ID_KEEPALIVEINFO = 10;

    private:

        KeepAliveInfo(const KeepAliveInfo&);
        KeepAliveInfo& operator= (const KeepAliveInfo&);

    public:

        KeepAliveInfo();

        virtual ~KeepAliveInfo();

        virtual unsigned char getDataStructureType() const;

        virtual KeepAliveInfo* cloneDataStructure() const;

        virtual void copyDataStructure(const DataStructure* src);

        virtual std::string toString() const;

        virtual bool equals(const DataStructure* value) const;

        /**
         * @return an answer of true to the isKeepAliveInfo() query.
         */
        virtual bool isKeepAliveInfo() const {
            return true;
        }

        virtual Pointer<Command> visit(activemq::state::CommandVisitor* visitor);

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_KEEPALIVEINFO_H_*/
