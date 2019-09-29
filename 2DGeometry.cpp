


	auto rotate=[](double x, double y, double alfa)
	{
		double rx=cos(alfa)*x-sin(alfa)*y;
		double ry=sin(alfa)*x+cos(alfa)*y;
		return pair<double, double>(rx, ry);
	};

	
/// ----------------------------------------------------------------------------------------------------------- 2DGeometry.cpp


///                                             P o i n t   &   V e c t o r

template<class T> struct tVec2;
template<class T> struct tPoint
{
	T x=0, y=0;
	tPoint() {}
	tPoint(T x, T y): x(x), y(y) {}
	template<class U> tPoint(const tPoint<U> &Other): x(Other.x), y(Other.y) {}
	void Read();
	tPoint &operator-=(const tVec2<T> &v) { x-=v.x; y-=v.y; return *this; }
	tPoint &operator+=(const tVec2<T> &v) { x+=v.x; y+=v.y; return *this; }
	tPoint operator+(const tVec2<T> &v) const { return tPoint(x+v.x, y+v.y); }
	tPoint operator-(const tVec2<T> &v) const { return tPoint(x-v.x, y-v.y); }
	template<class U> tPoint &operator=(const tPoint<U> &Other) { x=Other.x;  y=Other.y; }
	bool operator==(const tPoint &Other) const { return x==Other.x&&y==Other.y; }
	bool operator!=(const tPoint &Other) const { return x!=Other.x||y!=Other.y; }
	double DistanceFrom(tPoint Other) const
	{
		auto xdist=x-Other.x, ydist=y-Other.y;
		return sqrt(xdist*xdist+ydist*ydist);
	}
	operator pair<T, T>() const { return { x,y }; }
};
using cIntPoint=tPoint<int>;
using cPoint=tPoint<double>;
template<> inline void tPoint<int>::Read() { scanf("%d %d", &x, &y); }
template<> inline void tPoint<double>::Read() { scanf("%lf %lf", &x, &y); }

template<class T> struct tVec2
{
	T x=0, y=0;
	tVec2() {}
	tVec2(T x, T y): x(x), y(y) {}
	tVec2(const tPoint<T> &From, const tPoint<T> &To): x(To.x-From.x), y(To.y-From.y) {}

	tVec2 &operator-=(const tVec2 &v) { x-=v.x; y-=v.y; return *this; }
	tVec2 &operator+=(const tVec2<T> &v) { x+=v.x; y+=v.y; return *this; }
	tVec2 operator+(const tVec2<T> &v) const { return tVec2(x+v.x, y+v.y); }
	tVec2 operator-(const tVec2<T> &v) const { return tVec2(x-v.x, y-v.y); }
	tVec2 operator-() const { return tVec2(-x, -y); }
	template<class M> tVec2 operator*(M m) const { return tVec2(x*m, y*m); }
	template<class D> tVec2 operator/(D d) const { return tVec2(x/d, y/d); }
	template<class M> tVec2 &operator*=(M m) { return *this=*this*m; }
	template<class D> tVec2 &operator/=(D d) { return *this=*this/d; }
	operator pair<T, T>() const { return { x,y }; }
	void RotateLeft90() { swap(x, y); x=-x; }
	void RotateRight90() { RotateLeft90(); *this=-*this; }

	double GetLength() const { return sqrt((double)(x*x+y*y)); }
	tVec2<T> GetUnitVector() const;
};
using cIntVec2=tVec2<int>;
using cVec2=tVec2<double>;

template<class T> tVec2<T> tVec2<T>::GetUnitVector() const
{
	auto l=GetLength();
	return l==0.0?tVec2<T>{}:*this/l;
}

template<class T> T DotProduct(const tVec2<T> &a, const tVec2<T> &b)
{
	return a.x*b.x+a.y*b.y;
}

template<class T> inline cPoint Lerp(T a, T b, double t)
{
	return { a.x+(b.x-a.x)*t, a.y+(b.y-a.y)*t };
}

// Three points are a counter-clockwise turn if ccw > 0, clockwise if
// ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
// gives twice the signed  area of the triangle formed by p1, p2 and p3.
double ccw(cPoint p1, cPoint p2, cPoint p3)
{
	return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
}

template<class T>
tPoint<T> MirrorPointOnPoint(tPoint<T> center, tPoint<T> point_to_transform)
{
	return center+tVec2<T>(point_to_transform, center);
}

///                                             S e g m e n t

