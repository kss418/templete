#include <bits/stdc++.h>
#define fastio cin.tie(0), cout.tie(0), ios::sync_with_stdio(0);
using namespace std; typedef long long ll;
using ull = unsigned long long;
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
ll n, m, k, t; string s;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

//KMP

//TRIE

//HASH
class _hash { // 0-based index
public:
    // ll mod[3] = { MOD, 998244353, 922337203685471 };
    // ll key[3] = {29, 31, 26};
    ll n, m, mod, key;
    vector <ll> h;

    _hash(ll n, ll m, ll mod, ll key = 2) {
        this->n = n; this->m = m;
        this->mod = mod; this->key = key;
        h.resize(n + 1, 0);
    }

    ll mo(ll n) {
        if (n % mod >= 0) return n % mod;
        else return (n % mod) + mod;
    }

    void init(string& arr) {
        ll pow = 1;
        for (int i = 0; i < m; i++) {
            h[0] = mo(h[0] + arr[m - i - 1] * pow);
            if (i != m - 1) pow = mo(key * pow);
        }

        for (int i = 1; i <= n - m; i++) {
            h[i] = mo(key * mo(h[i - 1] - arr[i - 1] * pow) + arr[i + m - 1]);
        }
    }

    ll ret(ll a) {
        return mo(h[a]);
    }
};

