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

#include "MutexTest.h"

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/Concurrent.h>
#include <decaf/util/concurrent/Mutex.h>
#include <decaf/util/Random.h>

#include <decaf/internal/util/concurrent/SynchronizableImpl.h>

#include <time.h>

using namespace std;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::lang::exceptions;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace decaf::internal::util::concurrent;

///////////////////////////////////////////////////////////////////////////////
void MutexTest::testConstructor() {

    Mutex mutex;

    mutex.lock();
    mutex.unlock();

    CPPUNIT_ASSERT( mutex.tryLock() == true );

    mutex.unlock();

    try {
        synchronized(&mutex) {
            throw Exception();
        }
    } catch (...) {}

    CPPUNIT_ASSERT(!mutex.isLocked());
}

///////////////////////////////////////////////////////////////////////////////
class MyThread : public lang::Thread, public SynchronizableImpl{
public:

    volatile int value;
    MyThread() : value(0) {}
    virtual ~MyThread(){}

    virtual void run(){
        {
            Lock lock (this);

            value = value * 25;
        }
    }

};

///////////////////////////////////////////////////////////////////////////////
void MutexTest::testSimpleThread() {
    MyThread test;

    synchronized(&test){

        test.start();

        for( int ix=0; ix<100; ix++ ){
            test.value += 1;
        }
    }

    test.join();

    CPPUNIT_ASSERT( test.value == 2500 );
}

///////////////////////////////////////////////////////////////////////////////
class MyWaitingThread : public lang::Thread, public SynchronizableImpl{
public:

    volatile int value;
    volatile bool started;
    MyWaitingThread() : value(0), started(false) {}
    virtual ~MyWaitingThread(){}

