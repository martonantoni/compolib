unsigned int snoob(unsigned int x) // https://www.geeksforgeeks.org/next-higher-number-with-same-number-of-set-bits/
{
	unsigned int rightOne;
	unsigned int nextHigherOneBit;
	unsigned int rightOnesPattern;
	unsigned int next = 0;

	if(x)
	{

		// right most set bit 
		rightOne = x & -(signed)x;

		// reset the pattern and set next higher bit 
		// left part of x will be here 
		nextHigherOneBit = x + rightOne;

		// nextHigherOneBit is now part [D] of the above explanation. 

		// isolate the pattern 
		rightOnesPattern = x ^ nextHigherOneBit;

		// right adjust pattern 
		rightOnesPattern = (rightOnesPattern)/rightOne;

		// correction factor 
		rightOnesPattern >>= 2;

		// rightOnesPattern is now part [A] of the above explanation. 

		// integrate new pattern (Add [D] and [A]) 
		next = nextHigherOneBit | rightOnesPattern;
	}

	return next;
}