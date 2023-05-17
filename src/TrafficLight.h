#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "TrafficObject.h"
#include <condition_variable>
#include <deque>
#include <mutex>

enum TrafficLightPhase { red, green };

// forward declarations to avoid include cycle
class Vehicle;

template <class T> class MessageQueue {
public:
  void send(T &&msg);
  T receive();

private:
  std::deque<TrafficLightPhase> _queue;
  std::mutex _mutex;
  std::condition_variable _condition;
};

class TrafficLight : TrafficObject {
public:
  // constructor / desctructor
  TrafficLight();

  // getters / setters

  // typical behaviour methods
  void simulate();
  void waitForGreen();
  TrafficLightPhase getCurrentPhase();

private:
  // typical behaviour methods
  void cycleThroughPhases();
  void changeTrafficLights();

  std::shared_ptr<MessageQueue<TrafficLightPhase>> _messages;
  std::condition_variable _condition;
  std::mutex _mutex;
  TrafficLightPhase _currentPhase;
};

#endif