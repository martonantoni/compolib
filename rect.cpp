struct cRect
{
	ll x, y, ex, ey;
};

bool IsBetween(ll x, ll cx, ll cxe)
{
	return x>=cx&&x<=cxe;
}

pair<ll, ll> Intersection(ll x1, ll x1e, ll x2, ll x2e)
{
	if(IsBetween(x1, x2, x2e))
	{
		if(IsBetween(x1e, x2, x2e))
			return{ x1, x1e };
		return{ x1, x2e };
	}
	if(IsBetween(x1e, x2, x2e))
		return{ x2, x1e };
	if(IsBetween(x2, x1, x1e))
		return{ x2, x2e };
	return{ -1, -1 };
}

cRect IntersectingRect(const cRect& a, const cRect& b)
{
	cRect result;
	tie(result.x, result.ex) = Intersection(a.x, a.ex, b.x, b.ex);
	tie(result.y, result.ey) = Intersection(a.y, a.ey, b.y, b.ey);
	return result;
}
