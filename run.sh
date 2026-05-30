#!/bin/bash
cd /Users/pranayamurugan/ipc-inspector/build

./producer &
PRODUCER_PID=$!

sleep 0.5

./consumer &
CONSUMER_PID=$!

./inspector &
INSPECTOR_PID=$!

wait $CONSUMER_PID
kill $PRODUCER_PID $INSPECTOR_PID 2>/dev/null