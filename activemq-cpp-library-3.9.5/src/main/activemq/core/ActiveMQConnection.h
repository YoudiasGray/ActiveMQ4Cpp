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

#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTION_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTION_H_

#include <cms/EnhancedConnection.h>
#include <activemq/util/Config.h>
#include <activemq/core/Dispatcher.h>
#include <activemq/commands/ActiveMQTempDestination.h>
#include <activemq/commands/ConnectionInfo.h>
#include <activemq/commands/ConsumerInfo.h>
#include <activemq/commands/SessionId.h>
#include <activemq/exceptions/ActiveMQException.h>
#include <activemq/transport/Transport.h>
#include <activemq/transport/TransportListener.h>
#include <activemq/threads/Scheduler.h>
#include <activemq/core/kernels/ActiveMQProducerKernel.h>
#include <activemq/core/kernels/ActiveMQSessionKernel.h>
#include <decaf/util/Properties.h>
#include <decaf/util/ArrayList.h>
#include <decaf/util/concurrent/atomic/AtomicBoolean.h>
#include <decaf/util/concurrent/ExecutorService.h>
#include <decaf/lang/exceptions/UnsupportedOperationException.h>
#include <decaf/lang/exceptions/NullPointerException.h>
#include <decaf/lang/exceptions/IllegalStateException.h>

#include <string>
#include <memory>

namespace activemq {
namespace core {

    using decaf::lang::Pointer;

    class ActiveMQSession;
    class ConnectionConfig;
    class PrefetchPolicy;
    class RedeliveryPolicy;

