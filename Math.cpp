#include <bits/stdc++.h>
#define fastio cin.tie(0), cout.tie(0), ios::sync_with_stdio(0);
#define all(x) (x).begin(), (x).end()
#define x first 
#define y second
using namespace std; typedef long long ll;
using ld = long double;
using ull = unsigned long long;
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
ll n, m, k, t; string s;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

//에라토스체
class _es { // flag (1 == prime / 2 == phi / 4 == mobius)
public:
    ll n; vector <ll> pv, pn, mov, phiv;
    vector <bool> v;
    _es(ll n, ll flag = 1) {
        this->n = n;

        if(flag & 1){
            pv.resize(n + 1, -1); pv[1] = 1;
            for (ll i = 2;i <= n;i++) {
                if(pv[i] != -1) continue; pv[i] = i;
                pn.push_back(i);
                for (int j = 2 * i; j <= n; j += i) {
                    if(pv[j] == -1) pv[j] = i;
                }
            }
        }

        if(flag & 2){
            phiv.resize(n + 1); iota(all(phiv), 0);
            v.resize(n + 1);
            for(ll i = 2;i <= n;i++){
                if(v[i]) continue;
                for(ll j = i;j <= n;j += i){
                    phiv[j] /= i; phiv[j] *= (i - 1); v[j] = 1;
                }
            }
        }

        if(flag & 4){
            mov.resize(n + 1, -2);
            mov[0] = 0; mov[1] = 1;
            for(ll i = 2;i <= n;i++){
                if(mov[i] != -2) continue; mov[i] = -1;
                for(ll j = 2 * i;j <= n;j += i) {
                    if(mov[j] == -2) mov[j] = 1;
                    mov[j] *= -1;
                }
                for(ll j = i * i;j <= n; j += i * i) mov[j] = 0;
            }
        }
    }

    ll mp(ll n) { return pv[n]; } // 최소 소인수 반환
    vector<ll> ret() { return pn; } // 소수 벡터 반환
    bool prime(ll n) { // 소수 판별
        if (n == 1) return 0;
        return pv[n] == n;
    }
    ll mobius(ll n){ return mov[n]; }
    ll phi(ll n){ return phiv[n]; }
};

//POW
class _pow {
public:
	_pow() {}

	ll ret(ll a, ll b){
        ll ret = 1;
        while(b){
            if(b & 1) ret *= a;
            a *= a; b >>= 1;
        }      
        return ret;
    }

	ll ret(ll a, ll b, ll p){
        ll ret = 1;
        while(b){
            if(b & 1) ret *= a % p, ret %= p;
            a *= a; a %= p; b >>= 1;
        }      
        return ret;
    }
};

//GCD, INV
class _gcd {
public:
    _gcd() {}

    tll init(ll a, ll b) {
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

//조합
class _comb{
public:
    vector <ll> fac, inv; ll n, mod;
    _comb(ll n, ll m = 998244353){ // m == MOD && m == prime
        fac.resize(n + 1); inv.resize(n + 1);
        this->mod = m; this->n = n;

        fac[0] = 1; 
        for(int i = 1;i <= n;i++) fac[i] = fac[i - 1] * i % mod;
        inv[n] = pow(fac[n], mod - 2, mod);
        for(int i = n - 1;i >= 0;i--) inv[i] = inv[i + 1] * (i + 1) % mod;
    }

    ll pow(ll a, ll p, ll mod) {
		if (!p) return 1;
		if (p == 1) return a % mod;

		ll cur = pow(a, p / 2, mod);
		cur = cur * cur % mod;
		if (p % 2 == 0) return cur % mod;
		return (cur * a) % mod;
	}

    ll ret(ll n, ll m){ // return nCm
        if(n < 0 || m < 0 || n < m) return 0;
        return fac[n] * inv[m] % mod * inv[n - m] % mod;
    }
};

//CRT
class _crt{
public:
    _crt(){} vector <pll> arr;
    tll gcd(ll a, ll b) {
        if (!b) return { a, 1, 0 };
        auto[g, x, y] = gcd(b, a % b);
        return { g, y, x - a / b * y };
    }

    void clear(){ arr.clear(); }
    void add(ll a, ll m){ arr.push_back({a, m}); }

    ll mod(ll a, ll m){
        a %= m; if(a >= 0) return a;
        return a + m;
    }

    pll crt(pll a, pll b){
        auto[g, cx, cy] = gcd(a.y, b.y);
        ll lcd = a.y / g * b.y;
        if((b.x - a.x) % g) return {-1, -1};
        ll r = mod((b.x - a.x) / g, b.y); 
        ll l = mod(cx * r, b.y); 
        ll ret = mod(a.x + l * a.y, lcd);

        return { ret, lcd };
    }

    pll ret(){
        pll cur = arr[0];
        for(int i = 1;i < arr.size();i++){
            cur = crt(cur, arr[i]);
            if(cur.x == -1) return {-1, -1};
        }
        return cur;
    }
};

//이산 로그
class _dlog{
public:
    map<ll, ll> num; vector <ll> prime;
    _dlog(){};

    void factor(ll p){ // 소인수분해
        prime.clear();  
        for(ll i = 2;i * i <= p;i++){
            if(p % i) continue;
            prime.push_back(i);
            if(i * i != p) prime.push_back(p / i);
        }
    }

    ll phi(ll p){ 
        factor(p); ll ret = p;
        for(auto& i : prime) ret /= i, ret *= i - 1;
        return ret;
    }

    ll sqrt(ll num){
        ll lo = 0, hi = num;
        while(hi > lo){
            ll mid = (lo + hi + 1) >> 1;
            if(mid * mid < num) lo = mid;
            else hi = mid - 1;
        }
        return lo;
    }

    ll pow(ll a, ll b, ll p){
        ll ret = 1;
        while(b){
            if(b & 1) ret *= a % p, ret %= p;
            a *= a; a %= p; b >>= 1;
        }      
        return ret;
    }
    
    ll ret(ll a, ll b, ll p) { // a^x = b (mod p), ret x
        ll mx = sqrt(phi(p)) + 1; num.clear();
        ll inv = pow(a, p - 2, p), cur = b;
        for(int i = 0;i < mx;i++) {
            if(num.find(cur) == num.end()) num[cur] = i;
            cur *= inv; cur %= p;
        }
        cur = 1; for(int i = 0;i < mx;i++){
            if(num.find(cur) != num.end()) return i * mx + num[cur];
            cur *= pow(a, mx, p); cur %= p;
        }
        return -1;
    }
};
