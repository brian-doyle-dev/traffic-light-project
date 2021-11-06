#include <iostream>
#include <random>
#include <cstdlib>
#include "TrafficLight.h"

using namespace std::chrono_literals;

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
    _timer = rand();
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
    while(true)
    {
 
        TrafficLightPhase phase = queue.receive();
        std::cout << "Receive Message: " << ((phase == green) ? "Green" : "Red") << std::endl;
        if (phase == green)
        {
            return;
        }
    }
}

void TrafficLight::togglePhase()
{
        std::unique_lock<std::mutex> lck(_mutex);
    if (_currentPhase == red)
    {
        _currentPhase = green;
    }
    else
    {
        _currentPhase = red;
    }
} 

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    std::unique_lock<std::mutex> lck(_mutex);
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

int TrafficLight::rand()
{
    return 4000 + (std::rand() + 1) / ((static_cast<long>(RAND_MAX) + 1)/ 2000);
}
        
// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
    while(true)
    {    
        _timer--;
        if (_timer == 0)
        {
            _timer = 3000; //rand();
            togglePhase();
            TrafficLightPhase phase = getCurrentPhase();
            std::cout << "Send Message: " << ((phase == green) ? "Green" : "Red") << std::endl;
            queue.send(std::move(getCurrentPhase()));
        }

        std::this_thread::sleep_for(1ms);

    }
}

