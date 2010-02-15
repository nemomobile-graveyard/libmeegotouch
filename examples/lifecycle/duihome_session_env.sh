#!/bin/sh

# This script sets up the D-Bus environment variables to the same
# value as used by duihomescreen
#
# source ./duihome_session_env.sh

eval $(cat /proc/$(pgrep duihome)/environ | sed 's/\x00/\nexport /g'  | grep DBUS_SESSION_BUS)
