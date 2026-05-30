#!/bin/bash
cd /Users/pranayamurugan/ipc-inspector/build

./producer_mtex &
PRODUCER_PID=$!

sleep 0.5

./consumer_mtex &
CONSUMER_PID=$!

./inspector_mtex &
INSPECTOR_PID=$!

wait $CONSUMER_PID
kill $PRODUCER_PID $INSPECTOR_PID 2>/dev/null