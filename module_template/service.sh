#!/system/bin/sh

DEBUG=false
MODDIR=${0%/*}
MODULE_PROP="/data/adb/magisk_modules/magisk-hluda/module.prop"

# Wait for boot completion
while [ "$(getprop sys.boot_completed)" != 1 ]; do
    sleep 1
done

# Additional stabilization delay
sleep 5

# Start service if not running
if ! pgrep -x "florida" > /dev/null; then
    florida -D
    # Update module.prop on successful start
    sed -i 's/^description=.*/description=[Running✅]/' $MODULE_PROP
fi