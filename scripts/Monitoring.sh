#!/bin/bash

# Create the directory if it does not exist
mkdir -p ../rundir/

# Change to the directory
cd ../rundir/ || { echo "Failed to change directory to ../rundir/"; exit 1; }

# Define the log file with the current timestamp
LOGFILE="log"

sudo rm "$LOGFILE"

# Execute the script and redirect both stdout and stderr to the log file
sudo ../graphana/runBackEnds.sh >> "$LOGFILE" 2>&1

# Check if the script execution was successful
if [ $? -ne 0 ]; then
    echo "One of the scripts crashed. Check $LOGFILE for details."
else
    echo "Script executed successfully. Log file: $LOGFILE"
fi