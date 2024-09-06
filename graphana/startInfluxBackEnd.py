from influxdb import InfluxDBClient
import psutil
import time
from datetime import datetime

# InfluxDB connection parameters
INFLUXDB_HOST = 'localhost'
INFLUXDB_PORT = 8086
INFLUXDB_USER = 'root'
INFLUXDB_PASSWORD = 'root'
INFLUXDB_DATABASE = 'metrics'

# Connect to InfluxDB
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT, username=INFLUXDB_USER, password=INFLUXDB_PASSWORD)

# Create or switch to the database
client.create_database(INFLUXDB_DATABASE)
client.switch_database(INFLUXDB_DATABASE)

# Function to collect memory and CPU load and store it in InfluxDB
def collect_and_store_metrics():
    while True:
        # Collect memory and CPU load
        memory_load = psutil.virtual_memory().percent
        cpu_load = psutil.cpu_percent(interval=1)  # CPU usage over the last second
        current_time = datetime.utcnow().isoformat()

        # Prepare data points for memory and CPU load
        data = [
            {
                "measurement": "memory_load",
                "tags": {
                    "host": "localhost"
                },
                "time": current_time,
                "fields": {
                    "value": memory_load
                }
            },
            {
                "measurement": "cpu_load",
                "tags": {
                    "host": "localhost"
                },
                "time": current_time,
                "fields": {
                    "value": cpu_load
                }
            }
        ]

        # Write the data points to InfluxDB
        client.write_points(data)

        # Sleep for 1 second before collecting data again
        time.sleep(1)

if __name__ == '__main__':
    print("Collecting and storing memory and CPU load data in InfluxDB...")
    collect_and_store_metrics()

