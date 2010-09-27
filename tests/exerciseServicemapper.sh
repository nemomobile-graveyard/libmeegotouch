#! /bin/bash -x

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