    /**
     * Concrete connection used for all connectors to the
     * ActiveMQ broker.
     *
     * @since 2.0
     */
    class AMQCPP_API ActiveMQConnection : public virtual cms::EnhancedConnection,
                                          public transport::TransportListener {
    private:

        ConnectionConfig* config;

        /**
         * The instance of ConnectionMetaData to return to clients.
         */
        std::auto_ptr<cms::ConnectionMetaData> connectionMetaData;

        /**
         * Indicates if this Connection is started
         */
        decaf::util::concurrent::atomic::AtomicBoolean started;

        /**
         * Indicates that this connection has been closed, it is no longer
         * usable after this becomes true
         */
        decaf::util::concurrent::atomic::AtomicBoolean closed;

        /**
         * Indicates that this connection has been closed, it is no longer
         * usable after this becomes true
         */
        decaf::util::concurrent::atomic::AtomicBoolean closing;

        /**
         * Indicates that this connection's Transport has failed.
         */
        decaf::util::concurrent::atomic::AtomicBoolean transportFailed;

    private:

        ActiveMQConnection(const ActiveMQConnection&);
        ActiveMQConnection& operator=(const ActiveMQConnection&);

    public:

        /**
         * Constructor
         *
         * @param transport
         *        The Transport requested for this connection to the Broker.
         * @param properties
         *        The Properties that were defined for this connection
         */
        ActiveMQConnection(const Pointer<transport::Transport> transport,
                           const Pointer<decaf::util::Properties> properties);

        virtual ~ActiveMQConnection();

        /**
         * Adds the session resources for the given session instance.
         *
         * @param session
         *      The session to be added to this connection.
         *
         * @throws CMSException if an error occurs while removing performing the operation.
         */
        virtual void addSession(Pointer<activemq::core::kernels::ActiveMQSessionKernel> session);

        /**
         * Removes the session resources for the given session instance.
         *
         * @param session
         *      The session to be unregistered from this connection.
         *
         * @throws CMSException if an error occurs while removing performing the operation.
         */
        virtual void removeSession(Pointer<activemq::core::kernels::ActiveMQSessionKernel> session);

        /**
         * Adds an active Producer to the Set of known producers.
         *
         * @param producer
         *      The Producer to add from the the known set.
         *
         * @throws CMSException if an error occurs while removing performing the operation.
         */
        virtual void addProducer(Pointer<kernels::ActiveMQProducerKernel> producer);

        /**
         * Removes an active Producer to the Set of known producers.
         * @param producerId - The ProducerId to remove from the the known set.
         * @throws CMSException if an error occurs while removing performing the operation.
         */
        virtual void removeProducer(const Pointer<commands::ProducerId>& producerId);

        /**
         * Adds a dispatcher for a consumer.
         * @param consumer - The consumer for which to register a dispatcher.
         * @param dispatcher - The dispatcher to handle incoming messages for the consumer.
         * @throws CMSException if an error occurs while removing performing the operation.
         */
        virtual void addDispatcher(const Pointer<commands::ConsumerId>& consumer, Dispatcher* dispatcher);

        /**
         * Removes the dispatcher for a consumer.
         * @param consumer - The consumer for which to remove the dispatcher.
         * @throws CMSException if an error occurs while removing performing the operation.
         */
        virtual void removeDispatcher(const Pointer<commands::ConsumerId>& consumer);

        /**
         * If supported sends a message pull request to the service provider asking
         * for the delivery of a new message.  This is used in the case where the
         * service provider has been configured with a zero prefetch or is only
         * capable of delivering messages on a pull basis.
         * @param consumer - the ConsumerInfo for the requesting Consumer.
         * @param timeout - the time that the client is willing to wait.
         *
         * @throws ActiveMQException if an error occurs while removing performing the operation.
         */
        virtual void sendPullRequest(const commands::ConsumerInfo* consumer, long long timeout);

        /**
         * Checks if this connection has been closed
         * @return true if the connection is closed
         */
        bool isClosed() const {
            return this->closed.get();
        }

        /**
         * Check if this connection has been started.
         * @return true if the start method has been called.
         */
        bool isStarted() const {
            return this->started.get();
        }

        /**
         * Checks if the Connection's Transport has failed
         * @return true if the Connection's Transport has failed.
         */
        bool isTransportFailed() const {
            return this->transportFailed.get();
        }

        /**
         * Requests that the Broker removes the given Destination.  Calling this
         * method implies that the client is finished with the Destination and that
         * no other messages will be sent or received for the given Destination.  The
         * Broker frees all resources it has associated with this Destination.
         *
         * @param destination
         *        The Destination the Broker will be requested to remove.
         *
         * @throws NullPointerException
         *         If the passed Destination is Null
         * @throws IllegalStateException
         *         If the connection is closed.
         * @throws UnsupportedOperationException
         *         If the wire format in use does not support this operation.
         * @throws ActiveMQException
         *         If any other error occurs during the attempt to destroy the destination.
         */
        virtual void destroyDestination(const commands::ActiveMQDestination* destination);

        /**
         * Requests that the Broker removes the given Destination.  Calling this
         * method implies that the client is finished with the Destination and that
         * no other messages will be sent or received for the given Destination.  The
         * Broker frees all resources it has associated with this Destination.
         *
         * @param destination
         *        The CMS Destination the Broker will be requested to remove.
         *
         * @throws NullPointerException
         *         If the passed Destination is Null
         * @throws IllegalStateException
         *         If the connection is closed.
         * @throws UnsupportedOperationException
         *         If the wire format in use does not support this operation.
         * @throws ActiveMQException
         *         If any other error occurs during the attempt to destroy the destination.
         */
        virtual void destroyDestination(const cms::Destination* destination);

        /**
         * Allows Consumers to check if an incoming Message is a Duplicate.
         *
         * @param dispatcher
         *      The Dispatcher that is checking the Message for Duplication.
         * @param message
         *      The Message that should be checked.
         *
         * @return true if the Message was seen before.
         */
        bool isDuplicate(Dispatcher* dispatcher, Pointer<commands::Message> message);

        /**
         * Mark message as received.
         *
         * @param dispatcher
         *      The Dispatcher instance that has received the Message.
         * @param message
         *      The Message that has been received.
         */
        void rollbackDuplicate(Dispatcher* dispatcher, Pointer<commands::Message> message);

        /**
         * Removes the Audit information stored for a given MessageConsumer
         *
         * @param dispatcher
         *      The Dispatcher instance that has received the Message.
         */
        void removeAuditedDispatcher(Dispatcher* dispatcher);

    public:   // Connection Interface Methods

        /**
         * {@inheritDoc}
         */
        virtual const cms::ConnectionMetaData* getMetaData() const {
            return connectionMetaData.get();
        }

        /**
         * {@inheritDoc}
         */
        virtual cms::Session* createSession();

        /**
         * {@inheritDoc}
         */
        virtual std::string getClientID() const;

        /**
         * {@inheritDoc}
         */
        virtual void setClientID(const std::string& clientID);

        /**
         * {@inheritDoc}
         */
        virtual cms::Session* createSession(cms::Session::AcknowledgeMode ackMode);

        /**
         * {@inheritDoc}
         */
        virtual void close();

        /**
         * {@inheritDoc}
         */
        virtual void start();

        /**
         * {@inheritDoc}
         */
        virtual void stop();

        /**
         * {@inheritDoc}
         */
        virtual cms::ExceptionListener* getExceptionListener() const;

        /**
         * {@inheritDoc}
         */
        virtual void setExceptionListener(cms::ExceptionListener* listener);

        /**
         * {@inheritDoc}
         */
        virtual void setMessageTransformer(cms::MessageTransformer* transformer);

        /**
         * {@inheritDoc}
         */
        virtual cms::MessageTransformer* getMessageTransformer() const;

        /**
         * {@inheritDoc}
         */
        virtual cms::DestinationSource* getDestinationSource();

    public:   // Configuration Options

        /**
         * Sets the username that should be used when creating a new connection
         * @param username string
         */
        void setUsername(const std::string& username);

        /**
         * Gets the username that this factory will use when creating a new
         * connection instance.
         * @return username string, "" for default credentials
         */
        const std::string& getUsername() const;

        /**
         * Sets the password that should be used when creating a new connection
         * @param password string
         */
        void setPassword(const std::string& password);

        /**
         * Gets the password that this factory will use when creating a new
         * connection instance.
         * @return password string, "" for default credentials
         */
        const std::string& getPassword() const;

        /**
         * Sets the Client Id.
         * @param clientId - The new clientId value.
         */
        void setDefaultClientId(const std::string& clientId);

        /**
         * Sets the Broker URL that should be used when creating a new
         * connection instance
         * @param brokerURL string
         */
        void setBrokerURL(const std::string& brokerURL);

        /**
         * Gets the Broker URL that this factory will use when creating a new
         * connection instance.
         * @return brokerURL string
         */
        const std::string& getBrokerURL() const;

        /**
         * Sets the PrefetchPolicy instance that this factory should use when it creates
         * new Connection instances.  The PrefetchPolicy passed becomes the property of the
         * factory and will be deleted when the factory is destroyed.
         *
         * @param policy
         *      The new PrefetchPolicy that the ConnectionFactory should clone for Connections.
         */
        void setPrefetchPolicy(PrefetchPolicy* policy);

        /**
         * Gets the pointer to the current PrefetchPolicy that is in use by this ConnectionFactory.
         *
         * @return a pointer to this objects PrefetchPolicy.
         */
        PrefetchPolicy* getPrefetchPolicy() const;

        /**
         * Sets the RedeliveryPolicy instance that this factory should use when it creates
         * new Connection instances.  The RedeliveryPolicy passed becomes the property of the
         * factory and will be deleted when the factory is destroyed.
         *
         * @param policy
         *      The new RedeliveryPolicy that the ConnectionFactory should clone for Connections.
         */
        void setRedeliveryPolicy(RedeliveryPolicy* policy);

        /**
         * Gets the pointer to the current RedeliveryPolicy that is in use by this ConnectionFactory.
         *
         * @return a pointer to this objects RedeliveryPolicy.
         */
        RedeliveryPolicy* getRedeliveryPolicy() const;

        /**
         * @return The value of the dispatch asynchronously option sent to the broker.
         */
        bool isDispatchAsync() const;

        /**
         * Should messages be dispatched synchronously or asynchronously from the producer
         * thread for non-durable topics in the broker? For fast consumers set this to false.
         * For slow consumers set it to true so that dispatching will not block fast consumers. .
         *
         * @param value
         *        The value of the dispatch asynchronously option sent to the broker.
         */
        void setDispatchAsync(bool value);

        /**
         * Gets if the Connection should always send things Synchronously.
         *
         * @return true if sends should always be Synchronous.
         */
        bool isAlwaysSyncSend() const;

        /**
         * Sets if the Connection should always send things Synchronously.
         * @param value
         *        true if sends should always be Synchronous.
         */
        void setAlwaysSyncSend(bool value);

        /**
         * Gets if the useAsyncSend option is set
         * @return true if on false if not.
         */
        bool isUseAsyncSend() const;

        /**
         * Sets the useAsyncSend option
         * @param value - true to activate, false to disable.
         */
        void setUseAsyncSend(bool value);

        /**
         * Gets if the Connection is configured for Message body compression.
         * @return if the Message body will be Compressed or not.
         */
        bool isUseCompression() const;

        /**
         * Sets whether Message body compression is enabled.
         *
         * @param value
         *      Boolean indicating if Message body compression is enabled.
         */
        void setUseCompression(bool value);

        /**
         * Sets the Compression level used when Message body compression is enabled, a
         * value of -1 causes the Compression Library to use the default setting which
         * is a balance of speed and compression.  The range of compression levels is
         * [0..9] where 0 indicates best speed and 9 indicates best compression.
         *
         * @param value
         *      A signed int value that controls the compression level.
         */
        void setCompressionLevel(int value);

        /**
         * Gets the currently configured Compression level for Message bodies.
         *
         * @return the int value of the current compression level.
         */
        int getCompressionLevel() const;

        /**
         * Gets the assigned send timeout for this Connector
         * @return the send timeout configured in the connection uri
         */
        unsigned int getSendTimeout() const;

        /**
         * Sets the send timeout to use when sending Message objects, this will
         * cause all messages to be sent using a Synchronous request is non-zero.
         * @param timeout - The time to wait for a response.
         */
        void setSendTimeout(unsigned int timeout);

        /**
         * Gets the assigned connect response timeout for this Connector
         * @return the connect response timeout configured in the connection uri
         */
        unsigned int getConnectResponseTimeout() const;

        /**
         * Sets the connect response timeout to use when sending Message objects, this will
         * protect clients using a Synchronous request in the case of the broker not responding
         * or missing the brokers response.
         * @param timeout - The time to wait for a connect response.
         */
        void setConnectResponseTimeout(unsigned int connectResponseTimeout);

        /**
         * Gets the assigned close timeout for this Connector
         * @return the close timeout configured in the connection uri
         */
        unsigned int getCloseTimeout() const;

        /**
         * Sets the close timeout to use when sending the disconnect request.
         * @param timeout - The time to wait for a close message.
         */
        void setCloseTimeout(unsigned int timeout);

        /**
         * Gets the configured producer window size for Producers that are created
         * from this connector.  This only applies if there is no send timeout and the
         * producer is able to send asynchronously.
         * @return size in bytes of messages that this producer can produce before
         *         it must block and wait for ProducerAck messages to free resources.
         */
        unsigned int getProducerWindowSize() const;

        /**
         * Sets the size in Bytes of messages that a producer can send before it is blocked
         * to await a ProducerAck from the broker that frees enough memory to allow another
         * message to be sent.
         * @param windowSize - The size in bytes of the Producers memory window.
         */
        void setProducerWindowSize(unsigned int windowSize);

        /**
         * @return true if the Connections that this factory creates should support the
         * message based priority settings.
         */
        bool isMessagePrioritySupported() const;

        /**
         * Set whether or not this factory should create Connection objects with the Message
         * priority support function enabled.
         *
         * @param value
         *      Boolean indicating if Message priority should be enabled.
         */
        void setMessagePrioritySupported(bool value);

        /**
         * Get the Next Temporary Destination Id
         * @return the next id in the sequence.
         */
        long long getNextTempDestinationId();

        /**
         * Get the Next Temporary Destination Id
         * @return the next id in the sequence.
         */
        long long getNextLocalTransactionId();

        /**
         * Is the Connection configured to watch for advisory messages to maintain state of
         * temporary destination create and destroy.
         *
         * @return true if the Connection will listen for temporary topic advisory messages.
         */
        bool isWatchTopicAdvisories() const;

        /**
         * Sets whether this Connection is listening for advisory messages regarding temporary
         * destination creation and deletion.
         *
         * @param value
         *      Boolean indicating if advisory message monitoring should be enabled.
         */
        void setWatchTopicAdvisories(bool value);

        /**
         * Get the audit depth for Messages for consumers when using a fault
         * tolerant transport.  The higher the value the more messages are checked
         * for duplication, and the larger the performance impact of duplicate
         * detection will be.
         *
         * @return the configured audit depth.
         */
        int getAuditDepth() const;

        /**
         * Set the audit depth for Messages for consumers when using a fault
         * tolerant transport.  The higher the value the more messages are checked
         * for duplication, and the larger the performance impact of duplicate
         * detection will be.
         *
         * @param auditDepth
         *      The configured audit depth.
         */
        void setAuditDepth(int auditDepth);

        /**
         * The number of Producers that will be audited.
         *
         * @return the configured number of producers to include in the audit.
         */
        int getAuditMaximumProducerNumber() const;

        /**
         * The number of Producers that will be audited.
         *
         * @param auditMaximumProducerNumber
         *      The configured number of producers to include in the audit.
         */
        void setAuditMaximumProducerNumber(int auditMaximumProducerNumber);

        /**
         * Gets the value of the configured Duplicate Message detection feature.
         *
         * When enabled and a fault tolerant transport is used (think failover) then
         * this feature will help to detect and filter duplicate messages that might
         * otherwise be delivered to a consumer after a connection failure.
         *
         * Disabling this can increase performance since no Message auditing will
         * occur.
         *
         * @return the checkForDuplicates value currently set.
         */
        bool isCheckForDuplicates() const;

        /**
         * Gets the value of the configured Duplicate Message detection feature.
         *
         * When enabled and a fault tolerant transport is used (think failover) then
         * this feature will help to detect and filter duplicate messages that might
         * otherwise be delivered to a consumer after a connection failure.
         *
         * Disabling this can increase performance since no Message auditing will
         * occur.
         *
         * @param checkForDuplicates
         *      The checkForDuplicates value to be configured.
         */
        void setCheckForDuplicates(bool checkForDuplicates);

        /**
         * when true, submit individual transacted acks immediately rather than with transaction
         * completion.  This allows the acks to represent delivery status which can be persisted on
         * rollback Used in conjunction with KahaDB set to Rewrite On Redelivery.
         *
         * @return true if this option is enabled.
         */
        bool isTransactedIndividualAck() const;

        /**
         * when true, submit individual transacted acks immediately rather than with transaction
         * completion.  This allows the acks to represent delivery status which can be persisted on
         * rollback Used in conjunction with KahaDB set to Rewrite On Redelivery.
         *
         * @param transactedIndividualAck
         *      The value to set.
         */
        void setTransactedIndividualAck(bool transactedIndividualAck);

        /**
         * Returns true if non-blocking redelivery of Messages is configured for Consumers
         * that are rolled back or recovered.
         *
         * @return true if non-blocking redelivery is enabled.
         */
        bool isNonBlockingRedelivery() const;

        /**
         * When true a MessageConsumer will not stop Message delivery before re-delivering Messages
         * from a rolled back transaction.  This implies that message order will not be preserved and
         * also will result in the TransactedIndividualAck option to be enabled.
         *
         * @param nonBlockingRedelivery
         *      The value to configure for non-blocking redelivery.
         */
        void setNonBlockingRedelivery(bool nonBlockingRedelivery);

        /**
         * Gets the delay period for a consumer redelivery.
         *
         * @return configured time delay in milliseconds.
         */
        long long getConsumerFailoverRedeliveryWaitPeriod() const;

        /**
         * Sets the delay period for a consumer redelivery.
         *
         * @param value
         *      The configured time delay in milliseconds.
         */
        void setConsumerFailoverRedeliveryWaitPeriod(long long value);

        /**
         * @return true if optimizeAcknowledge is enabled.
         */
        bool isOptimizeAcknowledge() const;

        /**
         * Sets if Consumers are configured to use Optimized Acknowledge by default.
         *
         * @param optimizeAcknowledge
         *      The optimizeAcknowledge mode to set.
         */
        void setOptimizeAcknowledge(bool optimizeAcknowledge);

        /**
         * Gets the time between optimized ack batches in milliseconds.
         *
         * @return time between optimized ack batches in Milliseconds.
         */
        long long getOptimizeAcknowledgeTimeOut() const;

        /**
         * The max time in milliseconds between optimized ack batches.
         *
         * @param optimizeAcknowledgeTimeOut
         *      The time in milliseconds for optimized ack batches.
         */
        void setOptimizeAcknowledgeTimeOut(long long optimizeAcknowledgeTimeOut);

        /**
         * Gets the configured time interval that is used to force all MessageConsumers that have
         * optimizedAcknowledge enabled to send an ack for any outstanding Message Acks.  By default
         * this value is set to zero meaning that the consumers will not do any background Message
         * acknowledgment.
         *
         * @return the scheduledOptimizedAckInterval
         */
        long long getOptimizedAckScheduledAckInterval() const;

        /**
         * Sets the amount of time between scheduled sends of any outstanding Message Acks for
         * consumers that have been configured with optimizeAcknowledge enabled.
         *
         * Time is given in Milliseconds.
         *
         * @param optimizedAckScheduledAckInterval
         *      The scheduledOptimizedAckInterval to use for new Consumers.
         */
        void setOptimizedAckScheduledAckInterval(long long optimizedAckScheduledAckInterval);

        /**
         * Should all created consumers be retroactive.
         *
         * @return true if consumer will be created with the retroactive flag set.
         */
        bool isUseRetroactiveConsumer() const;

        /**
         * Sets whether or not retroactive consumers are enabled. Retroactive
         * consumers allow non-durable topic subscribers to receive old messages
         * that were published before the non-durable subscriber started.
         *
         * @param useRetroactiveConsumer
         *      The value of this configuration option.
         */
        void setUseRetroactiveConsumer(bool useRetroactiveConsumer);

        /**
         * Should all created consumers be exclusive.
         *
         * @return true if consumer will be created with the exclusive flag set.
         */
        bool isExclusiveConsumer() const;

        /**
         * Enables or disables whether or not queue consumers should be exclusive or
         * not for example to preserve ordering when not using Message Groups.
         *
         * @param exclusiveConsumer
         *      The value of this configuration option.
         */
        void setExclusiveConsumer(bool exclusiveConsumer);

        /**
         * Returns whether Message acknowledgments are sent asynchronously meaning no
         * response is required from the broker before the ack completes.
         *
         * @return the sendAcksAsync configured value.
         */
        bool isSendAcksAsync() const;

        /**
         * Sets whether Message acknowledgments are sent asynchronously meaning no
         * response is required from the broker before the ack completes.
         *
         * @param sendAcksAsync
         *      The sendAcksAsync configuration value to set.
         */
        void setSendAcksAsync(bool sendAcksAsync);

        /**
         * @return Returns the alwaysSessionAsync configuration setting.
         */
        bool isAlwaysSessionAsync() const;

        /**
         * If this flag is not set then a separate thread is not used for dispatching messages
         * for each Session in the Connection. However, a separate thread is always used if there
         * is more than one session, or the session isn't in auto acknowledge or duplicates ok mode.
         * By default this value is set to true and session dispatch happens asynchronously.
         */
        void setAlwaysSessionAsync(bool alwaysSessionAsync);

        /**
         * @return true if the consumer will skip checking messages for expiration.
         */
        bool isConsumerExpiryCheckEnabled();

        /**
         * Configures whether this consumer will perform message expiration processing
         * on all incoming messages.  This feature is enabled by default.
         *
         * @param consumerExpiryCheckEnabled
         *      False if the default message expiration checks should be disabled.
         */
        void setConsumerExpiryCheckEnabled(bool consumerExpiryCheckEnabled);

        /**
         * @return the current connection's OpenWire protocol version.
         */
        int getProtocolVersion() const;

    public: // TransportListener

        /**
         * Adds a transport listener so that a client can be notified of events in
         * the underlying transport, client's are always notified after the event has
         * been processed by the Connection class.  Client's should ensure that the
         * registered listener does not block or take a long amount of time to execute
         * in order to not degrade performance of this Connection.
         *
         * @param transportListener
         *      The TransportListener instance to add to this Connection's set of listeners
         *      to notify of Transport events.
         */
        void addTransportListener(transport::TransportListener* transportListener);

        /**
         * Removes a registered TransportListener from the Connection's set of Transport
         * listeners, this listener will no longer receive any Transport related events.  The
         * caller is responsible for freeing the listener in all cases.
         *
         * @param transportListener
         *      The pointer to the TransportListener to remove from the set of listeners.
         */
        void removeTransportListener(transport::TransportListener* transportListener);

        /**
         * Event handler for the receipt of a non-response command from the
         * transport.
         * @param command the received command object.
         */
        virtual void onCommand(const Pointer<commands::Command> command);

        /**
         * Event handler for an exception from a command transport.
         * @param ex The exception.
         */
        virtual void onException(const decaf::lang::Exception& ex);

        /**
         * The transport has suffered an interruption from which it hopes to recover
         */
        virtual void transportInterrupted();

        /**
         * The transport has resumed after an interruption
         */
        virtual void transportResumed();

    public:

        /**
         * Gets the ConnectionInfo for this Object, if the Connection is not open
         * than this method throws an exception.
         *
         * @throws ActiveMQException if an error occurs while performing this operation.
         */
        const commands::ConnectionInfo& getConnectionInfo() const;

        /**
         * Gets the ConnectionId for this Object, if the Connection is not open
         * than this method throws an exception.
         *
         * @throws ActiveMQException if an error occurs while performing this operation.
         */
        const commands::ConnectionId& getConnectionId() const;

        /**
         * Gets a reference to this object's Transport instance.
         *
         * @return a reference to the Transport that is in use by this Connection.
         */
        transport::Transport& getTransport() const;

        /**
         * Gets a reference to the Connection objects built in Scheduler instance.
         *
         * @return a reference to a Scheduler instance owned by this Connection.
         */
        Pointer<threads::Scheduler> getScheduler() const;

        /**
         * Returns the Id of the Resource Manager that this client will use should
         * it be entered into an XA Transaction.
         *
         * @return a string containing the resource manager Id for XA Transactions.
         */
        std::string getResourceManagerId() const;

        /**
         * Clean up this connection object, reseting it back to a state that mirrors
         * what a newly created ActiveMQConnection object has.
         */
        void cleanup();

        /**
         * Sends a message without request that the broker send a response to indicate that
         * it was received.
         *
         * @param command
         *      The Command object to send to the Broker.
         *
         * @throws ActiveMQException if not currently connected, or if the operation
         *         fails for any reason.
         */
        void oneway(Pointer<commands::Command> command);

        /**
         * Sends a synchronous request and returns the response from the broker.  This
         * method converts any error responses it receives into an exception.
         *
         * @param command
         *      The Command object that is to be sent to the broker.
         * @param timeout
         *      The time in milliseconds to wait for a response, default is zero or infinite.
         *
         * @return a Pointer instance to the Response object sent from the Broker.
         *
         * @throws BrokerException if the response from the broker is of type ExceptionResponse.
         * @throws ActiveMQException if any other error occurs while sending the Command.
         */
        Pointer<commands::Response> syncRequest(Pointer<commands::Command> command, unsigned int timeout = 0);

        /**
         * Sends a synchronous request and returns the response from the broker.  This
         * method converts any error responses it receives into an exception.
         *
         * @param command
         *      The Command object that is to be sent to the broker.
         * @param onComplete
         *      Completion callback that will be notified on send success or failure.
         *
         * @throws BrokerException if the response from the broker is of type ExceptionResponse.
         * @throws ActiveMQException if any other error occurs while sending the Command.
         */
        void asyncRequest(Pointer<commands::Command> command, cms::AsyncCallback* onComplete);

        /**
         * Notify the exception listener
         * @param ex the exception to fire
         */
        virtual void fire(const exceptions::ActiveMQException& ex);

        /**
         * Indicates that a Connection resource that is processing the transportInterrupted
         * event has completed.
         */
        void setTransportInterruptionProcessingComplete();

        /**
         * Sets the pointer to the first exception that caused the Connection to become failed.
         *
         * @param error
         *      pointer to the exception instance that is to be the first failure error if the
         *      first error is already set this value is deleted.
         */
        void setFirstFailureError(decaf::lang::Exception* error);

        /**
         * Gets the pointer to the first exception that caused the Connection to become failed.
         *
         * @return pointer to an Exception instance or NULL if none is set.
         */
        decaf::lang::Exception* getFirstFailureError() const;

        /**
         * Event handler for dealing with async exceptions.
         *
         * @param ex
         *      The exception that caused the error condition.
         */
        void onAsyncException(const decaf::lang::Exception& ex);

        /**
         * Handles async client internal exceptions which don't usually affect the connection
         * itself.  These are reported but do not shutdown the Connection.
         *
         * @param error the exception that the problem
         */
        void onClientInternalException(const decaf::lang::Exception& ex);

        /**
         * Check for Closed State and Throw an exception if true.
         *
         * @throws CMSException if the Connection is closed.
         */
        void checkClosed() const;

        /**
         * Check for Closed State and Failed State and Throw an exception if either is true.
         *
         * @throws CMSException if the Connection is closed or failed.
         */
        void checkClosedOrFailed() const;

        /**
         * If its not been sent, then send the ConnectionInfo to the Broker.
         */
        void ensureConnectionInfoSent();

        /**
         * @return the ExecutorService used to run jobs for this Connection
         */
        decaf::util::concurrent::ExecutorService* getExecutor() const;

        /**
         * Adds the given Temporary Destination to this Connections collection of known
         * Temporary Destinations.
         *
         * @param destination
         *      The temporary destination that this connection should track.
         */
        void addTempDestination(Pointer<commands::ActiveMQTempDestination> destination);

        /**
         * Removes the given Temporary Destination to this Connections collection of known
         * Temporary Destinations.
         *
         * @param destination
         *      The temporary destination that this connection should stop tracking.
         */
        void removeTempDestination(Pointer<commands::ActiveMQTempDestination> destination);

        /**
         * Removes the given Temporary Destination to this Connections collection of known
         * Temporary Destinations.
         *
         * @param destination
         *      The temporary destination that this connection should remove from the Broker.
         *
         * @throws CMSException if the temporary destination is in use by an active Session.
         */
        void deleteTempDestination(Pointer<commands::ActiveMQTempDestination> destination);

        /**
         * Removes any TempDestinations that this connection has cached, ignoring any exceptions
         * generated because the destination is in use as they should not be removed.  This method
         * is useful for Connection pools that retain connection objects for long durations and
         * want to periodically purge old temporary destination instances this connection is tracking.
         */
        void cleanUpTempDestinations();

        /**
         * Determines whether the supplied Temporary Destination has already been deleted from the
         * Broker.  If watchTopicAdvisories is disabled this method will always return false.
         *
         * @return true if the temporary destination was deleted already.
         */
        bool isDeleted(Pointer<commands::ActiveMQTempDestination> destination) const;

        /**
         * Returns an ArrayList that contains a copy of all Sessions that are
         * currently active in the Connection
         *
         * @return an ArrayList of Sessions active in this connection.
         */
        decaf::util::ArrayList< Pointer<activemq::core::kernels::ActiveMQSessionKernel> > getSessions() const;

    protected:

        /**
         * @return the next available Session Id.
         */
        virtual Pointer<commands::SessionId> getNextSessionId();

        // Sends a oneway disconnect message to the broker.
        void disconnect(long long lastDeliveredSequenceId);

        // Waits for all Consumers to handle the Transport Interrupted event.
        void waitForTransportInterruptionProcessingToComplete();

        // Marks processing complete for a single caller when interruption processing completes.
        void signalInterruptionProcessingComplete();

        // Allow subclasses to access the original Properties object for this connection.
        const decaf::util::Properties& getProperties() const;

        // Process the WireFormatInfo command
        void onWireFormatInfo(Pointer<commands::Command> command);

        // Process the ControlCommand command
        void onControlCommand(Pointer<commands::Command> command);

        // Process the ConnectionControl command
        void onConnectionControl(Pointer<commands::Command> command);

        // Process the ConsumerControl command
        void onConsumerControl(Pointer<commands::Command> command);

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTION_H_*/
