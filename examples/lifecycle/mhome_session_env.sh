#!/bin/sh
# This script sets up the D-Bus environment variables to the same
# value as used by mhomescreen
#
# source ./mhome_session_env.sh

eval $(cat /proc/$(pgrep mhome)/environ | sed 's/\x00/\nexport /g' | grep DBUS_SESSION_BUS)
