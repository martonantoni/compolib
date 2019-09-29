/*
* w[i][j] = amount bidder j is willing to pay for item i (0 if he is not bidding)
* run time is O(nm^2) where n = #of items and m = #of bidders
* resets negative bids in w to 0
* returns a, where a[i] = j means ith item got assigned to bidder j
* a[i] = -1 means item i did not get assigned
* for minimizing set w[i][j] = max(w) - w[i][j] + 1
* for assigning all, w[i][j] = min(w) + w[i][j]
*/
constexpr ll INF=numeric_limits<ll>::max();

vector<int> hungarianMethod(vector<vector<ll>> w)
{
	const int n = (int)w.size(), m = (int)w[0].size(), PHI = -1, NOL = -2;
	vector<vector<bool>> x(n);
	for(auto &xx: x) xx.resize(m);
	//	boolean[] x[] = new boolean[n][m], ss = new boolean[n], st = new boolean[m];
	vector<bool> ss(n), st(m);
	//	int[] u = new int[n], v = new int[m], p = new int[m], ls = new int[n], lt = new int[m], a = new int[n];
	vector<ll> u(n), v(m), p(m), ls(n), lt(m);
	vector<int> a(n);
	ll f = 0;

	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			f = max(f, w[i][j]);

	auto fill2=[](auto &container, auto value) { std::fill(ALL(container), value); };

	fill2(u, f);
	fill2(p, INF);
	fill2(lt, NOL);
	fill2(ls, PHI);
	fill2(a, -1);

	while(true) {
		f = -1;
		for(int i = 0; i < n && f == -1; i++)
			if(ls[i] != NOL && !ss[i])
				f = i;

		if(f != -1) {
			ss[f] = true;
			for(int j = 0; j < m; j++)
				if(!x[f][j] && u[f] + v[j] - w[f][j] < p[j]) {
					lt[j] = f;
					p[j] = u[f] + v[j] - w[f][j];
				}
		}
		else {
			for(int i = 0; i < m && f == -1; i++)
				if(lt[i] != NOL && !st[i] && p[i] == 0)
					f = i;

			if(f == -1) {
				ll d1 = INF, d2 = INF, d;
				for(ll i : u)
					d1 = min(d1, i);

				for(ll i : p)
					if(i > 0)
						d2 = min(d2, i);

				d = min(d1, d2);

				for(int i = 0; i < n; i++)
					if(ls[i] != NOL)
						u[i] -= d;

				for(int i = 0; i < m; i++) {
					if(p[i] == 0)
						v[i] += d;
					if(p[i] > 0 && lt[i] != NOL)
						p[i] -= d;
				}

				if(d2 >= d1)
					break;
			}
			else {
				st[f] = true;
				int s = -1;

				for(int i = 0; i < n && s == -1; i++)
					if(x[i][f])
						s = i;

				if(s == -1) {
					for(ll l, r; ; f = r) {
						r = f;
						l = lt[r];

						if(r >= 0 && l >= 0)
							x[l][r] = !x[l][r];
						else
							break;

						r = ls[l];
						if(r >= 0 && l >= 0)
							x[l][r] = !x[l][r];
						else
							break;
					}

					fill2(p, INF);
					fill2(lt, NOL);
					fill2(ls, NOL);
					fill2(ss, false);
					fill2(st, false);

					for(int i = 0; i < n; i++) {
						bool ex = true;
						for(int j = 0; j < m && ex; j++)
							ex = !x[i][j];
						if(ex)
							ls[i] = PHI;
					}
				}
				else
					ls[s] = f;
			}
		}
	}

	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			if(x[i][j])
				a[i]=j;
	//a[j] = i;
	return a;
}