struct cSegment
{
	cPoint p1, p2;
	cSegment(const cPoint &p1, const cPoint &p2): p1(p1), p2(p2) {}
	pair<double, double> GetLeftAndRight() const; // X increases RIGHT  >
	pair<double, double> GetTopAndBottom() const; // Y increases UP     ^
};

pair<double, double> cSegment::GetLeftAndRight() const
{
	if(p1.x<p2.x)
		return { p1.x, p2.x };
	return { p2.x, p1.x };
}

pair<double, double> cSegment::GetTopAndBottom() const
{
	if(p1.y>p2.y)
		return { p1.y, p2.y };
	return { p2.y, p1.y };
}

///                                             C i r c l e

struct cCircle
{
	cPoint center;
	double r;
	cCircle(const cPoint &center, double r): center(center), r(r) {}
};

///                                             I n t e r s e c t i o n s

// int number_of_intersections;
// array<cPoint, 2> intersections;
// tie(number_of_intersections, intersections)=GetIntersection(circle, segment);
tuple<int, array<cPoint, 2>> GetIntersection(const cSegment &a, const cSegment &b)
{
	// original: http://www.cplusplus.com/forum/beginner/49408/

	cVec2 vec_a(a.p1, a.p2), vec_b(b.p1, b.p2);
	cVec2 vec_a_l90=vec_a, vec_b_l90=vec_b;
	vec_a_l90.RotateLeft90();
	vec_b_l90.RotateLeft90();

	double f = DotProduct(vec_a_l90, vec_b);
	if(!f)      // lines are parallel
	{
		if(a.p1==b.p1||a.p1==b.p2)
			return tuple<int, array<cPoint, 2>>{ 1, { a.p1, {} } };
		if(a.p2==b.p1||a.p2==b.p2)
			return tuple<int, array<cPoint, 2>>{ 1, { a.p2, {} } };
		// infinite or zero
		return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}

	cVec2 vec_c(a.p2, b.p2);
	double aa = DotProduct(vec_a_l90, vec_c);
	double bb = DotProduct(vec_b_l90, vec_c);

	if(f < 0)
	{
		if(aa>0||bb>0||aa<f||bb<f)
			return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}
	else
	{
		if(aa<0||bb<0||aa>f||bb>f)
			return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}
	double intersection_ratio=1.0-aa/f;
	return tuple<int, array<cPoint, 2>>{ 1, { b.p1+vec_b*intersection_ratio, {} } };
}

// int number_of_intersections;
// array<cPoint, 2> intersections;
// tie(number_of_intersections, intersections)=GetIntersection(circle, segment);
tuple<int, array<cPoint, 2>> GetIntersection(const cCircle &circle, const cSegment &segment)
{
	//original: http://csharphelper.com/blog/2014/09/determine-where-a-line-intersects-a-circle-in-c/
	double dx, dy, A, B, C, det;

	dx = segment.p2.x-segment.p1.x;
	dy = segment.p2.y-segment.p1.y;

	A = dx * dx + dy * dy;
	B = 2 * (dx * (segment.p1.x - circle.center.x) + dy * (segment.p1.y - circle.center.y));
	C = (segment.p1.x - circle.center.x) * (segment.p1.x - circle.center.x) +
		(segment.p1.y - circle.center.y) * (segment.p1.y - circle.center.y) -
		circle.r * circle.r;

	det = B * B - 4 * A * C;
	if((A <= 0.0000001) || (det < 0))
	{
		// No real solutions.
		return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}
	else if(det == 0)
	{
		// One solution.
		double t = -B / (2 * A);
		if(t>=0.0&&t<=1.0)
			return tuple<int, array<cPoint, 2>>{ 1, { segment.p1+cVec2(dx,dy)*t, {} } };
		else
			return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}
	else
	{
		// Two solutions.
		tuple<int, array<cPoint, 2>> result(0, { cPoint {}, cPoint {} });
		det=sqrt(det);
		double t = ((-B + det) / (2 * A));
		if(t>=0.0&&t<=1.0)
		{
			get<0>(result)=1;
			get<1>(result)[0]=segment.p1+cVec2(dx, dy)*t;
		}
		t = ((-B - det) / (2 * A));
		if(t>=0.0&&t<=1.0)
		{
			get<1>(result)[get<0>(result)]=segment.p1+cVec2(dx, dy)*t;
			++get<0>(result);
		}
		return result;
	}
}

