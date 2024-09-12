
#include "Monitor.hpp"

#include <iostream>
#include <cstdlib>

void MonitoringDatabase::handlePacket(const Packet &packet, bool ssPacket) {

  totalSpeed.add(packet);

  if (ssPacket) {
    if (connections.find(packet.ip) == connections.end()) {
      connections.insert({packet.ip, Connection{packet.ip}});
    }

    Connection& connection = connections.find(packet.ip)->second;
    connection.speed.add(packet);
  }

  system("clear");

  std::cout << "total: in " << totalSpeed.in.get() / 1024 << " out: " << totalSpeed.out.get() / 1024 << "\n";

  for (auto& [_, con] : connections) {
    std::cout << "connection: " << con.ip << " in " << con.speed.in.get() << " out: " << con.speed.out.get() << "\n";
  }

  if (false) {
    std::cout << (ssPacket ? "SS" : "") << (packet.incoming ? " IN " : " OUT ") << packet.ip << " " << packet.sizeBytes
              << " bytes (local ip "
              << packet.localIP << " : " << packet.localPort << ")\n";
  }
}
