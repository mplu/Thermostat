#!/bin/sh

rm thermostatV2
gcc -Wall thermostat.c readfile.c -o thermostatV2 -lwiringPi
