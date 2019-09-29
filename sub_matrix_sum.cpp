constexpr int max_w=100, max_h=100;   // <--- check these!

ll matrix[max_w*max_h];  // <----- this is the input

ll sm[max_w*max_h];

ll SubMatrixSum(int x, int y, int ex, int ey) // ex & ey inclusive
{
	if(x==0)
		if(y==0)
			return sm[ey*max_w+ex];
		else
			return sm[ey*max_w+ex]-sm[(y-1)*max_w+ex];
	else
		if(y==0)
			return sm[ey*max_w+ex]-sm[ey*max_w+x-1];
		else
			return sm[ey*max_w+ex]-sm[ey*max_w+x-1]-sm[(y-1)*max_w+ex]+sm[(y-1)*max_w+x-1];
}

void BuildSM(int w, int h)
{
	sm[0]=matrix[0];
	for(int x=1; x<w; ++x)
		sm[x]=matrix[x]+sm[x-1];
	for(int y=1; y<h; ++y)
	{
		sm[y*max_w]=sm[(y-1)*max_w]+matrix[y*max_w];
		for(int x=1; x<w; ++x)
		{
			sm[y*max_w+x]=sm[y*max_w+x-1]+matrix[y*max_w+x]+sm[(y-1)*max_w+x]-sm[(y-1)*max_w+x-1];
		}
	}
}

//--------------- single coloumn / row checkers:

	auto CheckUp=[](int r, int c)
	{
		return r>0&&SubMatrixSum(c, 0, c, r-1)>0;
	};
	auto CheckDown=[rows](int r, int c)
	{
		return r<rows-1&&SubMatrixSum(c, r+1, c, rows-1)>0;
	};
	auto CheckLeft=[](int r, int c)
	{
		return c>0&&SubMatrixSum(0, r, c-1, r)>0;
	};
	auto CheckRight=[cols](int r, int c)
	{
		return c<cols-1&&SubMatrixSum(c+1, r, cols-1, r)>0;
	};



// ------------------------------------------------------


void TestSubMatrixSum()
{
	ll a=33;
	for(int y=0; y<10; ++y)
	{
		for(int x=0; x<6; ++x)
		{
			matrix[y*max_w+x]=a;
			a+=5;
			a%=7;
		}
	}
	BuildSM(6, 10);
	SubMatrixSum(1, 0, 2, 0);
	for(int sy=0; sy<10; ++sy)
	{
		for(int sx=0; sx<6; ++sx)
		{
			for(int ey=sy; ey<10; ++ey)
			{
				for(int ex=sx; ex<6; ++ex)
				{
					ll brute_force_result=0;
					for(int y=sy; y<=ey; ++y)
					{
						for(int x=sx; x<=ex; ++x)
							brute_force_result+=matrix[y*max_w+x];
					}
					ASSERT(brute_force_result==SubMatrixSum(sx, sy, ex, ey));
				}
			}
		}
	}
}