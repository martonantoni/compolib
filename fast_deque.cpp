template<int max_n>
class cFastDeque
{
	int values[max_n*2+1];
	int b=max_n, e=max_n;
public:
	bool empty() const { return b==e; }
	int front() const { return values[b]; }
	int back() const { return values[e-1]; }
	void push_front(int v) { --b; values[b]=v; }
	void push_back(int v) { values[e]=v; ++e; }
	void pop_front() { ++b; }
	void pop_back() { --e; }
	void clear() { b=max_n; e=max_n; }
};