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

// UNION FIND
class _uf { 
public:
    ll n; vector <ll> p, sz;
    
    _uf(){}
    _uf(ll n) {  
        this->n = n;
        p.resize(n + 1, -1); sz.resize(n + 1, 1);
    }

    ll find(ll num) { 
        if (p[num] == -1) return num;
        return p[num] = find(p[num]);
    }

    void merge(ll a, ll b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (size(a) < size(b)) swap(a, b);
        p[b] = a, sz[a] += sz[b];
    }

    ll size(ll num){ return sz[find(num)]; }
    bool same(ll a, ll b) { return find(a) == find(b); }
    void clear(){ p.assign(n + 1, -1); sz.assign(n + 1, 1); }
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
    ll sq, n, m, cur; vector <ll> result, arr;
    class query{
    public:
        ll l, r, n, sq;
        bool operator <(const query& ot) const{
            if((l / sq) == (ot.l / sq)) return r < ot.r;
            return (l / sq < ot.l / sq);
        }
    };
    vector <query> q;

    _mos(){}
    _mos(ll n, ll m) : n(n), m(m){ // 배열 크기, 쿼리 크기
        sq = sqrt(n); arr.resize(n + 1);
        result.resize(m + 1);
    }

    void add(ll l, ll r, ll n){ // 쿼리 추가
        q.push_back({l, r, n, sq});
    }

    void set(ll idx, ll val){ // 배열 삽입
        arr[idx] = val;
    }

    void push(ll idx){

    }   

    void pop(ll idx){

    }

    void init(){
        if(q.empty()) return;
        sort(all(q));

        ll s = q[0].l, e = s - 1;
        while (e < q[0].r) push(++e);
        while (s > q[0].l) push(--s);
        while (s < q[0].l) pop(s++);
        result[q[0].n] = cur;

        for(int i = 1;i < m;i++){
            while(q[i].l < s) push(--s);
            while(q[i].r > e) push(++e);
            while(q[i].l > s) pop(s++);
            while(q[i].r < e) pop(e--);
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
class _cht{
public:
    class line{
    public:
        ll a, b; line(){}
        line(ll a, ll b) : a(a), b(b){}
        ll f(ll x) const { return a * x + b; }
    };
    line id_line() const{ return is_min ? line(0, INF) : line(0, MINF);}

    vector<line> v;
    bool is_min = 1; ll idx = 0;
    _cht(bool is_min = 1) : is_min(is_min) {}
    void clear(){ v.clear(); idx = 0; }

    bool chk(const line& a, const line& b, const line& c) const{
        i128 x1 = (i128)(a.b - b.b) * (c.a - b.a), x2 = (i128)(b.b - c.b) * (b.a - a.a);
        return is_min ? x1 <= x2 : x1 >= x2;
    }

    bool better(const line& a, const line& b, ll x) const{
        i128 y1 = (i128)a.a * x + a.b, y2 = (i128)b.a * x + b.b;
        if(y1 == y2){
            // tie break
        }
        return is_min ? y1 <= y2 : y1 >= y2; 
    }

    // 기울기가 같은 두 직선을 비교
    bool better_same(const line& a, const line& b) const{
        if(a.b == b.b){
            // tie break
        }
        return is_min ? a.b <= b.b : a.b >= b.b;
    }

    // 기울기 단조 증가 순으로 넣을 것
    void add(line cur){
        if (!v.empty() && v.back().a == cur.a){
            if (!better_same(cur, v.back())) return;
            v.pop_back();
        }

        while (v.size() >= 2 && chk(v[v.size() - 2], v.back(), cur)) v.pop_back();
        v.push_back(cur);
        if (idx >= (ll)v.size()) idx = (ll)v.size() - 1;
    }

    // 단조 증가 순으로 넣을 것
    line ls(ll x){
        if(v.empty()) return id_line();
        while (idx + 1 < (ll)v.size() && better(v[idx + 1], v[idx], x)) idx++;
        return v[idx];
    }

    line bs(ll x) const{
        if(v.empty()) return id_line();
        ll lo = 0, hi = (ll)v.size() - 1;
        while (lo < hi){
            ll mid = (lo + hi) >> 1ll;
            if (better(v[mid + 1], v[mid], x)) lo = mid + 1;
            else hi = mid;
        }
        return v[lo];
    }
};

// PBS
class _pbs{
public:
    ll lo, hi;
    vector <ll> l, r;
    vector <vector <ll>> arr;

    _pbs(){}
    _pbs(ll lo, ll hi) : lo(lo), hi(hi){}

    class query{ 
    public:
        ll v;
    }; vector <query> q;
    void add(query a){ q.push_back(a); }

    void clear(){
        for(ll i = lo;i <= hi;i++) arr[i].clear();
    }

    bool f(ll cur, ll idx){ // 결정 함수
        query& now = q[idx];

        return 0;
    }

    void init(){
        arr.resize(hi + 1);
        l.resize(q.size(), lo); r.resize(q.size(), hi);

        while(1){
            clear(); bool flag = 0;
            for(int i = 0;i < q.size();i++){
                if(l[i] >= r[i]) continue;
                ll mid = (l[i] + r[i] + 1) >> 1ll;
                arr[mid].push_back(i); flag = 1;
            }
            if(!flag) break;

            ll num = 0;
            for(ll mid = lo;mid <= hi;mid++){
                for(auto& idx : arr[mid]){
                    if(f(mid, idx)) l[idx] = mid;
                    else r[idx] = mid - 1;
                }
            }
        }
    }
};