// int number_of_intersections;
// array<cPoint, 2> intersections;
// tie(number_of_intersections, intersections)=GetIntersection(circle, circle);
tuple<int, array<cPoint, 2>> GetIntersection(const cCircle &circle_a, const cCircle &circle_b)
{
	//original: http://stackoverflow.com/questions/3349125/circle-circle-intersection-points
	if(circle_a.center==circle_b.center)
	{
		ASSERT(circle_a.r!=circle_b.r);
		return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}
	cVec2 center_vector(circle_a.center, circle_b.center);
	double center_distance=center_vector.GetLength();
	if(center_distance>circle_a.r+circle_b.r)
	{
		// too far away
		return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}
	if(center_distance<abs(circle_a.r-circle_b.r))
	{
		// one completely inside the other
		return tuple<int, array<cPoint, 2>>{ 0, { cPoint {}, cPoint {} } };
	}

	double a = (circle_a.r*circle_a.r - circle_b.r*circle_b.r + center_distance*center_distance)/(2*center_distance);
	double h = sqrt(circle_a.r*circle_a.r - a*a);

	cPoint P2=circle_a.center+center_vector*(a/center_distance);
	cVec2 center_vector90=center_vector;
	center_vector90.RotateLeft90();
	center_vector90*=h/center_distance;

	if(h==0.0)
	{
		return tuple<int, array<cPoint, 2>>{ 1, { P2, {} } };
	}
	return tuple<int, array<cPoint, 2>>{ 2, { P2+center_vector90, P2-center_vector90 } };

}

vector<size_t> CreateConvexHull(const vector<cPoint> &points)
{
// using Graham Scan
// https://www.youtube.com/watch?v=VP9ylElm1yY

// find point with smallest Y, in case of tie, the one on the left most. this will be our base point.
	auto base_i=min_element(ALL(points), [](auto &a, auto &b) { return a.y<b.y||(a.y==b.y&&a.x<b.x); });
	auto base_point_index=base_i-points.begin();
// order the remaining points based on the angle of X axis & the line between them and the base point
	vector<double> cos_for_point; // calculating the cosine of that angle is enough for ordering (we're in the [0, PI[ range)
	cos_for_point.reserve(points.size());
	for(auto &point: points)
	{
		if(&point-points.data()!=base_point_index)
			cos_for_point.emplace_back(DotProduct(cVec2(*base_i, point).GetUnitVector(), { 1, 0 }));  // a.b=|a|*|b|*cos(angle)
		else
			cos_for_point.emplace_back(42.0); // base point will be at the front
	}
	auto indexes_to_try=create_sorted_indexes(cos_for_point, std::greater<>()); // if angle_a>angle_b => cos(angle_a)<cos(angle_b)   [ 0<=angle<PI ]

	vector<size_t> hull_indexes;
	hull_indexes.reserve(points.size());
// build the hull:
	for(auto i: indexes_to_try)
	{
		while(hull_indexes.size()>1&&ccw(points[hull_indexes.back()], points[hull_indexes[hull_indexes.size()-2]], points[i])>=0)
			hull_indexes.pop_back();
		hull_indexes.emplace_back(i);
	}
	return hull_indexes;
}






// sort by angle around origo:
struct cPointData
{
	cPoint original;
	double cos_alfa;
};
void Solve()
{
	RI(n);
	vector<cPointData> points(n);
	for(auto &p: points)
	{
		p.original.Read();
		p.cos_alfa=cVec2({ 0,0 }, p.original).GetUnitVector().x;
	}
	sort(ALL(points), [](const cPointData &a, const cPointData &b)
	{
		if(a.original.y>=0)
		{
			if(b.original.y>=0)
				return a.cos_alfa-b.cos_alfa>Epsilon||(abs(a.cos_alfa-b.cos_alfa)<=Epsilon&&a.original.DistanceFrom(cPoint(0.0,0.0))<b.original.DistanceFrom(cPoint(0.0, 0.0)));
			return true;
		}
		else
		{
			if(b.original.y>=0)
				return false;
			return a.cos_alfa-b.cos_alfa<-Epsilon||(abs(a.cos_alfa-b.cos_alfa)<=Epsilon&&a.original.DistanceFrom(cPoint(0.0, 0.0))<b.original.DistanceFrom(cPoint(0.0, 0.0)));
		}
	});
	for(auto &p: points)
	{
		printf("%.0f %.0f\n", p.original.x, p.original.y);
	}
}






/// ----------------------------------------------------------------------------------------------------------- end of 2DGeometry.cpp
