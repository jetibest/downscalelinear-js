// downscale with linear interpolation (width and height are reduced by 2, so total number of pixels is /4)
module.exports = {
    downscaleLinear: function(fd, nd, fw, fh)
    {
        var nw = Math.floor(fw/2);
        var nh = Math.floor(fh/2);
        
        for(var y=0;y<nh;++y)
        {
            for(var x=0;x<nw;++x)
            {
                var fi = 2 * (y * fw + x);
                
                nd[y * nw + x] = (fd[fi] + fd[fi + 1] + fd[fi + fw] + fd[fi + fw + 1]) / 4;
            }
        }
    }
};
