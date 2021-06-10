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
#ifndef _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_
#define _ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_

#include <activemq/util/Config.h>

#include <cms/ConnectionFactory.h>
#include <cms/Connection.h>

#include <activemq/transport/Transport.h>

#include <decaf/net/URI.h>
#include <decaf/util/Properties.h>

namespace activemq {
namespace core {

    using decaf::lang::Pointer;

    class ActiveMQConnection;
    class FactorySettings;
    class PrefetchPolicy;
    class RedeliveryPolicy;

    class AMQCPP_API ActiveMQConnectionFactory : public cms::ConnectionFactory {
    public:

        // Default Broker URI if none specified 'failover:tcp://localhost:61616'
        static const std::string DEFAULT_URI;

    private:

        FactorySettings* settings;

    private:

        ActiveMQConnectionFactory(const ActiveMQConnectionFactory&);
        ActiveMQConnectionFactory& operator=(const ActiveMQConnectionFactory&);

    public:

        ActiveMQConnectionFactory();

        /**
         * Constructor
         *
         * @param uri
         *      The URI of the Broker we are connecting to.
         * @param username
         *      The user name to authenticate with this connection.
         * @param password
         *      The password to authenticate with this connection.
         */
        ActiveMQConnectionFactory(const std::string& uri,
                                  const std::string& username = "",
                                  const std::string& password = "");

        /**
         * Constructor
         *
         * @param uri
         *      The URI of the Broker we are connecting to.
         * @param username
         *      The user name to authenticate with this connection.
         * @param password
         *      The password to authenticate with this connection.
         */
        ActiveMQConnectionFactory(const decaf::net::URI& uri,
                                  const std::string& username = "",
                                  const std::string& password = "");

        virtual ~ActiveMQConnectionFactory();

        /**
         * Creates a connection with the default user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.
         *
         * @return a Connection Pointer
         *
         * @throws CMSException if an error occurs.
         */
        virtual cms::Connection* createConnection();

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The user name and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         *
         * @param username
         *      The user name to authenticate with this connection.
         * @param password
         *      The password to authenticate with this connection.
         *
         * @return a Connection Pointer
         *
         * @throws CMSSecurityException if the user credentials are invalid.
         * @throws CMSException if an error occurs.
         */
        virtual cms::Connection* createConnection(const std::string& username,
                                                  const std::string& password);

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly
         * called.  The username and password values passed here do not
         * change the defaults, subsequent calls to the parameterless
         * createConnection will continue to use the default values that
         * were set in the Constructor.
         *
         * @param username
         *      The user name to authenticate with this connection.
         * @param password
         *      The password to authenticate with this connection.
         * @param clientId
         *      The client Id to assign to connection if "" then a random client
         *      Id is created for this connection.
         *
         * @return a Connection Pointer
         *
         * @throws CMSSecurityException if the user credentials are invalid.
         * @throws CMSException if an error occurs.
         */
        virtual cms::Connection* createConnection(const std::string& username,
                                                  const std::string& password,
                                                  const std::string& clientId);

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
         * Gets the Configured Client Id.
         * @return the clientId.
         */
        std::string getClientId() const;

        /**
         * Sets the Client Id.
         * @param clientId - The new clientId value.
         */
        void setClientId(const std::string& clientId);

        /**
         * Sets the Broker URI that should be used when creating a new connection instance.
         *
         * @param uri
         *      The string form of the Broker URI, this will be converted to a URI object.
         */
        void setBrokerURI(const std::string& uri);

        /**
         * Sets the Broker URI that should be used when creating a new connection instance.
         *
         * @param uri
         *      The URI of the broker that this client will connect to.
         */
        void setBrokerURI(const decaf::net::URI& uri);

        /**
         * Gets the Broker URI that this factory will use when creating a new
         * connection instance.
         * @return brokerURI string
         */
        const decaf::net::URI& getBrokerURI() const;

        /**
         * Set an CMS ExceptionListener that will be set on eat connection once it has been
         * created.  The factory does not take ownership of this pointer, the client must ensure
         * that its lifetime is scoped to the connection that it is applied to.
         *
         * @param listener
         * 		The listener to set on the connection or NULL for no listener.
         */
        virtual void setExceptionListener(cms::ExceptionListener* listener);

        /**
         * Returns the currently set ExceptionListener that will be set on any new Connection
         * instance that is created by this factory.
         *
         * @return a pointer to a CMS ExceptionListener instance or NULL if not set.
         */
        virtual cms::ExceptionListener* getExceptionListener() const;

        /**
         * Set an MessageTransformer instance that is passed on to all Connection objects created from
         * this ConnectionFactory
         *
         * @param transformer
         *      Pointer to the cms::MessageTransformer to set on all newly created Connection objects.
         */
        virtual void setMessageTransformer(cms::MessageTransformer* transformer);

        /**
         * Gets the currently configured MessageTransformer for this ConnectionFactory.
         *
         * @return the pointer to the currently set cms::MessageTransformer.
         */
        virtual cms::MessageTransformer* getMessageTransformer() const;

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
         * Returns whether Message acknowledgments are sent asynchronously meaning no
         * response is required from the broker before the ack completes.
         *
         * @return the sendAcksAsync configured value. (defaults to true)
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
         * Is the Connection created by this factory configured to watch for advisory messages
         * that inform the Connection about temporary destination create / destroy.
         *
         * @return true if Connection's will listen for temporary destination advisory messages.
         */
        bool isWatchTopicAdvisories() const;

        /**
         * Sets whether Connection's created by this factory will listen for advisory messages
         * regarding temporary destination creation and deletion.
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
         * Returns the current value of the always session async option.
         *
         * @return Returns the alwaysSessionAsync configuration setting.
         */
        bool isAlwaysSessionAsync() const;

        /**
         * If this flag is not set 'true' then a separate thread is not used for dispatching messages
         * for each Session in the Connection. However, a separate thread is always used if there
         * is more than one session, or the session isn't in auto acknowledge or duplicates ok mode.
         * By default this value is set to true and session dispatch happens asynchronously.
         *
         * @param alwaysSessionAsync
         * 		The alwaysSessionAsync value to use when creating new sessions.
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

    public:

        /**
         * Creates a connection with the specified user identity. The
         * connection is created in stopped mode. No messages will be
         * delivered until the Connection.start method is explicitly called.
         *
         * @param uri
         *      The URI of the Broker we are connecting to.
         * @param username
         *      The name of the user to authenticate with.
         * @param password
         *      The password for the user to authenticate with.
         * @param clientId
         *      The unique client id to assign to connection, defaults to "".
         *
         * @throw CMSException.
         */
        static cms::Connection* createConnection(const std::string& uri, const std::string& username,
                                                 const std::string& password, const std::string& clientId = "");

    protected:

        /**
         * Create a new ActiveMQConnection instnace using the provided Transport and Properties.
         * Subclasses can override this to control the actual type of ActiveMQConnection that
         * is created.
         *
         * @param transport
         *      The Transport that the Connection should use to communicate with the Broker.
         * @param properties
         *      The Properties that are assigned to the new Connection instance.
         *
         * @return a new ActiveMQConnection pointer instance.
         */
        virtual ActiveMQConnection* createActiveMQConnection(const Pointer<transport::Transport>& transport,
                                                             const Pointer<decaf::util::Properties>& properties);

    private:

        cms::Connection* doCreateConnection(const decaf::net::URI& uri, const std::string& username,
                                            const std::string& password, const std::string& clientId);

        void configureConnection(ActiveMQConnection* connection);

    };

}}

#endif /*_ACTIVEMQ_CORE_ACTIVEMQCONNECTIONFACTORY_H_*/
