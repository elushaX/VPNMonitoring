#pragma once

#include <string>

struct Packet {
  std::size_t sizeBytes = 0;
  bool incoming = false;
  std::string ip;
  std::string localIP;
};

void processPacket(const Packet& packet, bool ssPacket);