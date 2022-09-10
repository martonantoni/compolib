// based on https://bitbucket.org/StableSort/play/src/master/src/com/stablesort/stringmatch/KnuthMorrisPratt.java
// https://www.youtube.com/watch?v=EL4ZbRF587g&ab_channel=StableSort

template<class T>
int KMP(const T* a, int an, const T* b, int bn)
{
    vector<int> lps(bn + 1);
    lps[0] = -1;
    lps[1] = 0;
    int matches = 0;
    for (int i = 1; i < bn;)
    {
        if (b[matches] == b[i])
        {
            ++matches;
            ++i;
            lps[i] = matches;
        }
        else if(matches > 0)
        {
            matches = lps[matches];
        }
        else
        {
            ++i;
            lps[i] = 0;
        }
    }
//---------------------
    for (int ai = 0, bi = 0; ai < an;)
    {
        if (a[ai] == b[bi])
        {
            ++ai;
            ++bi;
            if (bi == bn)
            {
                return ai;
                // OR:
                // matches.emplace_back(
            }
        }
        else
        {
            bi = lps[bi];
            if (bi < 0)
            {
                ++ai;
                ++bi;
            }
        }
    }
    
    return -1; // no match
}

int KMP(const char* a, const char* b)
{
    return KMP(a, (int)strlen(a), b, (int)strlen(b));
}