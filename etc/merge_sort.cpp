
void mergeSort(int* a, int n, int* buffer)
{
    if (n == 1)
        return;
    int half = n / 2;
    mergeSort(a, half, buffer);
    mergeSort(a + half, n - half, buffer);
    for (int i = 0, j = half, s = 0; s < n; ++s)
    {
        auto take_i = [&]()
        {
            buffer[s] = a[i];
            ++i;
        };
        auto take_j = [&]()
        {
            buffer[s] = a[j];
            ++j;
        };
        if (i < half)
        {
            if (j < n)
            {
                if (a[i] < a[j])
                {
                    take_i();
                }
                else
                {
                    take_j();
                }
            }
            else
            {
                take_i();
            }
        }
        else
        {
            take_j();
        }
    }
    copy(buffer, buffer + n, a);
}

void Solve()
{
    RI(n);
    RIV(a, n);
    vector<int> buffer(n);
    mergeSort(a.data(), n, buffer.data());
    for (auto x : a)
        printf("%d ", x);

}