    virtual void run(){

        try {

            synchronized( this ) {
                this->started = true;
                this->wait();
                value = value * 25;
            }

        } catch( lang::Exception& ex ) {
            ex.setMark( __FILE__, __LINE__ );
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
void MutexTest::testWait(){

    try
    {
        MyWaitingThread test;
        test.start();

        while( !test.started ) {
            Thread::sleep(1);
        }

        synchronized( &test )
        {
            for( int ix=0; ix<100; ix++ ){
                test.value += 1;
            }

            test.notify();
        }

        test.join();

        CPPUNIT_ASSERT( test.value == 2500 );

    } catch( lang::Exception& ex ) {
        ex.setMark( __FILE__, __LINE__ );
        CPPUNIT_ASSERT( false );
    }
}

///////////////////////////////////////////////////////////////////////////////
class MyTimedWaitingThread : public lang::Thread, public SynchronizableImpl{
public:

    volatile int value;
    volatile bool started;

    MyTimedWaitingThread() : value(0), started(false) {}
    virtual ~MyTimedWaitingThread(){}

    virtual void run(){

        try {
            synchronized( this ) {

                this->started = true;
                this->wait(1100);
                value = 666;
            }
        } catch( lang::Exception& ex ) {
            ex.setMark( __FILE__, __LINE__ );
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
void MutexTest::testTimedWait(){

    try {

        MyTimedWaitingThread test;
        time_t startTime = time( NULL );
        test.start();
        test.join();
        time_t endTime = time( NULL );

        time_t delta = endTime - startTime;

        CPPUNIT_ASSERT( delta >= 1 && delta <= 2 );

        {
            Mutex test;
            test.lock();

            CPPUNIT_ASSERT_THROW_MESSAGE(
                "Should Throw an IllegalArgumentException",
                test.wait( -1, -1 ),
                IllegalArgumentException );

            CPPUNIT_ASSERT_THROW_MESSAGE(
                "Should Throw an IllegalArgumentException",
                test.wait( 1, 9999999 ),
                IllegalArgumentException );

            CPPUNIT_ASSERT_THROW_MESSAGE(
                "Should Throw an IllegalArgumentException",
                test.wait( 0, -1 ),
                IllegalArgumentException );

        }

    } catch(lang::Exception& ex) {
        std::cout << ex.getMessage() << std::endl;
        CPPUNIT_ASSERT( false );
    }
}

///////////////////////////////////////////////////////////////////////////////
class MyNotifiedThread : public lang::Thread, public Synchronizable{
public:

    volatile bool done;
    Mutex* mutex;
    Mutex* started;
    Mutex* completed;

private:

    MyNotifiedThread(const MyNotifiedThread&);
    MyNotifiedThread& operator= (const MyNotifiedThread&);

public:

    int value;
    MyNotifiedThread(Mutex* mutex, Mutex* started, Mutex* completed ) :
        done(false), mutex(mutex), started(started), completed(completed), value() {
    }
    virtual ~MyNotifiedThread(){}

    virtual void lock() {
        mutex->lock();
    }

    virtual bool tryLock() {
        return mutex->tryLock();
    }

    virtual void unlock() {
        mutex->unlock();
    }

    virtual void wait() {

        mutex->wait();
    }

    virtual void wait( long long millisecs ) {

        mutex->wait( millisecs );
    }

    virtual void wait( long long millisecs, int nanos ) {

        mutex->wait( millisecs, nanos );
    }

    virtual void notify() {

        mutex->notify();
    }

    virtual void notifyAll() {

        mutex->notifyAll();
    }

    virtual void run(){

        try {
            done = false;
            synchronized( this ) {
                synchronized( started ) {
                    started->notify();
                }

                this->wait();
                done = true;

                synchronized( completed ) {
                    completed->notify();
                }
            }
        } catch( lang::Exception& ex ) {
            ex.setMark( __FILE__, __LINE__ );
        }
    }
};

///////////////////////////////////////////////////////////////////////////////
void MutexTest::testNotify() {

    try{

        Mutex mutex;
        Mutex started;
        Mutex completed;

        const int numThreads = 30;
        MyNotifiedThread* threads[numThreads];

        // Create and start all the threads.
        for( int ix=0; ix<numThreads; ++ix ){
            threads[ix] = new MyNotifiedThread( &mutex, &started, &completed );
            threads[ix]->start();
        }

        synchronized( &started ) {
            int count = 0;

            while( count < ( numThreads ) ) {
                started.wait( 40 );
                count++;
            }
        }

        synchronized(&mutex ) {
            mutex.notify();
        }

        Thread::sleep( 1200 );

        int counter = 0;
        for( int ix=0; ix<numThreads; ++ix ){
            if( threads[ix]->done ){
                counter++;
            }
        }

        // Make sure only 1 thread was notified.
        CPPUNIT_ASSERT( counter == 1 );

        synchronized( &mutex ) {
            // Notify all threads.
            for( int ix=0; ix<numThreads-1; ++ix ){
                mutex.notify();
            }
        }

        synchronized( &started ){
            int count = 0;

            while( count < ( numThreads ) ) {
                started.wait( 40 );
                count++;
            }
        }

        int numComplete = 0;
        for( int ix=0; ix<numThreads; ++ix ){
            if( threads[ix]->done ){
                numComplete++;
            }
        }
        CPPUNIT_ASSERT( numComplete == numThreads );

        synchronized( &mutex ) {
            mutex.wait( 5 );
        }

        synchronized( &mutex ) {
            mutex.notifyAll();
        }

        // Delete all the threads.
        for( int ix=0; ix<numThreads; ++ix ){
            threads[ix]->join();
            delete threads[ix];
        }

    }catch( lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
        CPPUNIT_ASSERT( false );
    }
}

///////////////////////////////////////////////////////////////////////////////
void MutexTest::testNotifyAll()
{
    try{
        Mutex mutex;
        Mutex started;
        Mutex completed;

        const int numThreads = 100;
        MyNotifiedThread* threads[numThreads];

        // Create and start all the threads.
        for( int ix=0; ix<numThreads; ++ix ){
            threads[ix] = new MyNotifiedThread( &mutex, &started, &completed );
            threads[ix]->start();
        }

        synchronized( &started )
        {
            int count = 0;

            while( count < ( numThreads ) )
            {
                started.wait( 30 );
                count++;
            }
        }

        for( int ix=0; ix<numThreads; ++ix )
        {
            if( threads[ix]->done == true ){
                printf("threads[%d] is done prematurely\n", ix );
            }
            CPPUNIT_ASSERT( threads[ix]->done == false );
        }

        // Notify all threads.
        synchronized( &mutex ){
           mutex.notifyAll();
        }

        synchronized( &completed )
        {
            int count = 0;

            while( count < ( numThreads ) )
            {
                completed.wait( 30 );
                count++;
            }
        }

        int numComplete = 0;
        for( int ix=0; ix<numThreads; ++ix ){
            if( threads[ix]->done ){
                numComplete++;
            }
        }
        //printf("numComplete: %d, numThreads: %d\n", numComplete, numThreads );
        CPPUNIT_ASSERT( numComplete == numThreads );

        // Delete all the threads.
        for( int ix=0; ix<numThreads; ++ix ){
            threads[ix]->join();
            delete threads[ix];
        }

    }catch( lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
        CPPUNIT_ASSERT( false );
    }
}

///////////////////////////////////////////////////////////////////////////////
class MyRecursiveLockThread : public lang::Thread, public Synchronizable{
public:

    volatile bool done;
    Mutex* mutex;
    int index;

private:

    MyRecursiveLockThread(const MyRecursiveLockThread&);
    MyRecursiveLockThread& operator= (const MyRecursiveLockThread&);

public:

    volatile int value;
    MyRecursiveLockThread(Mutex* mutex, int index) : done(false), mutex(mutex), index(index), value() {}

    virtual ~MyRecursiveLockThread(){}

    virtual void lock() {
        mutex->lock();
    }

    virtual bool tryLock() {
        return mutex->tryLock();
    }

    virtual void unlock() {
        mutex->unlock();
    }

    virtual void wait()  {

        mutex->wait();
    }

    virtual void wait( long long millisecs ) {

        mutex->wait( millisecs );
    }

    virtual void wait( long long millisecs, int nanos ) {

        mutex->wait( millisecs, nanos );
    }

    virtual void notify() {

        mutex->notify();
    }

    virtual void notifyAll() {

        mutex->notifyAll();
    }

    virtual void run(){

        try {

            done = false;
            synchronized(this) {
                synchronized(this) {
                    this->wait();
                    done = true;
                }
            }

        } catch(lang::Exception& ex) {
            ex.setMark( __FILE__, __LINE__ );
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
void MutexTest::testRecursiveLock() {

    try {

        Mutex mutex;

        const int numThreads = 2;
        MyRecursiveLockThread* threads[numThreads];

        // Create and start all the threads.
        for( int ix=0; ix<numThreads; ++ix ){
            threads[ix] = new MyRecursiveLockThread( &mutex, ix );
            threads[ix]->start();
        }

        // Sleep so all the threads can get to the wait.
        Thread::sleep( 2000 );

        for( int ix=0; ix<numThreads; ++ix ){
            if( threads[ix]->done == true ){
                std::cout << "threads[" << ix
                          << "] is done prematurely\n";
            }
            CPPUNIT_ASSERT( threads[ix]->done == false );
        }

        // Notify all threads.
        synchronized( &mutex ) {
            synchronized( &mutex ) {
                mutex.notifyAll();
            }
        }

        // Sleep to give the threads time to wake up.
        Thread::sleep( 2000 );

        for( int ix=0; ix<numThreads; ++ix ){
            if( threads[ix]->done != true ){
                std::cout<< "threads[" << ix << "] is not done\n";
            }
            CPPUNIT_ASSERT( threads[ix]->done == true );
        }

        // Delete all the threads.
        for( int ix=0; ix<numThreads; ++ix ){
            threads[ix]->join();
            delete threads[ix];
        }

    }catch( lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
        CPPUNIT_ASSERT( false );
    }
}

///////////////////////////////////////////////////////////////////////////////
class MyDoubleLockThread : public lang::Thread {
public:

    volatile bool done;
    Mutex* mutex1;
    Mutex* mutex2;

private:

    MyDoubleLockThread(const MyDoubleLockThread&);
    MyDoubleLockThread& operator= (const MyDoubleLockThread&);

public:

    volatile int value;
    MyDoubleLockThread(Mutex* mutex1, Mutex* mutex2) :
        done(false), mutex1(mutex1), mutex2(mutex2), value(0) {
    }

    virtual ~MyDoubleLockThread(){}

    virtual void run() {

        try {

            done = false;
            synchronized(mutex1) {
                synchronized(mutex2) {
                    mutex2->wait();
                    done = true;
                }
            }

        } catch(lang::Exception& ex) {
            ex.setMark( __FILE__, __LINE__ );
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
void MutexTest::testDoubleLock() {

    try {

        Mutex mutex1;
        Mutex mutex2;

        MyDoubleLockThread thread(&mutex1, &mutex2);

        thread.start();

        // Let the thread get both locks
        Thread::sleep( 300 );

        // Lock mutex 2, thread is waiting on it
        synchronized( &mutex2 ) {
           mutex2.notify();
        }

        // Let the thread die
        thread.join();

        CPPUNIT_ASSERT( thread.done );

    }catch( lang::Exception& ex ){
        ex.setMark( __FILE__, __LINE__ );
        CPPUNIT_ASSERT( false );
    }
}

///////////////////////////////////////////////////////////////////////////////
class MyStoppableThread : public lang::Runnable {
public:

    volatile bool started;
    volatile bool closed;
    Mutex mutex;
    lang::Thread* thread;
    util::Random rand;

private:

    MyStoppableThread(const MyStoppableThread&);
    MyStoppableThread& operator= (const MyStoppableThread&);

public:

    MyStoppableThread() : started(false), closed(false), mutex(), thread(), rand() {}

    virtual ~MyStoppableThread(){ close(); }

    virtual void start(){
        synchronized( &mutex ) {

            if( closed || started ) {
                return;
            }

            started = true;

            // Don't create the thread unless we need to.
            if( thread == NULL ) {
                thread = new lang::Thread( this );
                thread->start();
            }

            mutex.notifyAll();
        }
    }

    virtual void stop() {
        synchronized( &mutex ) {

            if( closed || !started ) {
                return;
            }

            // Set the state to stopped.
            started = false;

            // Wakeup the thread so that it can acknowledge the stop request.
            mutex.notifyAll();

            // Wait for the thread to notify us that it has acknowledged
            // the stop request.
            mutex.wait();
        }
    }

    virtual void close() {
        synchronized( &mutex ) {

            closed = true;
            mutex.notifyAll();
        }

        if( thread != NULL ) {
            thread->join();
            delete thread;
            thread = NULL;
        }
    }

    virtual bool isStarted() const {
        return started;
    }

    virtual void run(){
        try {

            while( true ) {

                lang::Thread::sleep( rand.nextInt( 100 ) );

                synchronized( &mutex ) {

                    // If we're closing down, exit the thread.
                    if( closed ) {
                        return;
                    }

                    // When told to stop, the calling thread will wait for a
                    // responding notification, indicating that we have acknowledged
                    // the stop command.
                    if( !isStarted() ) {
                        mutex.notifyAll();

                        if( !closed ) {
                            // Wait for more data or to be woken up.
                            mutex.wait();
                        }
                    }
                }
            }
        } catch(...) {
            CPPUNIT_ASSERT( false );
        }
    }
};

////////////////////////////////////////////////////////////////////////////////
void MutexTest::testStressMutex(){

    MyStoppableThread tester;

    tester.start();

    CPPUNIT_ASSERT( tester.isStarted() );

    for( int i = 0; i < 100; ++i ) {
        tester.stop();
        tester.start();
    }

    CPPUNIT_ASSERT( true );
}
