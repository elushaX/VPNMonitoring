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

output_lines = run_bash_script(script_path)

for line in output_lines:
    print(line)
