#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <utility>
#include <vector>
#include <map>

#include <pqxx/pqxx>

class Database {
private:
  std::string connection_str;
  pqxx::connection* connection;

public:
  explicit Database(std::string conn_str) : connection_str(std::move(conn_str)) {
    connection = new pqxx::connection(connection_str);

    if (connection->is_open()) {
      std::cout << "Connected to database successfully." << std::endl;
    } else {
      std::cerr << "Could not connect to the database." << std::endl;
    }
  }

  ~Database() {
    // Close the connection
    delete connection;
  }

  // Method to check if an IP is present in the 'block_ips' table
  bool isIpBlocked(const std::string& ip_address) {
    try {
      // Start a non-transactional object
      pqxx::work txn(*connection);

      // SQL query to check if the IP address exists in the 'block_ips' table
      std::string query = "SELECT EXISTS(SELECT 1 FROM block_ips WHERE ip = " + txn.quote(ip_address) + ");";

      // Execute the query and store the result
      pqxx::result result = txn.exec(query);

      // Check if the IP exists
      bool ip_exists = result[0][0].as<bool>();

      // Commit the transaction
      txn.commit();

      // Return the result
      return ip_exists;

    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return false;
    }
  }
};

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
    std::string direction = (isBlocked(src_ip)) ? "INCOMING" : "OUTGOING";

    // Print packet information
    std::cout << "Packet size: " << pkthdr->len << " bytes\n";
    std::cout << "Source IP: " << src_ip << " Destination IP: " << dst_ip << std::endl;
    std::cout << "Direction: " << direction << std::endl;

    // Drop packet if source IP is in blocked_ip_list
    if (isBlocked(src_ip)) {
        std::cout << "Packet from " << src_ip << " is dropped." << std::endl;
        return;  // Dropping the packet
    }

    // If TCP, get source and destination ports
    if (ipHeader->ip_p == IPPROTO_TCP) {
        tcpHeader = (struct tcphdr*)(packet + 14 + (ipHeader->ip_hl * 4));  // Skip IP header
        std::cout << "Source Port: " << ntohs(tcpHeader->source) << " Destination Port: " << ntohs(tcpHeader->dest) << std::endl;
    }

    std::cout << "---------------------------------------------\n";
}

int main(int argc, char* argv[]) {
    char* dev;  // Network device to capture from
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle;

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

