#include <bits/stdc++.h>
#define all(x) (x).begin(), (x).end()
#define x first 
#define y second
using namespace std; using ll = long long; using u64 = uint64_t;
using ld = long double; using pld = pair<ld, ld>;
using i128 = __int128_t; using f128 = __float128; 
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;
constexpr ll MINF = 0xc0c0c0c0c0c0c0c0;

//에라토스체
template <size_t n>
class _es {
public:
    static constexpr size_t sz = (n >> 1) + 1; 
    bitset <sz> bs;
    vector <int> prime;
    _es() { init(); }

    void init(){
        if(n >= 2) prime.push_back(2);
        bs.set(0);
        for(int i = 3;i <= n;i += 2){
            if(bs[i >> 1]) continue;
            prime.push_back(i);
            for(ll j = 1ll * i * i;j <= (ll)n;j += 2ll * i) bs.set(j >> 1);
        }
    }

    const vector<int>& ret() { return prime; } // 소수 벡터 반환
    bool is_prime(int x) const { // 소수 판별
        if(x == 2) return 1;
        if(x < 2 || x % 2 == 0) return 0;
        return !bs[x >> 1];
    }
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

//조합
class _comb{
public:
    vector <_mint> fac, inv; int n; ll m; // m == prime
    _comb(int n, ll m = 998244353) : n(n), m(m), fac(n + 1), inv(n + 1){ 
        assert(n < m);
        fac[0] = _mint(1, m);
        for(int i = 1;i <= n;i++) fac[i] = fac[i - 1] * i;
        inv[n] = fac[n].pow(m - 2);
        for(int i = n - 1;i >= 0;i--) inv[i] = inv[i + 1] * (i + 1);
    }

    ll ret(ll n, ll r){
        if(n < 0 || r < 0 || n < r) return 0;
        return (fac[n] * inv[r] * inv[n - r]).v;
    }
};

//CRT
class _crt{ // Require _mint
public:
    _crt(){} vector <_mint> arr;
    tll gcd(ll a, ll b){
        if(!b) return {a, 1, 0};
        auto [g, x, y] = gcd(b, a % b);
        return {g, y, x - (a / b) * y}; 
    }

    bool crt(const _mint& a, const _mint& b, _mint& ret){
        auto[g, cx, cy] = gcd(a.mod, b.mod);
        ll lcm = (ll)((i128)a.mod / g * b.mod), diff = b.v - a.v;
        if(diff % g) return 0;

        _mint k = _mint(diff / g, b.mod / g) * cx;
        ret = _mint(a.v, lcm);
        ret += _mint(a.mod, lcm) * _mint(k.v, lcm);
        return 1;
    }

    pll ret(){ // O(log mod)
        if(arr.empty()) return {0, 1}; _mint cur = arr[0];
        for(int i = 1;i < arr.size();i++){
            _mint nxt;
            if(!crt(cur, arr[i], nxt)) return {-1, -1};
            cur = nxt;
        }
        return {cur.v, cur.mod};
    }

    void add(ll a, ll m){ arr.emplace_back(a, m); }
    void add(pll a){ arr.push_back({a.x, a.y}); }
    void add(const _mint& a){ arr.push_back(a); }
    void clear(){ arr.clear(); }
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

//FFT
template <typename T = ll>
class _fft{
public:
    const ld PI = acos(-1);
    using cpd = complex <double>;

    void fft(vector <cpd>& v, bool inv = 0){
        ll n = v.size();
        for(int i = 1,j = 0;i < n;i++){
            ll bit = n >> 1ll;
            for(;j >= bit;bit >>= 1ll) j -= bit;
            j += bit;
            if(i < j) swap(v[i], v[j]);
        }

        for(int k = 1;k < n;k <<= 1ll){
            double a = PI / k * (inv ? 1 : -1);
            cpd w(cos(a), sin(a));
            for(int i = 0;i < n;i += 2 * k){
                cpd wp(1, 0);
                for(int j = 0;j < k;j++){
                    cpd x = v[i + j], y = v[i + j + k] * wp;
                    v[i + j] = x + y;
                    v[i + j + k] = x - y;
                    wp *= w;
                }
            }
        }

        if(inv) for(int i = 0;i < n;i++) v[i] /= n;
    }  
        
