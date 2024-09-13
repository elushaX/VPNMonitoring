
#include "Monitor.hpp"

#include <iostream>
#include <cstdlib>

Monitor::Monitor() : pgDatabase() {
  backgroundThread = std::thread(([this]() { this->backgroundLoop(); }));
}

Monitor::~Monitor() {
  {
    std::lock_guard<std::mutex> guard(lock);
    active = false;
  }

  backgroundThread.join();
}

void Monitor::backgroundLoop() {
  bool activeCopy = true;

  while (activeCopy) {

    {
      std::lock_guard<std::mutex> guard(lock);
      activeCopy = active;

      sample();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void Monitor::handlePacket(const Packet &packet, bool ssPacket) {
  {
    std::lock_guard<std::mutex> guard(lock);
    totalSpeed.add(packet);

    if (ssPacket) {
      if (connections.find(packet.ip) == connections.end()) {
        connections.insert({packet.ip, Connection{packet.ip}});
      }

      Connection& connection = connections.find(packet.ip)->second;
      connection.speed.add(packet);
    }
  }
}

void Monitor::sample() {
  pgDatabase.recordTotal(totalSpeed.in.get(), totalSpeed.out.get());
  for (auto &[_, con]: connections) {
    pgDatabase.recordConnection(con.ip, con.speed.in.get(), con.speed.out.get());
  }
}