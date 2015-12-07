#!/bin/sh

rm thermostat_chambre
gcc -Wall thermostat_chambre.c readfile.c -o thermostat_chambre -lwiringPi
