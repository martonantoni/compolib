/// ----------------------------------------------------------------------------------------------------------- 3DGeometry.cpp

template<class T>
struct t3DPoint
{
	T x, y, z;
	t3DPoint(): x(0), y(0), z(0) {}
	t3DPoint(T x, T y, T z): x(x), y(y), z(z) {}
	template<class U> t3DPoint(const t3DPoint<U> &Other): x(Other.x), y(Other.y), z(Other.z) {}
	void Read();
	t3DPoint &operator-=(const t3DPoint &Other) { x-=Other.x; y-=Other.y; z-=Other.z; return *this; }
	t3DPoint &operator+=(const t3DPoint &Other) { x+=Other.x; y+=Other.y; z+=Other.z; return *this; }
	t3DPoint operator+(const t3DPoint &Offset) const { return t3DPoint(x+Offset.x, y+Offset.y, z+Offset.z); }
	t3DPoint operator-(const t3DPoint &Offset) const { return t3DPoint(x-Offset.x, y-Offset.y, z-Offset.z); }
	t3DPoint operator-() const { return t3DPoint(-x, -y, -z); }
	t3DPoint operator/(const t3DPoint &Other) const { return t3DPoint(x/Other.x, y/Other.y, z/Other.z); }
	template<class M> t3DPoint operator*(M m) const { return t3DPoint(x*m, y*m, z*m); }
	template<class D> t3DPoint operator/(D d) const { return t3DPoint(x/d, y/d, z/d); }
	template<class U> t3DPoint &operator=(const t3DPoint<U> &Other) { x=Other.x;  y=Other.y; z=Other.z; }
	bool operator==(const t3DPoint &Other) const { return x==Other.x&&y==Other.y&&z==Other.z; }
	bool operator!=(const t3DPoint &Other) const { return x!=Other.x||y!=Other.y||z!=Other.z; }
	double DistanceFrom(t3DPoint Other) const
	{
		auto v=Other-(*this);
		return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
	}
};
typedef t3DPoint<int> c3DIntPoint;
typedef t3DPoint<double> c3DPoint;

template<> inline void t3DPoint<int>::Read() { scanf("%d %d %d", &x, &y, &z); }
template<> inline void t3DPoint<double>::Read() {	scanf("%lf %lf %lf", &x, &y, &z); }

template<class T> t3DPoint<T> CrossProduct(const t3DPoint<T> &a, const t3DPoint<T> &b)
{
	return { a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x };
}

template<class T> t3DPoint<T> Normal(const t3DPoint<T> &a, const t3DPoint<T> &b)
{
	auto c=CrossProduct(a, b);
	return c/c.DistanceFrom({ 0.0,0.0,0.0 });
}

template<class T> T DotProduct(const t3DPoint<T> &a, const t3DPoint<T> &b)
{
	return a.x*b.x+a.y*b.y+a.z*b.z;
}

/// ----------------------------------------------------------------------------------------------------------- end of 3DGeometry.cpp
