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

#ifndef _ACTIVEMQ_TRANSPORT_FAILOVER_BACKUPTRANSPORTPOOL_H_
#define _ACTIVEMQ_TRANSPORT_FAILOVER_BACKUPTRANSPORTPOOL_H_

#include <activemq/util/Config.h>
#include <activemq/threads/CompositeTask.h>
#include <activemq/threads/CompositeTaskRunner.h>
#include <activemq/transport/failover/CloseTransportsTask.h>
#include <activemq/transport/failover/BackupTransport.h>
#include <activemq/transport/failover/URIPool.h>

#include <decaf/lang/Pointer.h>
#include <decaf/io/IOException.h>
#include <decaf/util/LinkedList.h>

namespace activemq {
namespace transport {
namespace failover {

    using decaf::lang::Pointer;
    using decaf::util::LinkedList;
    using activemq::threads::CompositeTaskRunner;

    class BackupTransportPoolImpl;
    class FailoverTransport;

    class AMQCPP_API BackupTransportPool : public activemq::threads::CompositeTask {
    private:

        friend class BackupTransport;

        BackupTransportPoolImpl* impl;

        FailoverTransport* parent;
        Pointer<CompositeTaskRunner> taskRunner;
        Pointer<CloseTransportsTask> closeTask;
        Pointer<URIPool> uriPool;
        Pointer<URIPool> updates;
        Pointer<URIPool> priorityUriPool;
        volatile int backupPoolSize;
        volatile bool enabled;
        volatile int maxReconnectDelay;

    public:

        BackupTransportPool(FailoverTransport* parent,
                            const Pointer<CompositeTaskRunner> taskRunner,
                            const Pointer<CloseTransportsTask> closeTask,
                            const Pointer<URIPool> uriPool,
                            const Pointer<URIPool> updates,
                            const Pointer<URIPool> priorityUriPool);

        BackupTransportPool(FailoverTransport* parent,
                            int backupPoolSize,
                            const Pointer<CompositeTaskRunner> taskRunner,
                            const Pointer<CloseTransportsTask> closeTask,
                            const Pointer<URIPool> uriPool,
                            const Pointer<URIPool> updates,
                            const Pointer<URIPool> priorityUriPool);

        virtual ~BackupTransportPool();

        /**
         * Closes down the pool and destroys any Backups contained in the pool.
         */
        void close();

        /**
         * Return true if we don't currently have enough Connected Transports
         */
        virtual bool isPending() const;

        /**
         * Get a Connected Transport from the pool of Backups if any are present,
         * otherwise it return a NULL Pointer.
         *
         * @return Pointer to a Connected Transport or NULL
         */
        Pointer<BackupTransport> getBackup();

        /**
         * Connect to a Backup Broker if we haven't already connected to the max
         * number of Backups.
         */
        virtual bool iterate();

        /**
         * Gets the Max number of Backups this Task will create.
         * @return the max number of active BackupTransports that will be created.
         */
        int getBackupPoolSize() const {
            return this->backupPoolSize;
        }

        /**
         * Sets the Max number of Backups this Task will create.
         * @param size - the max number of active BackupTransports that will be created.
         */
        void setBackupPoolSize(int size) {
            this->backupPoolSize = size;
        }

        /**
         * Gets if the backup Transport Pool has been enabled or not, when not enabled
         * no backups are created and any that were are destroyed.
         *
         * @return true if enable.
         */
        bool isEnabled() const {
            return this->enabled;
        }

        /**
         * Sets if this Backup Transport Pool is enabled.  When not enabled no Backups
         * are created and any that were are destroyed.
         *
         * @param value - true to enable backup creation, false to disable.
         */
        void setEnabled(bool value);

        /**
         * Returns true if there is a Backup in the pool that's on the priority
         * backups list.
         *
         * @return true if there is a priority backup available.
         */
        bool isPriorityBackupAvailable() const;

    private:

        // The backups report their failure to the pool, the pool removes them
        // from the list and returns their URIs to the URIPool, and then adds
        // the internal transport to the close transport's task for cleanup.
        void onBackupTransportFailure(BackupTransport* failedTransport);

        Pointer<Transport> createTransport(const decaf::net::URI& location) const;

    };

}}}

#endif /*_ACTIVEMQ_TRANSPORT_FAILOVER_BACKUPTRANSPORTPOOL_H_*/
