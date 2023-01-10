#!/bin/bash
OLD=$(dmesg | grep swear | tail -n1)
while true; do
	NEW=$(dmesg | grep swear | tail -n1)
	echo new is $NEW
	if [[ "$NEW" != "$OLD" ]]; then
		./invert;
		sleep 0.1; 
		./invert;
		sleep 0.1; 
	fi
done
