
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

    max_idle_interval = 60
    idle_interval = 0
    step_interval = 1

    while True:
        current_active_users = len(get_client_ips())

        if current_active_users != last_active_users or idle_interval > max_idle_interval:
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
            idle_interval = 0
        else:
            idle_interval += step_interval

        time.sleep(step_interval)


print("Collecting active vpn users")
collect_and_store_metrics()
