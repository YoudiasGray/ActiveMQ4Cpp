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

#include <activemq/commands/Response.h>
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
 *  Command code for OpenWire format for Response
 *
 *  NOTE!: This file is auto generated - do not modify!
 *         if you need to make a change, please see the Java Classes in the
 *         activemq-cpp-openwire-generator module
 *
 */

////////////////////////////////////////////////////////////////////////////////
Response::Response() :
    BaseCommand(), correlationId(0) {

}

////////////////////////////////////////////////////////////////////////////////
Response::~Response() {
}

////////////////////////////////////////////////////////////////////////////////
Response* Response::cloneDataStructure() const {
    std::auto_ptr<Response> response(new Response());

    // Copy the data from the base class or classes
    response->copyDataStructure(this);

    return response.release();
}

////////////////////////////////////////////////////////////////////////////////
void Response::copyDataStructure(const DataStructure* src) {

    // Protect against invalid self assignment.
    if (this == src) {
        return;
    }

    const Response* srcPtr = dynamic_cast<const Response*>(src);

    if (srcPtr == NULL || src == NULL) {
        throw decaf::lang::exceptions::NullPointerException(
            __FILE__, __LINE__,
            "Response::copyDataStructure - src is NULL or invalid");
    }

    // Copy the data of the base class or classes
    BaseCommand::copyDataStructure(src);

    this->setCorrelationId(srcPtr->getCorrelationId());
}

////////////////////////////////////////////////////////////////////////////////
unsigned char Response::getDataStructureType() const {
    return Response::ID_RESPONSE;
}

////////////////////////////////////////////////////////////////////////////////
std::string Response::toString() const {

    ostringstream stream;

    stream << "Response { "
           << "commandId = " << this->getCommandId() << ", "
           << "responseRequired = " << boolalpha << this->isResponseRequired();
    stream << ", ";
    stream << "CorrelationId = " << this->getCorrelationId();
    stream << " }";

    return stream.str();
}

////////////////////////////////////////////////////////////////////////////////
bool Response::equals(const DataStructure* value) const {

    if (this == value) {
        return true;
    }

    const Response* valuePtr = dynamic_cast<const Response*>(value);

    if (valuePtr == NULL || value == NULL) {
        return false;
    }

    if (this->getCorrelationId() != valuePtr->getCorrelationId()) {
        return false;
    }
    if (!BaseCommand::equals(value)) {
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
int Response::getCorrelationId() const {
    return correlationId;
}

////////////////////////////////////////////////////////////////////////////////
void Response::setCorrelationId(int correlationId) {
    this->correlationId = correlationId;
}

////////////////////////////////////////////////////////////////////////////////
decaf::lang::Pointer<commands::Command> Response::visit(activemq::state::CommandVisitor* visitor) {
    return visitor->processResponse(this);
}
