int left_of(const vector<int>& v, int i)
{
    int left_idx = i == 0 ? v.size() - 1 : i - 1;
    return v[left_idx];
}

int right_of(const vector<int>& v, int i)
{
    int right_idx = i == v.size() - 1 ? 0 : i + 1;
    return v[right_idx];
}

vector<int> removeMins(vector<int> v)
{
    vector<int> res;
    for(auto [i, x]: vs::enumerate(v))
    {
        if (x <= left_of(v, i) || x <= right_of(v, i))
        {
            //is_min[i] = true;
            res.push_back(x);
        }
    }
    return res;
}

vector<int> removeMaxs(vector<int> v)
{
    vector<int> res;
    for(auto [i, x]: vs::enumerate(v))
    {
        if (x >= left_of(v, i) || x >= right_of(v, i))
        {
            //is_min[i] = true;
            res.push_back(x);
        }
    }
    return res;
}
