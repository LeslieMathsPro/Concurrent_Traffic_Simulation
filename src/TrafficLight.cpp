#include <iostream>
#include <random>
#include "TrafficLight.h"


template<class T>
void MessageQueue<T>::send(T &&traffic_light_phase) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.clear(); 
    _queue.emplace_back(std::move(traffic_light_phase));
    _condition.notify_one();
}


template<class T>
T MessageQueue<T>::receive() {
    std::unique_lock<std::mutex> lock(_mutex);
    _condition.wait(lock, [this] { return !_queue.empty(); });
    T message = std::move(_queue.front());
    _queue.pop_front();
    return message;
}

void TrafficLight::cycleThroughPhases() {
    std::random_device _rd;
    std::mt19937 rng(_rd());
    std::uniform_int_distribution<int> uni(4000, 6000);
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(uni(rng)));
        _currentPhase = (_currentPhase == TrafficLightPhase::red) ? TrafficLightPhase::green : TrafficLightPhase::red;
        _messageQueue.send(std::move(_currentPhase));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void TrafficLight::simulate() {
    std::thread t(&TrafficLight::cycleThroughPhases, this);
    threads.emplace_back(std::move(t));
}

void TrafficLight::waitForGreen() {
    while (true) {
        if (_messageQueue.receive() == TrafficLightPhase::green) {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase() {
    return _currentPhase;
}

TrafficLight::TrafficLight() {
    _currentPhase = TrafficLightPhase::red;
}

