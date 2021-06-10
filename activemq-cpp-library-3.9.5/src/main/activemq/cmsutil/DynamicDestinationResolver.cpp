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

#include "DynamicDestinationResolver.h"
#include "ResourceLifecycleManager.h"
#include <cms/CMSException.h>

using namespace activemq::cmsutil;
using namespace decaf::util;
using namespace cms;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
cms::Topic* DynamicDestinationResolver::SessionResolver::getTopic(const std::string& topicName) {

    cms::Topic* topic = NULL;
    try {

        // See if we already have a topic with this name.
        topic = topicMap.get(topicName);

    } catch (decaf::util::NoSuchElementException& ex) {

        // Create a new topic.
        topic = session->createTopic(topicName);

        // Add the topic to the lifecycle manager.
        resourceLifecycleManager->addDestination(topic);

        // Add the topic to the map.
        topicMap.put(topicName, topic);
    }
    return topic;
}

////////////////////////////////////////////////////////////////////////////////
cms::Queue* DynamicDestinationResolver::SessionResolver::getQueue(const std::string& queueName) {

    cms::Queue* queue = NULL;
    try {

        // See if we already have a queue with this name.
        queue = queueMap.get(queueName);

    } catch (decaf::util::NoSuchElementException& ex) {

        // Create a new queue.
        queue = session->createQueue(queueName);

        // Add the queue to the lifecycle manager.
        resourceLifecycleManager->addDestination(queue);

        // Add the queue to the map.
        queueMap.put(queueName, queue);
    }
    return queue;
}

////////////////////////////////////////////////////////////////////////////////
DynamicDestinationResolver::DynamicDestinationResolver() :
    DestinationResolver(), sessionResolverMap(), resourceLifecycleManager(NULL) {
}

////////////////////////////////////////////////////////////////////////////////
DynamicDestinationResolver::~DynamicDestinationResolver() {
    destroy();
}

////////////////////////////////////////////////////////////////////////////////
void DynamicDestinationResolver::destroy() {

    // Destroy the session resolvers.
    std::auto_ptr<Iterator<SessionResolver*> > sessionResolvers(sessionResolverMap.values().iterator());
    while (sessionResolvers->hasNext()) {
        delete sessionResolvers->next();
    }

    sessionResolverMap.clear();
}

////////////////////////////////////////////////////////////////////////////////
cms::Destination* DynamicDestinationResolver::resolveDestinationName(cms::Session* session, const std::string& destName, bool pubSubDomain) {

    if (destName == "") {
        throw CMSException("destination name is invalid", NULL);
    }

    // Get the resolver for this session.
    SessionResolver* resolver = NULL;
    try {
        resolver = sessionResolverMap.get(session);
    } catch (decaf::util::NoSuchElementException& ex) {
        resolver = new SessionResolver(session, resourceLifecycleManager);
        sessionResolverMap.put(session, resolver);
    }

    // Return the appropriate destination.
    if (pubSubDomain) {
        return resolver->getTopic(destName);
    } else {
        return resolver->getQueue(destName);
    }
}
