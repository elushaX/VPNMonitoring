
from influxdb import InfluxDBClient
import psutil
import time
from datetime import datetime, timedelta

# InfluxDB connection parameters
INFLUXDB_HOST = 'localhost'
INFLUXDB_PORT = 8086
INFLUXDB_USER = 'root'
INFLUXDB_PASSWORD = 'root'
INFLUXDB_DATABASE = 'metrics'

# Connect to InfluxDB
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT, username=INFLUXDB_USER, password=INFLUXDB_PASSWORD)
client.switch_database(INFLUXDB_DATABASE)


class NetworkSampler:
    def __init__(self):
        self.sent_prev = 0
        self.received_prev = 0
        self.device = "eth0"

    def get_network_traffic(self):
        sent = float(psutil.net_io_counters(pernic=True)[self.device][0])
        received = float(psutil.net_io_counters(pernic=True)[self.device][1])
        return sent, received

    def sample(self):
        sent_now, received_now = self.get_network_traffic()

        sent = sent_now - self.sent_prev
        recv = received_now - self.received_prev

        self.sent_prev = sent_now
        self.received_prev = received_now

        return recv, sent


networkSampler = NetworkSampler()


def sample():
    memory_load = psutil.virtual_memory().percent  # Memory usage in percentage
    cpu_load = psutil.cpu_percent(interval=1)  # CPU usage in percentage

    net_in, net_out = networkSampler.sample()

    return {
        "memory_load": memory_load,
        "cpu_load": cpu_load,
        "net_in": net_in,
        "net_out": net_out,
    }


def collect_and_store_metrics():
    while True:
        # Get the current memory and CPU load as a dictionary
        data = sample()
        current_time = datetime.utcnow().isoformat()

        # Construct fields dictionary automatically from the sample data
        fields = {key: value for key, value in data.items()}

        # Create the data points in InfluxDB format
        influx_data = [
            {
                "measurement": "system_metrics",
                "tags": {
                    "host": "localhost"
                },
                "time": current_time,
                "fields": fields  # Automatically constructed fields
            }
        ]

        # Write the data points to InfluxDB
        client.write_points(influx_data)

        # Sleep for 1 second before collecting data again
        time.sleep(1)

if __name__ == '__main__':
    print("Collecting memory and CPU load data, and removing old data from InfluxDB...")
    collect_and_store_metrics()
