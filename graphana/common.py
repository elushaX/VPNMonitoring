import subprocess

# InfluxDB connection parameters
INFLUXDB_HOST = 'localhost'
INFLUXDB_PORT = 8086
INFLUXDB_USER = 'root'
INFLUXDB_PASSWORD = 'root'

def get_client_ips():
    script_path = "../graphana/listClients.sh"
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