
#include "Monitor.hpp"

#include <iostream>
#include <cstdlib>

Monitor::Monitor() : backgroundThread([this]() { this->backgroundLoop(); }) {}

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

  if (false) {
    std::cout << (ssPacket ? "SS" : "") << (packet.incoming ? " IN " : " OUT ") << packet.ip << " "
              << packet.sizeBytes
              << " bytes (local ip "
              << packet.localIP << " : " << packet.localPort << ")\n";
  }
}

void Monitor::sample() {
  system("clear");

  std::cout << "total: in " << totalSpeed.in.get() / 1024 << " out: " << totalSpeed.out.get() / 1024 << "\n";

  for (auto &[_, con]: connections) {
    std::cout << "connection: " << con.ip << " in " << con.speed.in.get() / 1024 << " out: " << con.speed.out.get() / 1024 << "\n";
  }
}