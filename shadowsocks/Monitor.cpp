
#include <iostream>

#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

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

  // Check if packet is incoming or outgoing (assuming you have access to the local IP address)
  std::string direction = "UNKNOWN";

  // Print packet information
  std::cout << "Packet size: " << pkthdr->len << " bytes\n";
  std::cout << "Source IP: " << src_ip << " Destination IP: " << dst_ip << std::endl;
  std::cout << "Direction: " << direction << std::endl;

  // Drop packet if source IP is in blocked_ip_list
  if (false) {
    std::cout << "Packet from " << src_ip << " is dropped." << std::endl;
    return;  // Dropping the packet
  }

  // If TCP, get source and destination ports
  if (ipHeader->ip_p == IPPROTO_TCP) {
    tcpHeader = (struct tcphdr*)(packet + 14 + (ipHeader->ip_hl * 4));  // Skip IP header
    std::cout << "Source Port: " << ntohs(tcpHeader->source) << " Destination Port: " << ntohs(tcpHeader->dest) << std::endl;
  } else {
    std::cout << "unknown protocol\n";
  }

  std::cout << "---------------------------------------------\n";
}