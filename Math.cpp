#include <bits/stdc++.h>
#define fastio cin.tie(0), cout.tie(0), ios::sync_with_stdio(0);
using namespace std; typedef long long ll;
using ull = unsigned long long;
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
ll n, m, k, t; string s;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

//에라토스체
class _es {
public:
    ll n; vector <ll> p, pn;
    _es(ll n) {
        this->n = n;
        p.resize(n + 1, -1);
    }

    void init() {
        p[1] = 1;
        for (int i = 2; i <= n; i++) {
            if (p[i] != -1) continue; p[i] = i;
            pn.push_back(i);
            for (int j = 2 * i; j <= n; j += i) {
                if (p[j] == -1) p[j] = i;
            }
        }
    }

    ll mp(ll n) { // 최소 소인수 반환
        return p[n];
    }

    vector<ll> ret() { // 소수 벡터 반환
        return pn;
    }

    bool prime(ll n) { // 소수 판별
        if (n == 1) return 0;
        return p[n] == n;
    }
};

//POW
class _pow {
public:
	_pow() {}

	ll ret(ll a, ll p) {
		if (!p) return 1;
		if (p == 1) return a;

		ll cur = ret(a, p / 2);
		cur = cur * cur;
		if (p % 2 == 0) return cur;
		return cur * a;
	}

	ll ret(ll a, ll p, ll mod) {
		if (!p) return 1;
		if (p == 1) return a % mod;

		ll cur = ret(a, p / 2, mod);
		cur = cur * cur % mod;
		if (p % 2 == 0) return cur % mod;
		return (cur * a) % mod;
	}
};

//GCD, INV
class _gcd {
public:
    _gcd() {}

    tuple <ll, ll, ll> init(ll a, ll b) {
        if (!b) return { a, 1, 0 };
        auto [g, x, y] = init(b, a % b);
        return { g, y, x - a / b * y };
    }

    ll ret(ll a, ll b) {
        return get<0>(init(a, b));
    }

    ll inv(ll a, ll mod) {
        auto [g, x, y] = init(a, mod);
        if (!a || g != 1) return -1;
        return (x < 0 ? x + mod : x);
    }
};

//p*q-1
class _inv {
public:
    _inv() {}
    ll pow_mod(ll a, ll b, ll c) {
        if (!b) return 1;
        ll ret = pow_mod(a, b / 2, c) % c;

        if (b % 2) return ((ret * ret) % c * (a % c)) % c;
        return (ret * ret) % c;
    }

    ll prime_inv(ll a, ll b) {
        return pow_mod(a, b - 2, b);
    }

    ll ret(ll a, ll b, ll m) { // (a * b^-1) % m 반환 m == prime
        return (a % m * prime_inv(b, m) % m) % m;
    }
};

//순열

//조합

//CRT
