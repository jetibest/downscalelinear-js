# downscalelinear-js
To illustrate that it is way faster using a C++ addon for NodeJS for heavy computations.

Download with `git clone https://github.com/jetibest/downscalelinear-js`. Run `./compile-node.sh` and `./test-node.sh` to test downscaling of an image. Depends on `node-gyp` (for compiling the addon) and `ffmpeg` (for i/o-handling).

Alternatively, run same algorithm in C++ without using NodeJS, in `cd test && ./test-cpp.sh`. You can also run the same algorithm in pure javascript, which is very slow, using `./test-node.sh --javascript`.

**Benchmarks**

Tested on: Intel(R) Core(TM) i7-7700HQ CPU @ 2.80GHz, these are some average benchmarks:

```
NodeJS with C++ addon: ~160us
Pure C++: ~180us
Pure javascript: ~4000us
```

On Raspberry Pi 3B+ (ARM Cortex-A53 1.4GHz), the benchmarks are as follows (`-march=armv8-a+crc+simd -mtune=cortex-a53 -O3`):

```
NodeJS with C++ addon: ~2300us
Pure C++: ~2000us
Pure javascript: ~35000us
```

**Conclusion**. Using C++ in NodeJS has the potential to make heavy computations roughly 15-25x faster.
