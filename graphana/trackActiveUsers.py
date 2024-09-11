
from common import *

from influxdb import InfluxDBClient
import psutil
import time
from datetime import datetime, timedelta

INFLUXDB_DATABASE = 'vpn_metrics'

# Connect to InfluxDB
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT, username=INFLUXDB_USER, password=INFLUXDB_PASSWORD)
client.switch_database(INFLUXDB_DATABASE)


def sample():
    current_ips = get_client_ips()
    return {
        "active_users": len(current_ips),
    }


def collect_and_store_metrics():
    while True:
        data = sample()
        current_time = datetime.utcnow().isoformat()

        influx_data = [
            {
                "measurement": "system_metrics",
                "tags": {
                    "host": "localhost"
                },
                "time": current_time,
                "fields": { key: value for key, value in data.items() }
            }
        ]

        client.write_points(influx_data)

        # Sleep for 1 second before collecting data again
        time.sleep(10)


print("Collecting active vpn users")
collect_and_store_metrics()
