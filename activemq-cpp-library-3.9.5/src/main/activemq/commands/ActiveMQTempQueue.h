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

#ifndef _ACTIVEMQ_COMMANDS_ACTIVEMQTEMPQUEUE_H_
#define _ACTIVEMQ_COMMANDS_ACTIVEMQTEMPQUEUE_H_

#include <activemq/util/Config.h>
#include <activemq/commands/ActiveMQTempDestination.h>
#include <cms/TemporaryQueue.h>
#include <vector>
#include <string>
#include <memory>

namespace activemq {
namespace commands {

    class AMQCPP_API ActiveMQTempQueue: public ActiveMQTempDestination, public cms::TemporaryQueue {
    public:

        const static unsigned char ID_ACTIVEMQTEMPQUEUE = 102;

    public:

        using ActiveMQTempDestination::equals;

    private:

        ActiveMQTempQueue(const ActiveMQTempQueue&);
        ActiveMQTempQueue& operator=(const ActiveMQTempQueue&);

    public:

        ActiveMQTempQueue();
        ActiveMQTempQueue(const std::string& name);
        virtual ~ActiveMQTempQueue() throw ();

        virtual unsigned char getDataStructureType() const;

        virtual ActiveMQTempQueue* cloneDataStructure() const;

        virtual void copyDataStructure(const DataStructure* src);

        virtual std::string toString() const;

        virtual bool equals(const DataStructure* value) const;

        virtual const cms::TemporaryQueue* getCMSDestination() const {
            return this;
        }

    public: // CMS Destination

        virtual cms::Destination::DestinationType getDestinationType() const {
            return cms::Destination::TEMPORARY_QUEUE;
        }

        virtual cms::Destination* clone() const {
            return dynamic_cast<cms::Destination*>(this->cloneDataStructure());
        }

        virtual void copy(const cms::Destination& source) {
            this->copyDataStructure(dynamic_cast<const DataStructure*>(&source));
        }

        virtual const cms::CMSProperties& getCMSProperties() const {
            return this->getOptions();
        }

        virtual bool equals(const cms::Destination& other) const;

    public:

        virtual std::string getQueueName() const {
            return this->getPhysicalName();
        }

        virtual void destroy();

    };

}}

#endif /*_ACTIVEMQ_COMMANDS_ACTIVEMQTEMPQUEUE_H_*/