    vector <T> mul(const vector <T>& a,const vector <T>& b){
        vector<cpd> av(all(a)), bv(all(b));
        ll n = 2;
        while(n < a.size() + b.size()) n <<= 1;
        av.resize(n); bv.resize(n);
        fft(av); fft(bv);
        for(int i = 0;i < n;i++) av[i] *= bv[i];
        fft(av, 1);

        vector <T> ret(n);
        for(int i = 0;i < n;i++) ret[i] = round(av[i].real());
    
        return ret;
    }
        
    vector <T> carry(vector <T>& a, vector <T>& b){
        reverse(all(a)); reverse(all(b));
        vector <T> ret = mul(a, b);

        for(int i = 0;i < ret.size();i++){
            if(ret[i] < 10) continue;
            if(i < ret.size() - 1) ret[i + 1] += ret[i] / 10;
            else ret.push_back(ret[i] / 10);
            ret[i] %= 10;
        }
        
        while(!ret.empty() && !ret.back()) ret.pop_back();
        if(ret.empty()) return vector<T>(1, 0);
        reverse(all(ret));
        return ret;
    }
};

//Pollard Rho
class _pr{
public:
    using u128 = __uint128_t; mt19937_64 rnd;
    _pr() : rnd((u64)chrono::steady_clock::now().time_since_epoch().count()){}
    
    u64 mul_mod(u64 a, u64 b, u64 mod){ return (u128)a * b % mod; }
    u64 pow_mod(u64 a, u64 b, u64 mod){
        u64 ret = 1; a %= mod;
        while(b){
            if(b & 1) ret = mul_mod(a, ret, mod);
            a = mul_mod(a, a, mod);
            b >>= 1ll;
        }
        return ret;
    }

    bool isprime(u64 n){
        if(n < 2) return 0;
        for(u64 i : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}){
            if(n == i) return 1;
            if(n % i == 0) return 0;
        }

        u64 d = n - 1, s = 0;
        while((d & 1) == 0) d >>= 1ll, ++s;

        auto chk = [&](u64 a) -> bool{
            if(a % n == 0) return 0;
            u64 x = pow_mod(a, d, n);
            if(x == 1 || x == n - 1) return 0;
            for(u64 r = 1;r < s;r++){
                x = mul_mod(x, x, n);
                if(x == n - 1) return 0;
            }
            return 1;
        };

        for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL}) {
            if (chk(a)) return 0;
        }
        return 1;
    }

    u64 f(u64 x, u64 c, u64 mod){ return ((u128)mul_mod(x, x, mod) + c) % mod; }
    u64 pollard(u64 n){ // 임의의 소인수 반환
        for(u64 i : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}){
            if(n % i == 0) return i;
        }
        uniform_int_distribution<u64> rx(2, n - 2), rc(1, n - 1);

        while(1){
            u64 a = rx(rnd), b = a;
            u64 c = rc(rnd), g = 1;
            while(g == 1){
                a = f(a, c, n);
                b = f(f(b, c, n), c, n);
                u64 diff = (a > b) ? (a - b) : (b - a);
                g = gcd(diff, n);
            }

            if(g != n) return g;
        }
    }

    void dnc(u64 n, vector<u64>& ret){ 
        if(n == 1) return;
        if(isprime(n)){
            ret.push_back(n);
            return;
        }

        u64 p = pollard(n);
        dnc(p, ret); dnc(n / p, ret);
    }

    vector <u64> factorize(u64 n){ // 소인수분해
        vector<u64> ret;
        dnc(n, ret);
        sort(all(ret));
        return ret;
    }

    vector <pair<u64, int>> factorize_cnt(u64 n){
        auto f = factorize(n);
        vector<pair<u64, int>> ret;
        if(n == 1) { ret.push_back({1, 1}); return ret; }
        for(u64& p : f){
            if(ret.empty() || ret.back().x != p) ret.push_back({p, 1});
            else ret.back().y++;
        }
        return ret;
    }

    u64 phi(u64 n){
        if(!n) return 0;
        auto fc = factorize_cnt(n);
        u128 ret = n;
        for(auto& [p, c] : fc) ret = ret / p * (p - 1);
        return (u64)ret;
    }

    int mobius(u64 n){
        if(n == 0 || n == 1) return n;
        auto fc = factorize_cnt(n);
        for(auto& [p, c] : fc) if(c >= 2) return 0;
        return fc.size() % 2 ? -1 : 1;
    }

    u64 divisor_num(u64 n){
        if(!n) return 0;
        auto fc = factorize_cnt(n);
        u128 ret = 1;
        for(auto& [p, c] : fc) ret *= c + 1;
        return (u64)ret;
    }

    u64 divisor_sum(u64 n){
        if(!n) return 0;
        auto fc = factorize_cnt(n);
        u128 ret = 1;
        for(auto& [p, c] : fc){
            u128 now = 1, cur = 1;
            for(int i = 0;i < c;i++){
                cur *= p;
                now += cur;
            }
            ret *= now;
        }
        return (u64)ret;
    }
};

