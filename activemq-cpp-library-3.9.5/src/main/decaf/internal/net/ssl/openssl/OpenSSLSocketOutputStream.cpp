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

#include "OpenSSLSocketOutputStream.h"

#include <decaf/internal/net/ssl/openssl/OpenSSLSocket.h>

using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::io;
using namespace decaf::internal;
using namespace decaf::internal::net;
using namespace decaf::internal::net::ssl;
using namespace decaf::internal::net::ssl::openssl;

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketOutputStream::OpenSSLSocketOutputStream( OpenSSLSocket* socket ) :
    OutputStream(), socket( socket ), closed( false ) {

    if( socket == NULL ) {
        throw NullPointerException(
            __FILE__, __LINE__, "TcpSocket instance passed was NULL." );
    }
}

////////////////////////////////////////////////////////////////////////////////
OpenSSLSocketOutputStream::~OpenSSLSocketOutputStream() {
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocketOutputStream::close() {

    if( this->closed ) {
        return;
    }

    try{
        this->closed = true;
        this->socket->close();
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocketOutputStream::doWriteByte( unsigned char c ) {

    try{

        // Treat the single byte case the same as an array.
        this->doWriteArrayBounded( &c, 1, 0, 1 );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCHALL_THROW( IOException )
}

////////////////////////////////////////////////////////////////////////////////
void OpenSSLSocketOutputStream::doWriteArrayBounded( const unsigned char* buffer, int size, int offset, int length ) {

    try{

        if( length == 0 ) {
            return;
        }

        if( buffer == NULL ) {
            throw NullPointerException(
                __FILE__, __LINE__, "passed buffer is null" );
        }

        if( closed ) {
            throw IOException(
                __FILE__, __LINE__, "This Stream has been closed." );
        }

        this->socket->write( buffer, size, offset, length );
    }
    DECAF_CATCH_RETHROW( IOException )
    DECAF_CATCH_RETHROW( NullPointerException )
    DECAF_CATCH_RETHROW( IndexOutOfBoundsException )
    DECAF_CATCHALL_THROW( IOException )
}
