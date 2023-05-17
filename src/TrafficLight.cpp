#include "TrafficLight.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>

template <typename T> T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> lock(_mutex);
  _condition.wait(lock, [this] { return !_queue.empty(); });
  T msg = std::move(_queue.back());
  _queue.pop_back();

  return msg;
}

template <typename T> void MessageQueue<T>::send(T &&msg) {
  std::lock_guard<std::mutex> lock(_mutex);
  _queue.clear();
  _queue.emplace_back(std::move(msg));
  _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() {
  _messages = std::shared_ptr<MessageQueue<TrafficLightPhase>>(
      new MessageQueue<TrafficLightPhase>());
  _currentPhase = TrafficLightPhase::red;
};

void TrafficLight::waitForGreen() {
  while (1) {
    auto trafficLightPhase = _messages->receive();

    if (trafficLightPhase == TrafficLightPhase::green) {
      return;
    }
  }
}

TrafficLightPhase TrafficLight::getCurrentPhase() { return _currentPhase; }

void TrafficLight::simulate() {
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// Change traffic lights from Green to Red or vice versa
void TrafficLight::changeTrafficLights() {
  switch (this->_currentPhase) {
  case TrafficLightPhase::red:
    this->_currentPhase = TrafficLightPhase::green;
    break;
  case TrafficLightPhase::green:
    this->_currentPhase = TrafficLightPhase::red;
    break;
  }

  _messages->send(std::move(_currentPhase));
}
// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases() {

  int loopCycle = 1;
  while (1) {
    unsigned int seed =
        std::chrono::high_resolution_clock::now().time_since_epoch().count();
    auto rnd = std::mt19937(seed);

    std::uniform_int_distribution<> randomRange(4, 6);
    int result = randomRange(rnd);
    std::this_thread::sleep_for(std::chrono::seconds(result));

    if (loopCycle == 2) {
      changeTrafficLights();
      loopCycle = 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    } else
      loopCycle += 1;
  }
}