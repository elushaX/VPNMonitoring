import subprocess

def run_bash_script(script_path):
    try:
        # Execute the bash script and capture its output
        result = subprocess.run(['bash', script_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        
        # Check for errors
        if result.returncode != 0:
            print(f"Error executing script: {result.stderr}")
            return []
        
        # Split the output into a list of strings (lines)
        output_lines = result.stdout.splitlines()
        return output_lines
    except Exception as e:
        print(f"An error occurred: {e}")
        return []

# Example usage:
script_path = './shadowsocks/listClients.sh'
output_lines = run_bash_script(script_path)

# Output lines are now in a list of strings
for line in output_lines:
    print(line)
