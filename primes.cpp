const size_t pregen_p_s=1'000'001;

char is_not_prime[pregen_p_s]={ 0 };
vector<ll> primes;

vector<pair<ll, ll>> factorization; // prime, count

void CalcFactorization(ll x)
{
    factorization.clear();
    for (int i = 0; i < (int)primes.size() && x>1; ++i)
    {
        if (!is_not_prime[x])  // WARNING:  x must be <= pregen_p_s!
        {
            factorization.emplace_back(x, 1);
            return;
        }
    again:
        if (x % primes[i] == 0)
        {
            if (!factorization.empty() && factorization.back().first == primes[i])
                ++factorization.back().second;
            else
                factorization.emplace_back(primes[i], 1);
            x /= primes[i];
            goto again;
        }
    }
    if (x != 1)
        factorization.emplace_back(x, 1);
}

ll lkkt(ll a, ll b) // legkisebb kozos tobbszoros, nagyon nem optimalizalt
{
    CalcFactorization(a);
    auto af = move(factorization);
    CalcFactorization(b);
    auto bf = move(factorization);
    ll r = 1;
    auto cp = [](ll a, ll b)
    {
        ll r = 1;
        for (; b > 0; --b)
            r *= a;
        return r;
    };
    for (auto f : af)
    {
        auto i = find_if(ALL(bf), [p = f.first](auto g) { return g.first == p; });
        if (i == bf.end())
        {
            r *= cp(f.first, f.second);
        }
        else
        {
            r *= cp(f.first, max(i->second, f.second));
            bf.erase(i);
        }
    }
    for (auto f : bf)
    {
        r *= cp(f.first, f.second);
    }
    return r;
}


void InitPrimes()
{	
	primes.reserve(100'000);
	for(int i=2; i<pregen_p_s; ++i)
	{
		if(!is_not_prime[i])
		{
			primes.push_back(i);
			for(int j=i*2; j<pregen_p_s; j+=i)
			{
				is_not_prime[j]=1;
			}
		}
	}
}

vector<ll> divisors;

void GenerateAllDivisors()      // Call CalcFactorization first!
{                               // divisors are not ordered!
    divisors.clear();
    divisors.emplace_back(1);
    for (auto f : factorization)
    {
        int ds = divisors.size();
        FOR(j, ds)
        {
            ll d = divisors[j];
            FOR(i, f.second)
            {
                d *= f.first;
                divisors.emplace_back(d);
            }
        }
    }
}
