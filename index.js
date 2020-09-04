const { PerformanceObserver, performance } = require('perf_hooks');
const downscaleLinear = require('./build/Release/downscalelinear.node'); // C++ addon
// const downscaleLinear = require('./test/downscale-linear.js'); // pure javascript

var frames = 0;
var timeStart = Date.now();

var n = 1280 * 720;
var queueCounter = 0;
var queue = [];
var update = async function()
{
	if(queueCounter < n) return;
	
	var framedata = new Uint8Array(n);
	var frameIndex = 0;
	while(queue.length)
	{
		var q = queue[0];
		
		if(frameIndex + q.length <= n)
		{
			framedata.set(q, frameIndex);
			frameIndex += q.length;
			
			queue.shift();
			
			if(frameIndex === n) break;
		}
		else
		{
			var remainder = n - frameIndex;
			
			framedata.set(q.slice(0, remainder), frameIndex);
			frameIndex += remainder;
			
			queue[0] = q.slice(remainder);
			break;
		}
	}
	
	queueCounter -= frameIndex;
	
	var framedata_scaled = new Uint8Array(n/4);
	
	var t0 = performance.now();
    
	downscaleLinear.downscaleLinear(framedata, framedata_scaled, 1280, 720);
	
    var t1 = performance.now();
	
    console.error('Time taken: ' + Math.round((t1 - t0) * 1000) + 'us');
	
	process.stdout.write(framedata_scaled);
	
	update();
};

process.stdout.on('error', function()
{
    process.exit(1);
});

process.stdin.on('error', function(){});
process.stdin.on('data', function(data)
{
	// expecting gray8 rawvideo 1280x720 data
	queueCounter += data.length;
	queue.push(data);
	update();
});
