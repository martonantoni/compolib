const size_t max_prime_to_generate = 10'000'001;

char is_not_prime[max_prime_to_generate] = { 0 };
vector<ll> primes;

vector<pair<ll, ll>> factorization; // prime, count

void calcFactorization(ll x)
{
    factorization.clear();
    for (auto prime : primes)
    {
        if (x < max_prime_to_generate && !is_not_prime[x])  // shortcut
        {
            if (x <= 1)
                return;
            factorization.emplace_back(x, 1);
            return;
        }
        if (x % prime == 0)
        {
            factorization.emplace_back(prime, 1);
            x /= prime;
            while (x % prime == 0)
            {
                ++factorization.back().second;
                x /= prime;
            }
        }
    }
    if (x != 1)
        factorization.emplace_back(x, 1);
}

void initPrimes()
{
    primes.reserve(100'000);
    for (int i = 2; i < max_prime_to_generate; ++i)
    {
        if (!is_not_prime[i])
        {
            primes.push_back(i);
            for (int j = i * 2; j < max_prime_to_generate; j += i)
            {
                is_not_prime[j] = 1;
            }
        }
    }
}

vector<ll> divisors;

void generateAllDivisors(const vector<pair<ll, ll>>& factors = ::factorization) // divisors are not ordered!
{
    divisors.clear();
    divisors.emplace_back(1);
    for (auto&& [prime, count] : factors)
    {
        int ds = (int)divisors.size();
        FOR(j, ds)
        {
            ll d = divisors[j];
            FOR(i, count)
            {
                d *= prime;
                divisors.emplace_back(d);
            }
        }
    }
}
