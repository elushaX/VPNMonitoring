#pragma once

#include "Timings.hpp"

#include <string>
#include <map>
#include <mutex>
#include <thread>

using IP = std::string;
using PORT = std::string;

struct Packet {
  std::size_t sizeBytes = 0;
  bool incoming = false;
  IP ip;
  IP localIP;
  PORT localPort;
};

struct NetworkSpeedTracker {
  FlowCount in;
  FlowCount out;

  void add(const Packet& packet) {
    if (packet.incoming) in.add(packet.sizeBytes);
    else out.add(packet.sizeBytes);
  }
};

struct Connection {
  IP ip;
  NetworkSpeedTracker speed;
  TimeStamp firstSeen;
};

class Monitor {
public:
  Monitor();
  ~Monitor();

  void handlePacket(const Packet& packet, bool ssPacket);

private:
  void backgroundLoop();
  void sample();

private:
  std::thread backgroundThread;

  std::mutex lock;
  bool active = true;

  NetworkSpeedTracker totalSpeed;
  std::map<IP, Connection> connections;
};
