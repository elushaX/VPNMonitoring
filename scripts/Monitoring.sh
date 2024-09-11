#!/bin/bash

mkdir ../rundir/
cd ../rundir/

LOGFILE="log_$(date +'%Y-%m-%d_%H-%M-%S').log"

./../graphana/runBackEnds.sh


if [ $? -ne 0 ]; then
    echo "One of the scripts crashed. Check $LOGFILE for details."
fi