#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;
template <class T>
class MessageQueue
{
public:
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    T receive() {
        std::unique_lock<std::mutex> lock(_mut);
        _cond.wait(lock, [this] { return !_queue.empty(); });
        T message = std::move(_queue.back());

        _queue.pop_back();
        return message;
    }

    void send(T &&message)
    {
        // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
        // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
        std::lock_guard<std::mutex> lock(_mut);

        _queue.push_back(std::move(message));
        _cond.notify_one();
    }

private:
    std::deque<T> _queue;
    std::condition_variable _cond;
    std::mutex _mut;
};



// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. 
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ 
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that 
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. 
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. 

enum TrafficLightPhase {red, green};
class TrafficLight : public TrafficObject
{
public:

    // constructor / desctructor
    TrafficLight();
    ~TrafficLight() {};


    // getters / setters
    TrafficLightPhase getCurrentPhase();
    int rand();

    // typical behaviour methods
    void togglePhase();
    void waitForGreen();
    void simulate();


private:
    // typical behaviour methods
    void cycleThroughPhases();

    TrafficLightPhase _currentPhase = red;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.
    MessageQueue<TrafficLightPhase> queue;
    std::condition_variable _condition;
    std::mutex _mutex;
    int _timer;
};

#endif