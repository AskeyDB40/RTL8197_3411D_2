#!/bin/sh
#
# script file to start reinit daemon
#
#

ubusd&
sleep 1 
reinitSer&
sleep 1

