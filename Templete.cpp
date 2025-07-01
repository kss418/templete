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

// UNION FIND
class _uf { 
public:
    ll n; vector <ll> p, si;
    
    _uf(){}
    _uf(ll n) { 
        this->n = n;
        p.resize(n + 1, -1); si.resize(n + 1, 1);
    }

    ll find(ll num) { 
        if (p[num] == -1) return num;
        return p[num] = find(p[num]);
    }

    void merge(ll a, ll b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (size(a) < size(b)) swap(a, b);
        p[b] = a, si[a] += si[b];
    }

    ll size(ll num){ return si[find(num)]; }
    bool same(ll a, ll b) { return find(a) == find(b); }
};

//BINARY SEARCH
template <typename T = ll>
class _bs { // 0-based index
public:
    T l, r;
    _bs(T l, T r) {
        this->l = l; this->r = r;
    }

    bool f(T cur) {

    }

    T ret() {
        T lo = l, hi = r, mid = -1;
        while (hi > lo) {
            mid = (hi + lo + 1) / (T)2;
            if (f(mid)) lo = mid;
            else hi = mid - 1;
        }

        return lo;
    }
};

// LIS
template <typename T = ll>
class _lis {
public:
    ll n; vector<T> arr;
    vector<T> result, rec;
    vector<T> num;

    _lis(ll n) {
        this->n = n;
        arr.resize(n + 1); rec.resize(n + 1);
    }

    void add(ll idx, ll n) {
        arr[idx] = n;
    }

    void init() {
        for (int i = 1; i <= n; i++) {
            ll cur = arr[i];
            auto nxt = upper_bound(num.begin(), num.end(), cur) - num.begin();
            if (nxt == num.size()) num.push_back(cur);
            else num[nxt] = cur;
            rec[i] = nxt + 1;
        }

        ll cnt = num.size();
        result.resize(cnt);
        for (int i = n; i >= 1; i--) {
            if (rec[i] != cnt) continue;
            cnt--; result[cnt] = arr[i];
        }
    }

    vector <ll> ret() {
        return result;
    }
};

//LCS
class _lcs {
public:
    string a, b; vector<vector<ll>> dp;
    ll n, m;

    _lcs(string a, string b) {
        this->a = a; this->b = b;
        this->n = a.size(), this->m = b.size();
        dp.resize(n + 1, vector<ll>(m + 1, 0));
    }

    void init() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i] == b[j]) {
                    dp[i + 1][j + 1] = dp[i][j] + 1;
                }
                else {
                    dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
                }
            }
        }
    }

    ll ret() { // lcs 길이 반환
        return dp[n][m];
    }

    string track() { // lcs string 반환
        string ret;
        ll cy = n, cx = m;
        while (cy && cx) {
            if (a[cy - 1] == b[cx - 1]) {
                ret.push_back(a[cy - 1]); cy--; cx--;
            }
            else if (dp[cy - 1][cx] > dp[cy][cx - 1]) cy--;
            else cx--;
        }

        reverse(ret.begin(), ret.end());
        return ret;
    }
};

//MO's
class _mos{
public:
    ll sq, m; vector <ll> result, arr;
    class query{
    public:
        ll l, r, n, sq;
        bool operator <(query& ot){
            if((l / sq) == (ot.l / sq)) return r < ot.r;
            return (l / sq < ot.l / sq);
        }
    };
    vector <query> q;

    _mos(ll n, ll m){ // 배열 크기, 쿼리 크기
        sq = sqrt(n); arr.resize(n + 1);
        result.resize(m + 1); this->m = m;
    }

    void add(ll l, ll r, ll n){ // 쿼리 추가
        q.push_back({l, r, n, sq});
    }

    void con(ll idx, ll val){ // 배열 삽입
        arr[idx] = val;
    }

    ll push(ll idx){
        return arr[idx];
    }

    ll pop(ll idx){
        return -arr[idx];
    }

    void init(){
        sort(q.begin(), q.end());

        ll s = q[0].l, e = q[0].r, cur = 0;
        for(int i = s;i <= e;i++) cur += push(i);
        result[q[0].n] = cur;

        for(int i = 1;i < m;i++){
            while(q[i].l > s) cur += pop(s++);
            while(q[i].r < e) cur += pop(e--);
            while(q[i].l < s) cur += push(--s);
            while(q[i].r > e) cur += push(++e);
            result[q[i].n] = cur;
        }
    }

