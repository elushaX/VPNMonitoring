#!/bin/bash

cd ../rundir/

# Define log file name with current timestamp
LOGFILE="backend_log_$(date +'%Y-%m-%d_%H-%M-%S').log"

# Run the Python script and log output and crashes to the log file
python3 ../graphana/startInfluxBackEnd.py > "$LOGFILE" 2>&1

# Optional: Notify on crash
if [ $? -ne 0 ]; then
    echo "Script crashed. Check $LOGFILE for details."
fi
