#!/bin/bash

TEST_PROGRAM=$1
TEST_PARAMS=$2
ITERATIONS=$3
DATA_SUBDIRECTORY=$4

PATH_TEST_PROGRAM="test_programs/$TEST_PROGRAM/bin/${TEST_PROGRAM}_custom ${TEST_PARAMS}"
PATH_DATA="data/$DATA_SUBDIRECTORY/${TEST_PROGRAM}_${TEST_PARAMS}"
INPUT_EVENT="/dev/input/event9"

mkdir "data/$DATA_SUBDIRECTORY" 2> /dev/null
mkdir "data/$DATA_SUBDIRECTORY/img" 2> /dev/null

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

sudo -E ./latency_tester_parport/bin/latency_tester "${INPUT_EVENT}" "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS $WINID > "${PATH_DATA}.csv"
#sudo ./latency_tester_parport/bin/latency_tester "/dev/input/event10" "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS $WINID > "${PATH_DATA}.csv"
#sudo ./latency_tester_parport/bin/latency_tester "/dev/input/event4" "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS 29360135 > "${PATH_DATA}.csv"

#python3 control_yalmd.py "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS 1 "${PATH_DATA}_fw.csv"

#python3 control_yalmd.py "${TEST_PROGRAM}_${TEST_PARAMS}" $ITERATIONS 0 "${PATH_DATA}_nofw.csv"

# move tearing images to data dir
for img in tearing_img_tmp/*.png; do
	BASE_NAME=$(basename ${img})
	#mv "$img" "data/$DATA_SUBDIRECTORY/img/${PATH_DATA}_${BASE_NAME}"
	rm -f "data/$DATA_SUBDIRECTORY/img/${TEST_PROGRAM}_${TEST_PARAMS}_${BASE_NAME}"
	mv "$img" "data/$DATA_SUBDIRECTORY/img/${TEST_PROGRAM}_${TEST_PARAMS}_${BASE_NAME}"
done

kill -9 $PID_TEST_PROGRAM

echo "finished measurement"
