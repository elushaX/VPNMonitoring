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

def get_network_traffic():
    net_io = psutil.net_io_counters()
    sent = net_io.bytes_sent / (1024)  # Convert to MB
    received = net_io.bytes_recv / (1024)  # Convert to MB
    return sent, received

# Monitor network traffic in a loop
if __name__ == "__main__":
    print(f"{'Time':<10} {'MB Sent':<15} {'MB Received'}")
    sent_prev = 0
    received_prev = 0
    while True:
        sent_now, received_now = get_network_traffic()
        
        sent = sent_now - sent_prev
        recv = received_now - received_prev

        sent_prev = sent_now
        received_prev = received_now

        print(f"{time.strftime('%H:%M:%S'):<10} {sent:<15.2f} {recv:.2f}")

        time.sleep(1)  # Pause for 1 second before getting the next measurement