class _lucas{ // mod == prime
public:
    int mod; vector <_mint> fac, inv;
    _lucas(int mod) : mod(mod), fac(mod), inv(mod){ // O(mod)
        fac[0] = _mint(1, mod);
        for(int i = 1;i < mod;i++) fac[i] = fac[i - 1] * i;
        inv[mod - 1] = fac[mod - 1].pow(mod - 2);
        for(int i = mod - 2;i >= 0;i--) inv[i] = inv[i + 1] * (i + 1);
    }

    _mint cal(ll n, ll r){ // O(1)
        if(n < r) return _mint(0, mod);
        return fac[n] * inv[r] * inv[n - r];
    }

    int ret(ll n, ll r){ // O(log n)
        if(n < r || n < 0 || r < 0) return 0;
        if(!n || !r || n == r) return 1;
        return (cal(n % mod, r % mod) * ret(n / mod, r / mod)).v;
    }
};

class _ls{
public:
    vector <int> lp, prime, vphi, vmu; int n, flag; 
    _ls(){}
    _ls(int n, int flag = 0) : n(n), flag(flag){ init(); }
    
    void init(){
        lp.assign(n + 1, 0);
        if(n >= 3) prime.reserve((int)(n / log((ld)n)) + 10);
        if(flag & 1) vphi.assign(n + 1, 0), vphi[1] = 1;
        if(flag & 2) vmu.assign(n + 1, 0), vmu[1] = 1;

        for(int i = 2;i <= n;i++){
            if(!lp[i]){
                lp[i] = i;
                prime.push_back(i);
                if(flag & 1) vphi[i] = i - 1;
                if(flag & 2) vmu[i]  = -1;
            }

            for(auto& p : prime){
                ll x = 1ll * p * i;
                if(x > n) break;
                lp[x] = p;
                
                if(i % p == 0){
                    if(flag & 1) vphi[x] = vphi[i] * p;
                    if(flag & 2) vmu[x] = 0;
                    break;
                }
                else{
                    if(flag & 1) vphi[x] = vphi[i] * (p - 1);
                    if(flag & 2) vmu[x] = -vmu[i];
                }
            }
        }
    }

    bool is_prime(int x) const{ return x >= 2 && lp[x] == x; }
    vector <ll> factorize(int x){ // 소인수분해
        vector <ll> ret;
        while(x != 1){
            ret.push_back(lp[x]);
            x /= lp[x];
        }
        return ret;
    }

    vector <pll> factorize_cnt(int x){ // p^c 꼴로 소인수분해
        vector <pll> ret;
        while(x != 1){
            ll p = lp[x], c = 0;
            while(x % p == 0) x /= p, c++;
            ret.push_back({p, c});
        }
        return ret;
    }

    ll divisor_num(int x){ // 약수의 개수
        ll ret = 1;
        while(x != 1){
            ll p = lp[x], c = 0;
            while(x % p == 0) x /= p, c++;
            ret *= c + 1;
        }
        return ret;
    }

