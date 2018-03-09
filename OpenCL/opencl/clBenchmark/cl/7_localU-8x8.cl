__kernel void convolute(
	const __global float4 * input, 
	__global float4 * output,
	__constant float4 * filter,
	__local float4 * cached
)
{
	const int rowOffset = get_global_id(1) * IMAGE_W;
	const int my = get_global_id(0) + rowOffset;
	
	const int localRowLen = HALF_FILTER_SIZE * 2 + get_local_size(0);
	const int localRowOffset = ( get_local_id(1) + HALF_FILTER_SIZE ) * localRowLen;
	const int myLocal = localRowOffset + get_local_id(0) + HALF_FILTER_SIZE;		
		
	// copy my pixel
	cached[ myLocal ] = input[ my ];

	
	if (
		get_global_id(0) < HALF_FILTER_SIZE || 
		get_global_id(0) > IMAGE_W - HALF_FILTER_SIZE - 1 || 
		get_global_id(1) < HALF_FILTER_SIZE ||
		get_global_id(1) > IMAGE_H - HALF_FILTER_SIZE - 1
	)
	{
		// no computation for me, sync and exit
		barrier(CLK_LOCAL_MEM_FENCE);
		return;
	}
	else 
	{
		// copy additional elements
		int localColOffset = -1;
		int globalColOffset = -1;
		
		if ( get_local_id(0) < HALF_FILTER_SIZE )
		{
			localColOffset = get_local_id(0);
			globalColOffset = -HALF_FILTER_SIZE;
			
			cached[ localRowOffset + get_local_id(0) ] = input[ my - HALF_FILTER_SIZE ];
		}
		else if ( get_local_id(0) >= get_local_size(0) -HALF_FILTER_SIZE )
		{
			localColOffset = get_local_id(0) + HALF_FILTER_SIZE * 2;
			globalColOffset = HALF_FILTER_SIZE;
			
			cached[ localRowOffset + get_local_id(0) + HALF_FILTER_SIZE * 2 ] = input[ my + HALF_FILTER_SIZE ];
		}
		
		
		if ( get_local_id(1) < HALF_FILTER_SIZE )
		{
			cached[ get_local_id(1) * localRowLen + get_local_id(0) + HALF_FILTER_SIZE ] = input[ my - HALF_FILTER_SIZE * IMAGE_W ];
			if (localColOffset > 0)
			{
				cached[ get_local_id(1) * localRowLen + localColOffset ] = input[ my - HALF_FILTER_SIZE * IMAGE_W + globalColOffset ];
			}
		}
		else if ( get_local_id(1) >= get_local_size(1) -HALF_FILTER_SIZE )
		{
			int offset = ( get_local_id(1) + 2 * HALF_FILTER_SIZE ) * localRowLen;
			cached[ offset + get_local_id(0) + HALF_FILTER_SIZE ] = input[ my + HALF_FILTER_SIZE * IMAGE_W ];
			if (localColOffset > 0)
			{
				cached[ offset + localColOffset ] = input[ my + HALF_FILTER_SIZE * IMAGE_W + globalColOffset ];
			}
		}
		
		// sync
		barrier(CLK_LOCAL_MEM_FENCE);

		
		// perform convolution
		float4 sum = (float4) 0.0;
		
@@UNROLLED LOCAL LOOP@@
		
		output[my] = sum;
	}
}

