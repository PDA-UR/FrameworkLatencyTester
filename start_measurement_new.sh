#!/bin/bash

TEST_PROGRAM=$1
TEST_PARAMS=$2
ITERATIONS=$3
DATA_SUBDIRECTORY=$4

PATH_TEST_PROGRAM="test_programs/$TEST_PROGRAM/bin/${TEST_PROGRAM}_${TEST_PARAMS}"
PATH_DATA="data/$DATA_SUBDIRECTORY/${TEST_PROGRAM}_${TEST_PARAMS}"

mkdir "data/$DATA_SUBDIRECTORY" 2> /dev/null

echo "starting $PATH_TEST_PROGRAM"

./$PATH_TEST_PROGRAM > /dev/null &
PID_TEST_PROGRAM=$!

# could be used to move cursor to specified position
#xdotool mousemove $x $y

sigint() {
    kill -INT $PID_TEST_PROGRAM
}

trap sigint INT

#sleep "3s"
#
#python3 calibrate_yalmd.py

sleep "3s"

xdotool mousemove 500 500
python3 click.py 0 > /dev/null &

sleep "2s"

python3 click.py 3 > /dev/null &
xdotool mousemove 500 500
WINID=$(xwininfo -int | grep "Window id:" | awk '{split($0,a," "); print a[4];}')

# (c) Raphael "WinID" Wagner, 2024
#WINID=$(xwininfo -root -all -int | grep "${TEST_PROGRAM}" | head -n 1 | awk '{$1=$1;print}' | awk '{split($1,a," "); print a[1]}')

echo "$TEST_PROGRAM $TEST_PARAMS $WINID"

sleep "1s"

sudo ./latency_tester_pi/bin/latency_tester "/dev/input/event5" "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS $WINID > "${PATH_DATA}.csv"

#python3 control_yalmd.py "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS 1 "${PATH_DATA}_fw.csv"

#python3 control_yalmd.py "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS 0 "${PATH_DATA}_nofw.csv"

kill -9 $PID_TEST_PROGRAM
