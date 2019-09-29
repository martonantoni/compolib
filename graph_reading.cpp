/////
// for input format: 
// The first line contains two integers, N and E, followed by E lines containing three integers each: 
//    the two stations that are connected to each other and the fare between them (C)

struct cEdge
{
	int to;
	int w;
	cEdge(int to, int w): to(to), w(w) {}
};

struct cNode
{
	vector<cEdge> edges;
};

vector<cNode> g;

void Solve()
{
	RII(n, e);
	g.resize(n);
	for(int i=0; i<e; ++i)
	{
		RIII(f, t, w);
		--f; --t;
		g[f].edges.emplace_back(t, w);
		g[t].edges.emplace_back(f, w);
	}
}