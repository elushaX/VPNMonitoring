
#include <iostream>
#include <pcap.h>

void packetHandler(u_char* userData, const struct pcap_pkthdr* pkthdr, const u_char* packet);

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

