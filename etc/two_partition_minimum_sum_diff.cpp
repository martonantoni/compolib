void Solve(const vector<int>& ws)
{
    int total = 0;
    for(auto x: ws)
    {
		total+=x;
    }
    vector<bool> possible(total+1, false);
    possible[0] = true;
    for (auto x : ws)
    {
        for (int i = total-1; i >= 0; --i)
        {
            if (possible[i])
                possible[i + x] = true;
        }
    }
    for (int i = 0; i < total; ++i)
    {
        if (possible[i + total / 2])
        {
            printf("total: %d, possible: %d, diff: %d\n", total, i + total / 2, abs((i + total / 2) - (total - (i+total/2))) );
            break;
        }
    }
}
