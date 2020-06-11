const size_t pregen_p_s=1'000'001;

char is_not_prime[pregen_p_s]={ 0 };
vector<ll> primes;

vector<pair<ll, ll>> factorization; // prime, count

void CalcFactorization(ll x)
{
	factorization.clear();
	for(int i=0;is_not_prime[x] && i<(int)primes.size();++i)
	{
		again:
		if(x%primes[i]==0)
		{
			if(!factorization.empty()&&factorization.back().first==primes[i])
				++factorization.back().second;
			else
				factorization.emplace_back(primes[i], 1);
			x/=primes[i];
			goto again;
		}
	}
	if(x!=1)
		factorization.emplace_back(x, 1);

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
