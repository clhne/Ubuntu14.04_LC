__kernel void convolute(
	const __global float4 * input, 
	__global float4 * output,
	__constant float4 * filter __attribute__((max_constant_size(4096)))
)
{
	int my = get_global_id(0) + get_global_id(1) * IMAGE_W;

	if (
		get_global_id(0) < HALF_FILTER_SIZE || 
		get_global_id(0) > IMAGE_W - HALF_FILTER_SIZE - 1 || 
		get_global_id(1) < HALF_FILTER_SIZE ||
		get_global_id(1) > IMAGE_H - HALF_FILTER_SIZE - 1
	)
	{
		return;
	}
	
	else
	{
		// perform convolution
		float4 sum = (float4) 0.0;

@@UNROLLED LOOP@@

		output[my] = sum;
	}
}

