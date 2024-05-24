#!/bin/bash

TEST_PROGRAM=$1
TEST_PARAMS=$2
#ITERATIONS=$3
#DATA_SUBDIRECTORY=$4

PATH_TEST_PROGRAM="test_programs/$TEST_PROGRAM/bin/${TEST_PROGRAM}_${TEST_PARAMS}"
#PATH_DATA="data/$DATA_SUBDIRECTORY/${TEST_PROGRAM}_${TEST_PARAMS}"

#mkdir "data/$DATA_SUBDIRECTORY" 2> /dev/null


./$PATH_TEST_PROGRAM > /dev/null &
PID_TEST_PROGRAM=$!

echo $PID_TEST_PROGRAM

# could be used to move cursor to specified position
#xdotool mousemove $x $y

sigint() {
    kill -INT $PID_TEST_PROGRAM
}

trap sigint INT

# (c) Raphael "WinID" Wagner, 2024
WINID=$(xwininfo -root -all -int | grep pygame_default | awk '{$1=$1;print}' | awk '{split($1,a," "); print a[1]}')
#WINID=$(xdotool search --pid $PID_TEST_PROGRAM)
echo $WINID

sleep "2s"

kill -9 $PID_TEST_PROGRAM
