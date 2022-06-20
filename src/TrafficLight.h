#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum TrafficLightPhase {
    red, green
};

template<class T>
class MessageQueue {
public:

    void send(T &&traffic_light_phase);

    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};


class TrafficLight : public TrafficObject {
public:
    TrafficLight();

    void waitForGreen();

    void simulate() override;


    TrafficLightPhase getCurrentPhase();

private:
    void cycleThroughPhases();

    TrafficLightPhase _currentPhase;
    std::condition_variable _condition;
    std::mutex _mutex;
    MessageQueue<TrafficLightPhase> _messageQueue;
};

#endif