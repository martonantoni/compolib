class cTimer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime = std::chrono::high_resolution_clock::now();
	int mChronoCallSkips, mCalls;
public:
	cTimer(int ChronoCallSkips): mChronoCallSkips(ChronoCallSkips), mCalls(mChronoCallSkips) {}
	bool runMore()
	{
		if(--mCalls>0)
			return true;
		mCalls = mChronoCallSkips;
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mStartTime).count()<1800;
	}
};

#define WHILE_TIME(skips) for(cTimer stop_timer(skips); stop_timer.runMore();)

