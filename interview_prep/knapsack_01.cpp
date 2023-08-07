/*

https://www.hackerearth.com/problem/algorithm/01-knapsack-problem-5a88b815/

Problem
^^^^^^^

You are given n  objects, a knapsack of capacity  c, array v, and array w. The ith object has value v[i] and weight w[i].
Determine the maximum total value that you can get by selecting objects in such a manner that their 
sum of weights is not greater than the capacity c.

Print a single integer denoting the maximum value that you can get by selecting the objects

Input
^^^^^

First line: Two integers n and c  denoting the number of objects and capacity of the knapsack ( 1 <= n<= 10^3 and  1 <= C <= 2*10^6 ) .
Second line:  n  integers  (0 <= Vi <= 50)
Third line:  n integers  ( 10 <=Wi <= 2*10^6)

*/


void Solve()
{
    RII(n, c);
    RIV(values, n);
    RIV(weights, n);
    vector<int> max_values(c+1, 0);
    FOR(i, n)
    {
        int w = weights[i];
        int v = values[i];
        for (int target_cap = c; target_cap >= 0; --target_cap)
        {
            if (target_cap >= w)
                max_values[target_cap] = max(max_values[target_cap], max_values[target_cap - w] + v);
        }
    }
    printf("%d\n", max_values[c]);
}
