
struct cPrefixSum2D
{
    vector<ll> sums;
    ll w = 1, h = 1;

    void build(int width, int height, auto query)
    {
        w = width;
        h = height;
        sums.resize(w * h);
        for (int r = 0; r < h; ++r)
        {
            for (int c = 0; c < w; ++c)
            {
                ll v = query(r, c);
                sums[r *w + c] = v
                    + (r > 0 ? sums[(r - 1) * w + c] : 0)
                    + (c > 0 ? sums[r * w + (c - 1)] : 0)
                    - (r > 0 && c > 0 ? sums[(r - 1) * w + (c - 1)] : 0);
            }
        }
    }

    ll sum(cPosition topLeft, cPosition bottomRight) const
    {
        if (topLeft.row > bottomRight.row || topLeft.col > bottomRight.col)
            return 0;
        ll a = sums[bottomRight.row * w + bottomRight.col];
        ll b = topLeft.row > 0 ? sums[(topLeft.row - 1) * w + bottomRight.col] : 0;
        ll c = topLeft.col > 0 ? sums[bottomRight.row * w + (topLeft.col - 1)] : 0;
        ll d = (topLeft.row > 0 && topLeft.col > 0) ? sums[(topLeft.row - 1) * w + (topLeft.col - 1)] : 0;
        return a - b - c + d;
    }
};