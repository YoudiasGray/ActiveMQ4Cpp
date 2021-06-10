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
#include <decaf/net/SocketFactory.h>

#include <decaf/lang/Runnable.h>
#include <decaf/internal/net/DefaultSocketFactory.h>
#include <decaf/internal/net/Network.h>

using namespace decaf;
using namespace decaf::io;
using namespace decaf::net;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal::net;

////////////////////////////////////////////////////////////////////////////////
namespace {

    class ShutdownTask : public decaf::lang::Runnable {
    private:

        SocketFactory** defaultRef;

    private:

        ShutdownTask( const ShutdownTask& );
        ShutdownTask& operator= ( const ShutdownTask& );

    public:

        ShutdownTask( SocketFactory** defaultRef ) : defaultRef( defaultRef ) {}
        virtual ~ShutdownTask() {}

        virtual void run() {
            *defaultRef = NULL;
        }
    };
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory* SocketFactory::defaultFactory = NULL;

////////////////////////////////////////////////////////////////////////////////
SocketFactory::SocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory::~SocketFactory() {
}

////////////////////////////////////////////////////////////////////////////////
Socket* SocketFactory::createSocket() {

    throw IOException(
        __FILE__, __LINE__, "Unconnected Sockets not implemented for this Socket Type." );
}

////////////////////////////////////////////////////////////////////////////////
SocketFactory* SocketFactory::getDefault() {

    Network* networkRuntime = Network::getNetworkRuntime();

    synchronized( networkRuntime->getRuntimeLock() ) {

        if( defaultFactory == NULL ) {
            defaultFactory = new DefaultSocketFactory();
            networkRuntime->addAsResource( defaultFactory );
            networkRuntime->addShutdownTask( new ShutdownTask( &defaultFactory ) );
        }
    }

    return defaultFactory;
}
