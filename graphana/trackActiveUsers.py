
from common import *

from influxdb import InfluxDBClient
import psutil
import time
from datetime import datetime, timedelta

INFLUXDB_DATABASE = 'vpn_metrics'

# Connect to InfluxDB
client = InfluxDBClient(host=INFLUXDB_HOST, port=INFLUXDB_PORT, username=INFLUXDB_USER, password=INFLUXDB_PASSWORD)
client.switch_database(INFLUXDB_DATABASE)


def collect_and_store_metrics():
    last_active_users = 0

    while True:
        current_active_users = len(get_client_ips())

        if current_active_users != last_active_users:
            influx_data = [
                {
                    "measurement": "system_metrics",
                    "tags": {
                        "host": "localhost"
                    },
                    "time": datetime.utcnow().isoformat(),
                    "fields": {
                        "active_users" : current_active_users
                    }
                }
            ]

            client.write_points(influx_data)

            last_active_users = current_active_users

        time.sleep(5)


print("Collecting active vpn users")
collect_and_store_metrics()