    void print(){
        for(int i = 1;i <= m;i++) cout << result[i] << "\n";
    }
};

//TERNARY SEARCH
class _ts { // 0-based index
public:
    ll l, r;
    _ts(ll l, ll r) {
        this->l = l; this->r = r;
    }

    ll f(ll cur) {
       
       return 0;
    }

    ll ret() {
        while (r - l > 3) {
            ll st = (2 * l + r) / 3, en = (l + 2 * r) / 3;
            if(f(st) < f(en)) l = st;
            else r = en;
        }

        ll result = -INF;
        for(ll cur = l; cur <= r;cur++) result = max(result, f(cur));

        return result;
    }
};

// WEIGHTED UNION FIND
class _uf { 
public:
    ll n; vector <ll> p, d, dis;
    
    _uf(){}
    _uf(ll n) { 
        this->n = n; dis.resize(n + 1);
        p.resize(n + 1, -1); d.resize(n + 1); 
    }

    pll find(ll num) { 
        if (p[num] == -1) return { num, 0ll };
        auto [pa, diff] = find(p[num]);
        p[num] = pa; d[num] += diff;
        return { p[num], d[num] };
    }

    void merge(ll a, ll b, ll c) {
        auto [pa, da] = find(a);
        auto [pb, db] = find(b);
        if (pa == pb) { dis[pb] |= (db - da - c != 0); return; }
        p[pa] = pb; dis[pb] |= dis[pa];
        d[pa] = db - da - c; 
    }

    pll diff(ll a, ll b){
        auto [pa, da] = find(a);
        auto [pb, db] = find(b);
        if(pa == pb && !dis[pa]) return { 1, db - da };
        else if(pa == pb && dis[pa]) return { 0, -1 };
        else return { 0, 0 };
    }

    bool same(ll a, ll b) { return find(a).x == find(b).x; }
};

// UNION FIND SET
class _uf { 
public:
    ll n; vector <ll> p, si, l, r;
    
    _uf(){}
    _uf(ll n) { 
        this->n = n;
        p.resize(n + 1, -1); si.resize(n + 1, 1);
        l.resize(n + 1); r.resize(n + 1);
        iota(all(l), 0); iota(all(r), 0);
    }

    ll find(ll num) { 
        if (p[num] == -1) return num;
        return p[num] = find(p[num]);
    }

    void merge(ll a, ll b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (size(a) < size(b)) swap(a, b);
        p[b] = a, si[a] += si[b];
        l[a] = min(l[a], l[b]);
        r[a] = max(r[a], r[b]);
    }

    ll size(ll num){ return si[find(num)]; }
    bool same(ll a, ll b) { return find(a) == find(b); }
    ll prv(ll num){ return find(l[find(num)] - 1); }
    ll nxt(ll num){ return find(r[find(num)] + 1); }
};

// CHT
class _cht { 
public:
    class line{
        public:
        ll a, b;
        // max line() : line(0, MINF){}
        line() : line(0, INF){}
        line(ll a, ll b) : a(a), b(b){}
        ll f(ll x){ return a * x + b; }
    };

    _cht(){}
    vector <line> v; ll idx = 0;

    bool chk(const line& a, const line& b, const line& c) const{
        return (__int128_t)(a.b - b.b) * (c.a - b.a) >= (__int128_t)(b.b - c.b) * (b.a - a.a);
    }
    bool chk(const line& a, const line& b, ll c) const{
        return (__int128_t)c * (b.a - a.a) <= a.b - b.b;
        // max return (__int128_t)c * (b.a - a.a) >= a.b - b.b;
    }

    void add(ll a, ll b){
        line cur = {a, b};
        if(idx < v.size() && v.back().a == cur.a){
            if(cur.b < v.back().b) cur = v.back();
            v.pop_back();
        }

        while(v.size() >= idx + 2 && chk(v[v.size() - 2], v.back(), cur)) v.pop_back();
        v.push_back(cur); 
    }

    ll ls(ll x){ // linear search
        while(idx + 1 < v.size() && chk(v[idx], v[idx + 1], x)) idx++;
        return v[idx].f(x);
    }

    ll bs(ll x){ // binary search
        ll lo = 0, hi = v.size() - 1;
        while(lo < hi){
            ll mid = (lo + hi) >> 1ll;
            if(chk(v[mid], v[mid + 1], x)) lo = mid + 1;
            else hi = mid;
        }

        return v[lo].f(x);
    }
};
