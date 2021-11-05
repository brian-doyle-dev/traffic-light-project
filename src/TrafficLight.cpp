#include <iostream>
#include <random>
#include <cstdlib>
#include "TrafficLight.h"

using namespace std::chrono_literals;

/* Implementation of class "MessageQueue" */

// Implemented in TrafficLight.h". Was in MessageQueue.h

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
        std::cout << "Waiting for Green\n";
        TrafficLightPhase phase = queue.receive();
        if (phase == green)
        {
            return;
        }
    }
}

void TrafficLight::togglePhase()
{
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
            _timer = rand();
            togglePhase();
            queue.send(std::move(getCurrentPhase()));
        }

        std::this_thread::sleep_for(1ms);

    }
}

