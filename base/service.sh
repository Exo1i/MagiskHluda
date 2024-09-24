#!/system/bin/sh

DEBUG=false

MODDIR=${0%/*}

# This script will be executed in late_start service mode

# wait for boot to complete
while [ "$(getprop sys.boot_completed)" != 1 ]; do
    sleep 1
done

# ensure boot has actually completed
sleep 5

# restart on crash, but only if florida-server is not running
while true; do
    if ! pgrep -x "hluda" > /dev/null; then
        hluda -D
    fi
    sleep 1
done