#!/bin/bash

cd ../rundir/

# Define log file name with current timestamp
LOGFILE="backend_log_$(date +'%Y-%m-%d_%H-%M-%S').log"

# Run both Python scripts in parallel and log outputs to the same log file
{
  echo "Starting influx.py at $(date)"
  python3 ../graphana/influx.py
  echo "Finished influx.py at $(date)"
} >> "$LOGFILE" 2>&1 &

{
  echo "Starting postgres.py at $(date)"
  python3 ../graphana/postgres.py
  echo "Finished postgres.py at $(date)"
} >> "$LOGFILE" 2>&1 &

# Wait for both background jobs to finish
wait

# Optional: Notify if either script crashed
if [ $? -ne 0 ]; then
    echo "One of the scripts crashed. Check $LOGFILE for details."
fi