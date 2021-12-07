ll sum_of_ar_seq(ll first, ll step, ll count)   // count: how many items in total
{
    return (first + first + (count - 1) * step) * count / 2;
}