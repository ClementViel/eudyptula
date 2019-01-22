#!/bin/bash

WRITE=1;

while  true; do
	WRITE=$((WRITE+1))
	echo $WRITE > /sys/kernel/debug/eudyptula/foo
done&

while true; do
	cat /sys/kernel/debug/eudyptula/foo;
done&


