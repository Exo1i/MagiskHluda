#!/system/bin/sh

DEBUG=false
MODDIR=${0%/*}
MODULE_PROP="/data/adb/modules/magisk-hluda/module.prop"
MODULE_CFG="/data/adb/modules/magisk-hluda/module.cfg"

# Wait for boot completion
while [ "$(getprop sys.boot_completed)" != 1 ]; do
   sleep 1
done

# Additional stabilization delay
sleep 5

# Sync status from module.cfg to module.prop
if [ ! -f "$MODULE_CFG" ]; then
   sed -i 's/^description=.*/description=[Error: Config Missing ⚠️]/' "$MODULE_PROP"
   exit 1
fi

# Source config and update initial status
. "$MODULE_CFG"
if [ "$status" = "1" ]; then
   sed -i 's/^description=.*/description=[Running✅]/' "$MODULE_PROP"
else
   sed -i 's/^description=.*/description=[Stopped❌]/' "$MODULE_PROP"
fi

# Read configuration from module.cfg
if [ -f "$MODULE_CFG" ]; then
   # Source the config file to get variables
   . "$MODULE_CFG"

   # Check if service should start (status=1)
   if [ "$status" = "1" ]; then
       # Start service if not running
       if ! pgrep -x "florida" > /dev/null; then
           # Build command with port and parameters
           CMD="florida -D -l 0.0.0.0:$port"

           # Add additional parameters if specified
           if [ -n "$parameters" ]; then
               CMD="$CMD $parameters"
           fi

           # Execute the command and check result
           if ! $CMD; then
               sed -i 's/^description=.*/description=[Start Failed ⚠️]/' "$MODULE_PROP"
           fi
       fi
   else
       # Update module.prop to show stopped status
       sed -i 's/^description=.*/description=[Stopped❌]/' "$MODULE_PROP"
   fi
else
   # Log error if config file is missing
   echo "Error: module.cfg not found";
fi