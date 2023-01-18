#!/bin/bash
OLD=$(dmesg | grep swear | tail -n1)
while true; do
	NEW=$(dmesg | grep swear | tail -n1)
	if [[ "$NEW" == *revoked* ]]; then
		OLD=$NEW
	fi
	if [[ "$NEW" != "$OLD" ]]; then # new swear word typed
		./invert;
		sleep 0.1; 
		./invert;
		sleep 0.1; 
	fi
done
