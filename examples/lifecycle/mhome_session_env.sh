#!/bin/sh
#/***************************************************************************
#**
#** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
#** All rights reserved.
#** Contact: Nokia Corporation (directui@nokia.com)
#**
#** This file is part of libmeegotouch.
#**
#** If you have questions regarding the use of this file, please contact
#** Nokia at directui@nokia.com.
#**
#** This library is free software; you can redistribute it and/or
#** modify it under the terms of the GNU Lesser General Public
#** License version 2.1 as published by the Free Software Foundation
#** and appearing in the file LICENSE.LGPL included in the packaging
#** of this file.
#**
#****************************************************************************/
# This script sets up the D-Bus environment variables to the same
# value as used by mhomescreen
#
# source ./mhome_session_env.sh

eval $(cat /proc/$(pgrep mhome)/environ | sed 's/\x00/\nexport /g' | grep DBUS_SESSION_BUS)
