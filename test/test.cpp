#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

// #pragma GCC push_options
// #pragma GCC optimize ("unroll-loops")

#define byte unsigned char
#define BUFFER_LENGTH 1024
#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 720
#define FRAME_LENGTH FRAME_WIDTH * FRAME_HEIGHT

void downscaleLinear(byte* fd, byte* nd, size_t fw, size_t fh)
{
    size_t nw = fw / 2;
    size_t nh = fh / 2;
    
    for(size_t y=0;y<nh;++y)
    {
        for(size_t x=0;x<nw;++x)
        {
            size_t fi = 2 * (y * fw + x);
            size_t fj = y * nw + x;
            
            nd[fj] = (fd[fi] + fd[fi + 1] + fd[fi + fw] + fd[fi + fw + 1]) / 4;
        }
    }
}

// usage: ffmpeg -i /path/to/some/image.jpg -f rawvideo -pix_fmt gray8 -s 1280x720 pipe:1 2>/dev/null | ./downscale-linear >output.jpg
int main()
{
    struct timeval t0;
    struct timeval t1;
    
    // read frame from stdin
    byte frame[FRAME_LENGTH];
    size_t offset = 0;
    int bytes_read = 0;
    while((bytes_read = read(STDIN_FILENO, frame + offset, FRAME_LENGTH)) > 0)
    {
        offset += bytes_read;
    }
    
    gettimeofday(&t0, NULL);
    
    // downscale frame to mipmap
    size_t NEW_FRAME_LENGTH = FRAME_LENGTH / 4;
    byte nframe[NEW_FRAME_LENGTH];
    downscaleLinear(frame, nframe, FRAME_WIDTH, FRAME_HEIGHT);
    
    gettimeofday(&t1, NULL);
    
    // write mipmaps to stdout
    size_t bytes_written = write(STDOUT_FILENO, nframe, NEW_FRAME_LENGTH);
    
    long time_us = ((t1.tv_sec - t0.tv_sec) * 1000000L) + (t1.tv_usec - t0.tv_usec);
    fprintf(stderr, "Time taken: %dus\n", time_us);
}
