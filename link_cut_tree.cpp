struct cOriginalNode
{
	int depth;
	int parent;
	vector<int> to;
};
vector<cOriginalNode> original_tree;

// Link-Cut Tree is from: https://github.com/saadtaame/link-cut-tree/blob/master/link_cut.cpp

struct Node
{
	int sz, label; /* size, label */
	Node *p, *pp, *l, *r; /* parent, path-parent, left, right pointers */
	Node() { p = pp = l = r = 0; }
};

void update(Node *x)
{
	x->sz = 1;
	if(x->l) x->sz += x->l->sz;
	if(x->r) x->sz += x->r->sz;
}

void rotr(Node *x)
{
	Node *y, *z;
	y = x->p, z = y->p;
	if((y->l = x->r)) y->l->p = y;
	x->r = y, y->p = x;
	if((x->p = z))
	{
		if(y == z->l) z->l = x;
		else z->r = x;
	}
	x->pp = y->pp;
	y->pp = 0;
	update(y);
}

void rotl(Node *x)
{
	Node *y, *z;
	y = x->p, z = y->p;
	if((y->r = x->l)) y->r->p = y;
	x->l = y, y->p = x;
	if((x->p = z))
	{
		if(y == z->l) z->l = x;
		else z->r = x;
	}
	x->pp = y->pp;
	y->pp = 0;
	update(y);
}

void splay(Node *x)
{
	Node *y, *z;
	while(x->p)
	{
		y = x->p;
		if(y->p == 0)
		{
			if(x == y->l) rotr(x);
			else rotl(x);
		}
		else
		{
			z = y->p;
			if(y == z->l)
			{
				if(x == y->l) rotr(y), rotr(x);
				else rotl(x), rotr(x);
			}
			else
			{
				if(x == y->r) rotl(y), rotl(x);
				else rotr(x), rotl(x);
			}
		}
	}
	update(x);
}

Node *access(Node *x)
{
	splay(x);
	if(x->r)
	{
		x->r->pp = x;
		x->r->p = 0;
		x->r = 0;
		update(x);
	}

	Node *last = x;
	while(x->pp)
	{
		Node *y = x->pp;
		last = y;
		splay(y);
		if(y->r)
		{
			y->r->pp = y;
			y->r->p = 0;
		}
		y->r = x;
		x->p = y;
		x->pp = 0;
		update(y);
		splay(x);
	}
	return last;
}

Node *root(Node *x)
{
	access(x);
	while(x->l) x = x->l;
	splay(x);
	return x;
}

void cut(Node *x)
{
	access(x);
	x->l->p = 0;
	x->l = 0;
	update(x);
}

void link(Node *x, Node *y)
{
	access(x);
	access(y);
	x->l = y;
	y->p = x;
	update(x);
}

Node *lca(Node *x, Node *y)
{
	access(x);
	return access(y);
}

int depth(Node *x)
{
	access(x);
	return x->sz - 1;
}

unordered_set<ull> roads;

void AddRoad(ull x, ull y)
{
	if(x>y)
		swap(x, y);
	roads.insert((x<<32)|y);
}

bool HasRoad(ull x, ull y)
{
	if(x>y)
		swap(x, y);
	return roads.count((x<<32)|y)==1;
}

class LinkCut
{
	Node *x;

public:
	LinkCut(int n)
	{
		x = new Node[n];
		for(int i = 0; i < n; i++)
		{
			x[i].label = i;
			update(&x[i]);
		}
	}

	virtual ~LinkCut()
	{
		delete[] x;
	}

	void link(int u, int v)
	{
		::link(&x[u], &x[v]);
	}

	void cut(int u)
	{
		::cut(&x[u]);
	}

	void landslide(int u, int v)
	{
		if(!HasRoad(u, v))
			return;
		if(root(u)==root(v))
		{
			if(depth(u)>depth(v))
				swap(u, v);
			// v is deeper
			cut(v);
		}
	}

	void clear_rubble(int u, int v)
	{
		if(!HasRoad(u, v))
			return;
		if(root(u)!=root(v))
		{
			if(original_tree[u].depth>original_tree[v].depth)
				link(u, v);
			else
				link(v, u);
		}
	}

	int root(int u)
	{
		return ::root(&x[u])->label;
	}

	int depth(int u)
	{
		return ::depth(&x[u]);
	}

	int lca(int u, int v)
	{
		return ::lca(&x[u], &x[v])->label;
	}

	void print_distance(int u, int v)
	{
		if(root(u)!=root(v))
		{
			printf("Impossible\n");
			return;
		}
		int l=lca(u, v);
		printf("%d\n",
			[=]()
		{
			if(l==u)
				return depth(v)-depth(u);
			if(l==v)
				return depth(u)-depth(v);
			int ld=depth(l);
			return depth(u)-ld+depth(v)-ld;
		}());
	}
};

char buf[100];

void dfs_original(int root, int from, int depth, LinkCut& tree)
{
	if(from!=-1)
	{
		tree.link(root, from);
	}
	original_tree[root].depth=depth;
	original_tree[root].parent=from;
	for(auto to: original_tree[root].to)
	{
		if(to!=from)
		{
			dfs_original(to, root, depth+1, tree);
		}
	}
}

void Solve()
{
	RI(n);
	original_tree.resize(n);
	FOR(i, n-1)
	{
		RII(x, y);
		--x; --y;
		original_tree[x].to.emplace_back(y);
		original_tree[y].to.emplace_back(x);
		AddRoad(x, y);
	}
	LinkCut tree(n);
	dfs_original(0, -1, 0, tree);
	RI(q);
	FOR(i, q)
	{
		int x, y;
		scanf("%s %d %d", buf, &x, &y);
		--x; --y;
		switch(*buf)
		{
		case 'd':
			tree.landslide(x, y);
			break;
		case 'c':
			tree.clear_rubble(x, y);
			break;
		default: // 'q':
			tree.print_distance(x, y);
			break;
		}
	}
}
