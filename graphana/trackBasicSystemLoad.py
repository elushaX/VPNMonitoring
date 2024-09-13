
from common import *

from influxdb import InfluxDBClient
import psutil
import time
from datetime import datetime, timedelta

INFLUXDB_DATABASE = 'metrics'

# Connect to InfluxDB
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT, username=INFLUXDB_USER, password=INFLUXDB_PASSWORD)
client.switch_database(INFLUXDB_DATABASE)


class NetworkSampler:
    def __init__(self):
        self.device = "eth0"

        s, r = self.get_network_traffic()

        self.sent_prev = s
        self.received_prev = r

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

    disk_usage = psutil.disk_usage('/')

    memory_load = psutil.virtual_memory().percent  # Memory usage in percentage
    cpu_load = psutil.cpu_percent(interval=1)  # CPU usage in percentage

    net_in, net_out = networkSampler.sample()

    return {
        "memory_load": memory_load,
        "cpu_load": cpu_load,
        "net_in": net_in,
        "net_out": net_out,
        "disk_free_gb": disk_usage.free / (1024**3),
        "disk_usage_percent": disk_usage.percent
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


print("Collecting basic system load per our")
collect_and_store_metrics()