    ll divisor_sum(int x){  // 약수의 합
    if(x == 0 || x == 1) return x;   
    ll ret = 1;
    while(x != 1){
        ll p = lp[x], sum = 1, cur = 1;
        while(x % p == 0){
            x /= p;
            cur *= p;
            sum += cur;
        }
        ret *= sum;
    }
    return ret;
}

    vector <ll> divisor(int x){ // 약수 벡터 반환
        if(!x) return {};
        if(x == 1) return {1};
        auto fc = factorize_cnt(x);
        vector <ll> ret = {1};
        for(auto& [p, c] : fc){
            ll mul = 1, sz = ret.size();
            for(ll k = 1;k <= c;k++){
                mul *= p;
                for(int i = 0;i < sz;i++) ret.push_back(ret[i] * mul);
            }
        }

        sort(all(ret));
        return ret;
    }

    ll phi(int x) const { return vphi[x]; } // 오일러 피
    ll mobius(int x) const { return vmu[x]; } // 뫼비우스
};

// comb mod (p1^e1 + p2^e2...)
class _comb_crt{ // Require mint, crt
public:
    class _comb{ // O(p1^e1 + p2^e2 ...)
    public:
        vector <_mint> pre; ll p, e, mod = 1; 
        _comb(ll prime, ll exp) : p(prime), e(exp){
            for(int i = 1;i <= e;i++) mod *= p;
            pre.assign(mod + 1, _mint(1, mod));
            for(ll i = 1;i <= mod;i++){
                pre[i] = pre[i - 1];
                if(i % p) pre[i] *= i;
            }
        }   
        
        ll vp(ll n) const{
            ll ret = 0;
            while(n){ n /= p; ret += n; }
            return ret;
        }

        _mint f(ll n) const{ // p^e = p^k * f(n)
            if(!n) return _mint(1, mod);
            ll div = n / mod, rem = n % mod; _mint ret(1, mod);
            if(p % 2) ret = div & 1 ? ret *= _mint(mod - 1, mod) : _mint(1, mod);
            else ret *= pre[mod].pow(div);
            ret *= pre[rem]; ret *= f(n / p);
            return ret;
        }

        ll ret(ll n, ll r) const{ // O(log n + log p)
            if(r < 0 || r > n) return 0;
            ll now = vp(n) - vp(r) - vp(n - r);
            if(now >= e) return 0;

            _mint div = f(r) * f(n - r), inv = div.inv();
            return (f(n) * inv * _mint(p, mod).pow(now)).v;
        }
    }; 

    vector <_comb> c; vector <_lucas> l; _crt crt;
    void add(ll p, ll e){ // O(1) 
        if(e <= 0) return;
        if(e == 1) l.emplace_back(p);
        else c.emplace_back(p, e);
    }

    ll ret(ll n, ll r){ // O(log n + log p)
        crt.clear();
        for(auto& i : c) crt.add(i.ret(n, r), i.mod);
        for(auto& i : l) crt.add(i.ret(n, r), i.mod);
        return crt.ret().x;
    }

    void clear(){ l.clear(); c.clear(); }
};

// Prime Tower
class _pt{
public:
    vector <ll> arr; _crt crt;
    void add(ll v){ // O(1) 
        if(!arr.empty() && arr.back() <= 1) return;
        arr.push_back(v); 
    }

    void add_mod(ll p, ll e){ // O(sqrt(n) * log n)
        ll mod = 1;
        for(int i = 1;i <= e;i++) mod *= p;
        crt.add(cal(0, mod), mod); 
    }

    void add_mod(ll m){ // O(sqrt(n) * log n)
        for(ll i = 2;i * i <= m;i++){
            if(m % i) continue; ll c = 0;
            while(m % i == 0) m /= i, c++;
            add_mod(i, c);
        }
        if(m > 1) add_mod(m, 1);
    }

    i128 mul_cap(ll a, ll b, ll cap){
        if(!a || !b) return 0;
        if(a >= cap) return cap;
        i128 ret = (i128)a * b;
        return (ret >= (i128)cap) ? cap : ret; 
    }

