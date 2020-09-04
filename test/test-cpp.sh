#!/bin/bash

TEST_FILE="$1"
if [ -z "$TEST_FILE" ]
then
	TEST_FILE="../otzi.jpg"
fi

OUTPUT_DIR="./build"

if ! [ -d "$OUTPUT_DIR" ]
then
	mkdir -p "$OUTPUT_DIR" || exit 1
fi

INPUT_FILE="./test.cpp"
OUTPUT_FILE="$OUTPUT_DIR/downscale-linear"
TEST_COMMAND="ffmpeg -i '$TEST_FILE' -f rawvideo -pix_fmt gray8 -s 1280x720 pipe:1 2>/dev/null | $OUTPUT_FILE | ffmpeg -f rawvideo -pixel_format gray8 -video_size 640x360 -i pipe:0 -y $OUTPUT_FILE.jpg 2>/dev/null"

echo "Suggested march/mtune parameters:"
gcc -v -E -march=native -mtune=native -O2 - 2>&1 <<<'' | grep -E -o -e '-march=[^ ]+' -e '-mtune=[^ ]+' | grep -v native | sort -u | tr '\n' ' ' && echo
echo

num="0"
testrun()
{
	local opts=("$@")
	local opts_str="$*"
	local file_out="$OUTPUT_FILE-$num"
	echo " --- testrun --- "
	echo "Compiling with: gcc $opts_str"
	gcc "${opts[@]}" "$INPUT_FILE" -o "$file_out" || exit 1
	echo "Binary compressed filesize: $(gzip -c "$file_out" | wc -c)B"
	echo "Run:"
	/bin/bash -c "${TEST_COMMAND//$OUTPUT_FILE/$file_out}"
	echo
	
	num="$((num + 1))"
}

testrun -march=skylake -mtune=skylake -O3
testrun -O3
testrun -march=skylake -mtune=skylake -O2
testrun -O2
testrun
