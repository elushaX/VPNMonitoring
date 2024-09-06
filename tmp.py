import subprocess

def get_client_ips():
    script_path = "../shadowsocks/listClients.sh"
    try:
        result = subprocess.run(['bash', script_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if result.returncode != 0:
            print(f"Error executing script: {result.stderr}")
            return []
        output_lines = result.stdout.splitlines()
        return output_lines
    except Exception as e:
        print(f"An error occurred: {e}")
        return []

output_lines = get_client_ips()

for line in output_lines:
    print(line)

import psutil
import time

class NetworkSampler:
    def __init__(self):
        self.sent_prev = 0
        self.received_prev = 0

    def get_network_traffic(self):
        net_io = psutil.net_io_counters()
        sent = net_io.bytes_sent / (1024)
        received = net_io.bytes_recv / (1024)
        return sent, received

    def sample(self):
            sent_now, received_now = self.get_network_traffic()
            
            sent = sent_now - self.sent_prev
            recv = received_now - self.received_prev

            self.sent_prev = sent_now
            self.received_prev = received_now
            
            return recv, -sent
            print(f"{sent:.2f} {recv:.2f}")

sampler = NetworkSampler()

for i in range(1000):
    sampler.sample()
    time.sleep(1)