    ll pow_cap(ll a, ll b, ll cap){
        i128 ret = 1;
        while(b){
            if(b & 1){
                ret = mul_cap(ret, a, cap);
                if(ret >= cap) return cap;
            }
            b >>= 1ll;
            if(!b) break;
            a = mul_cap(a, a, cap);
            if (a >= cap) a = cap;
        }
        return ret;
    }

    ll pow_mod(ll a, ll b, ll p){ return _mint(a, p).pow(b).v; }
    ll cal_phi(ll x){
        ll ret = x;
        for(ll i = 2;i * i <= x;i++){
            if(x % i) continue;
            while(x % i == 0) x /= i;
            ret *= i - 1; ret /= i;
        }
        if(x > 1) ret *= x - 1, ret /= x;
        return ret;
    }

    ll cal_exact(int cur){
        if(cur + 1 == arr.size()) return arr[cur];
        return pow_cap(arr[cur], cal_exact(cur + 1), 100);
    }

    bool tower_le(int cur, ll num){
        if((int)arr.size() - cur > 4) return 0;
        ll now = arr.back();
        for(int i = (int)arr.size() - 2;i >= cur;i--){
            if(now > num) return 0;
            now = pow_cap(arr[i], now, num + 1);
        }
        return now <= num;
    }

    ll cal(int cur, ll mod){
        if(mod == 1) return 0;
        if(arr[cur] == 1) return 1;
        if(cur + 1 == arr.size()) return _mint(arr[cur], mod).v;
        if(tower_le(cur + 1, 100)) return pow_mod(arr[cur], cal_exact(cur + 1), mod);
        ll phi = cal_phi(mod), e = cal(cur + 1, phi) + 100 * phi;
        return pow_mod(arr[cur], e, mod);
    }

    ll ret(){ return crt.ret().x; } 
};

class _mint{
public:
    ll mod, v;
    _mint(ll v, ll mod) : mod(mod), v(norm(v, mod)) {}
    static ll norm(ll x, ll m){ x %= m; return x < 0 ? x + m : x; }
    static tll gcd(ll a, ll b){
        if (b == 0) return {a, 1, 0};
        auto [g, x, y] = gcd(b, a % b);
        return {g, y, x - (a / b) * y};
    }

    _mint inv() const{
        assert(v);
        auto [g, x, y] = gcd(v, mod);
        assert(g == 1 || g == -1);
        if(g == -1) x *= -1;
        return _mint(x, mod);
    }

    _mint pow(ll e) const{
        assert(e >= 0);
        _mint base = *this, ret(1, mod);
        while(e){
            if(e & 1) ret *= base;
            base *= base; e >>= 1ll;
        }
        return ret;
    }

    _mint& operator+=(const _mint& ot){
        assert(mod == ot.mod);
        v += ot.v;
        if (v >= mod) v -= mod;
        return *this;
    }

    _mint& operator-=(const _mint& ot){
        assert(mod == ot.mod);
        v -= ot.v;
        if (v < 0) v += mod;
        return *this;
    }

    _mint& operator*=(const _mint& ot){
        assert(mod == ot.mod);
        v = (ll)((i128)v * ot.v % mod);
        return *this;
    }
    _mint& operator/=(const _mint& ot){ return (*this) *= ot.inv(); }

    friend _mint operator+(_mint a, const _mint& b){ return a += b; }
    friend _mint operator-(_mint a, const _mint& b){ return a -= b; }
    friend _mint operator*(_mint a, const _mint& b){ return a *= b; }
    friend _mint operator/(_mint a, const _mint& b){ return a /= b; }
    _mint& operator+=(ll b){ return (*this) += _mint(b, mod); }
    _mint& operator-=(ll b){ return (*this) -= _mint(b, mod); }
    _mint& operator*=(ll b){ return (*this) *= _mint(b, mod); }
    _mint& operator/=(ll b){ return (*this) /= _mint(b, mod); }
    friend _mint operator+(_mint a, ll b){ return a += b; }
    friend _mint operator-(_mint a, ll b){ return a -= b; }
    friend _mint operator*(_mint a, ll b){ return a *= b; }
    friend _mint operator/(_mint a, ll b){ return a /= b; }
    friend ostream& operator<<(ostream& os, const _mint& a){ return os << a.v; }
};
