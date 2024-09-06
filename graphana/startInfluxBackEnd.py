
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

# Function to return memory and CPU load as a dictionary
def sample():
    memory_load = psutil.virtual_memory().percent  # Memory usage in percentage
    cpu_load = psutil.cpu_percent(interval=1)  # CPU usage in percentage
    return {
        "memory_load": memory_load,
        "cpu_load": cpu_load,
        "dummy": "name",
    }

# Function to collect and store memory and CPU load in InfluxDB
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
