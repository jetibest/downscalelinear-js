#!/bin/bash

TEST_FILE="$1"
if [ -z "$TEST_FILE" ]
then
	TEST_FILE="./otzi.jpg"
fi

OUTPUT_DIR="./build"

if ! [ -d "$OUTPUT_DIR" ]
then
	mkdir -p "$OUTPUT_DIR" || exit 1
fi

OUTPUT_FILE="$OUTPUT_DIR/downscale-linear-node.jpg"

ffmpeg -i "$TEST_FILE" -f rawvideo -pix_fmt gray8 -s 1280x720 pipe:1 2>/dev/null |
	node index.js |
	ffmpeg -f rawvideo -pixel_format gray8 -video_size 640x360 -i pipe:0 -y "$OUTPUT_FILE" 2>/dev/null &&
	echo "File written to: $TEST_FILE"
