#!/bin/bash -x
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

serviceFiles=( $(grep -l Interface /usr/share/dbus-1/services/*.service) )
services=( $(awk -F= '/Name/ { print $2 }' ${serviceFiles[@]} ) )
interfaces=( $(awk -F= '/Interface/ { print $2 }' ${serviceFiles[@]} ) )

echo "services =" ${services[@]}
echo "interfaces =" ${interfaces[@]}

# test service lookup - primary use case
for service in ${services[@]}; do
    qdbus com.nokia.MServiceFw / com.nokia.MServiceFwIf.serviceName $service
done

# test interface lookup
for interface in ${interfaces[@]}; do
    qdbus com.nokia.MServiceFw / com.nokia.MServiceFwIf.interfaceName $interface
done

# add/remove services
for serviceFile in ${serviceFiles[@]}; do
    mv $serviceFile /tmp/tmpService
    sleep 1
    mv /tmp/tmpService $serviceFile
    sleep 1
done
