
#include <iostream>

#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

struct Packet {
  uint sizeBytes = 0;
  bool incoming = false;
  std::string ip;
};

void processShadowSocksPackets(const Packet& packet) {
  std::cout << (packet.incoming ? "IN " : "OUT ") << packet.ip << " " << packet.sizeBytes << " bytes\n";
}

const std::string shadowSocksPort = "2338";

// Callback function to process each packet
void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {

  struct ip* ipHeader;
  struct tcphdr* tcpHeader;
  char src_ip[INET_ADDRSTRLEN];
  char dst_ip[INET_ADDRSTRLEN];

  // Get IP header
  ipHeader = (struct ip*)(packet + 14);  // Skip Ethernet header (14 bytes)
  inet_ntop(AF_INET, &(ipHeader->ip_src), src_ip, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &(ipHeader->ip_dst), dst_ip, INET_ADDRSTRLEN);

  Packet ssPacket;
  ssPacket.sizeBytes = pkthdr->len;

  if (std::string(src_ip).find("192.168") == std::string::npos) {
    ssPacket.incoming = true;
    ssPacket.ip = dst_ip;
  } else if (std::string(dst_ip).find("192.168") == std::string::npos) {
    ssPacket.incoming = false;
    ssPacket.ip = src_ip;
  } else {
    std::cout << "Error Parsing data packet\n";
  }

  if (ipHeader->ip_p == IPPROTO_TCP) {
    tcpHeader = (struct tcphdr*)(packet + 14 + (ipHeader->ip_hl * 4));  // Skip IP header

    if (shadowSocksPort == std::to_string(ntohs(ssPacket.incoming ? tcpHeader->dest : tcpHeader->source))) {
      processShadowSocksPackets(ssPacket);
    }
  }
}