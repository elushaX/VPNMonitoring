
#include "Monitor.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

MonitoringDatabase monitor;

void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet);

const std::string shadowSocksPort = "2338";
const std::string shadowSocksIP = "185.238.170.251";


// Get local IP addresses (both IPv4 and IPv6)
std::vector<std::string> getLocalIPs() {
  std::vector<std::string> ips;
  struct ifaddrs *ifAddrStruct = nullptr;
  struct ifaddrs *ifa = nullptr;
  void *tmpAddrPtr = nullptr;

  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != nullptr; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == nullptr) continue;

    // Check if the address is IPv4 or IPv6
    if (ifa->ifa_addr->sa_family == AF_INET) {  // IPv4
      tmpAddrPtr = &((struct sockaddr_in *) ifa->ifa_addr)->sin_addr;
      char addressBuffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      ips.push_back(addressBuffer);
    } else if (ifa->ifa_addr->sa_family == AF_INET6) {  // IPv6
      tmpAddrPtr = &((struct sockaddr_in6 *) ifa->ifa_addr)->sin6_addr;
      char addressBuffer[INET6_ADDRSTRLEN];
      inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
      ips.push_back(addressBuffer);
    }
  }

  if (ifAddrStruct != nullptr) freeifaddrs(ifAddrStruct);
  return ips;
}

// Check if an IP is local
bool isLocalIP(const std::string &ip) {
  std::vector<std::string> localIPs = getLocalIPs();
  return std::find(localIPs.begin(), localIPs.end(), ip) != localIPs.end();
}

// Callback function to process each packet
void packetHandler(u_char *userData, const struct pcap_pkthdr *pkthdr, const u_char *packet) {

  struct ip *ipHeader;
  struct tcphdr *tcpHeader;
  char src_ip[INET_ADDRSTRLEN];
  char dst_ip[INET_ADDRSTRLEN];

  // Get IP header
  ipHeader = (struct ip *) (packet + 14);  // Skip Ethernet header (14 bytes)
  inet_ntop(AF_INET, &(ipHeader->ip_src), src_ip, INET_ADDRSTRLEN);
  inet_ntop(AF_INET, &(ipHeader->ip_dst), dst_ip, INET_ADDRSTRLEN);

  Packet ssPacket;
  ssPacket.sizeBytes = pkthdr->len;

  if (isLocalIP(src_ip)) {
    ssPacket.incoming = false;
    ssPacket.ip = dst_ip;
    ssPacket.localIP = src_ip;
  } else {
    ssPacket.incoming = true;
    ssPacket.ip = src_ip;
    ssPacket.localIP = dst_ip;
  }

  bool processed = false;

  if (ipHeader->ip_p == IPPROTO_TCP) {
    tcpHeader = (struct tcphdr *) (packet + 14 + (ipHeader->ip_hl * 4));  // Skip IP header
    auto port = std::to_string(ntohs(ssPacket.incoming ? tcpHeader->dest : tcpHeader->source));

    ssPacket.localPort = port;

    if (shadowSocksPort == port) {
      monitor.handlePacket(ssPacket, true);
      processed = true;
    }
  }

  if (!processed) {
    monitor.handlePacket(ssPacket, false);
  }
}

int main(int argc, char *argv[]) {
  char *dev;  // Network device to capture from
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t *handle;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <network_interface>\n";
    return 1;
  }

  dev = argv[1];  // Interface provided via command line

  // Open the device for capturing
  handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
  if (handle == nullptr) {
    std::cerr << "Could not open device " << dev << ": " << errbuf << std::endl;
    return 2;
  }

  // Capture packets indefinitely, passing each one to the packetHandler function
  pcap_loop(handle, 0, packetHandler, nullptr);

  // Close the session
  pcap_close(handle);
  return 0;
}