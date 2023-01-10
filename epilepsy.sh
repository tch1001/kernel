#!/bin/bash
while true; do
	if test -f "/tmp/epilepsy"; then
		./invert;
		sleep 0.1; 
		./invert;
		sleep 0.1; 
	fi
done
