
#include "Monitor.hpp"

#include <iostream>


void processPacket(const Packet &packet, bool ssPacket) {
  std::cout << (ssPacket ? "SS" : "") << (packet.incoming ? " IN " : " OUT ") << packet.ip << " " << packet.sizeBytes
            << " bytes (local ip "
            << packet.localIP << ")\n";
}