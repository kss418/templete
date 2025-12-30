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

//SUM SEG
class _sseg {
public:
    ll n; vector <ll> seg;
    _sseg(){}
    _sseg(ll n) { this->n = n; seg.resize(4 * n + 1); }

    ll query(ll st, ll en) { return query(st, en, 0, n); }
    ll query(ll st, ll en, ll l, ll r, ll node = 1) {
        if (en < l || st > r) return 0;
        if (st <= l && en >= r) return seg[node];
        ll mid = (l + r) >> 1;
        return query(st, en, l, mid, node * 2) + query(st, en, mid + 1, r, node * 2 + 1);
    }

    void add(ll idx, ll val) { add(idx, val, 0, n); }
    void add(ll idx, ll val, ll l, ll r, ll node = 1) {
        if (idx < l || idx > r) return;
        seg[node] += val;
        if (l == r) return;
        ll mid = (l + r) >> 1;
        add(idx, val, l, mid, node * 2);
        add(idx, val, mid + 1, r, node * 2 + 1);
    }

    ll update(ll idx, ll val) { return update(idx, val, 0, n); }
    ll update(ll idx, ll val, ll l, ll r, ll node = 1) {
        if (idx < l || idx > r) return seg[node];
        if (l == r) return seg[node] = val;
        ll mid = (l + r) >> 1;

        seg[node] = update(idx, val, l, mid, node * 2);
        seg[node] += update(idx, val, mid + 1, r, node * 2 + 1);
        return seg[node];
    }

    ll cnt(ll val) { return cnt(val, 0, n); }
    ll cnt(ll val, ll l, ll r, ll node = 1){
        if(l == r) return r;
        ll mid = (l + r) >> 1;

        if(seg[node * 2] < val) return cnt(val - seg[node * 2], mid + 1, r, node * 2 + 1);
        else return cnt(val, l, mid, node * 2);
    }
};

//SEG
struct seg_policy{
    struct node { ll v; };
    static node id(){ return node{0}; }
    static node op(const node& l, const node& r){
        return node{
            l.v + r.v
        };
    }
};

template <class policy = seg_policy>
class _seg {
private:
    using node = typename policy::node;
    int n, sz; vector <node> seg;
    node op(const node& l, const node& r) const{ return policy::op(l, r); }
    node id() const{ return policy::id(); }
public:
    _seg(int n = 0){ clear(n); }
    void clear(int n){
        this->n = n;
        sz = 1; while(sz < n + 1) sz <<= 1;
        seg.assign(2 * sz, id());
    }

    void build(const vector<node>& arr){
        if(arr.empty()){ clear(0); return; }
        clear((int)arr.size() - 1);
        for(int i = 0;i < (int)arr.size();i++) seg[i + sz] = arr[i];
        for(int i = sz - 1;i >= 1;i--) seg[i] = op(seg[i << 1], seg[i << 1 | 1]);
    }

    node query(int st, int en) {
        st = max(0, st); en = min(n, en);
        if(st > en) return id();
        node l = id(), r = id();
        st += sz; en += sz;
        while(st <= en){
            if(st & 1) l = op(l, seg[st++]);
            if(!(en & 1)) r = op(seg[en--], r);
            st >>= 1; en >>= 1; 
        }
        return op(l, r);
    }

    void set(int idx, const node& v){
        if(idx < 0 || idx > n) return;
        int p = idx + sz; seg[p] = v;
        for(p >>= 1; p; p >>= 1) seg[p] = op(seg[p << 1], seg[p << 1 | 1]);
    }

    void add(int idx, const node& v){
        if(idx < 0 || idx > n) return;
        int p = idx + sz; seg[p] = op(seg[p], v);
        for(p >>= 1; p; p >>= 1) seg[p] = op(seg[p << 1], seg[p << 1 | 1]);
    }
};

//LAZY PROP
class _prop { // 구간 예외 처리하기
public:
    ll n, sz, h;
    class node{
    public:
        ll v, c;
        node() : node(0, 0){} // identity
        node(ll v, ll c) : v(v), c(c) {}
        operator ll(){ // query
            return v;
        }
    }; vector <node> seg;

    class lazy{
    public:
        ll v;
        lazy() : lazy(0){} // identity
        lazy(ll v) : v(v){}
        bool is_id(){ return !v; }
    }; vector <lazy> lz;

    _prop(){}
    _prop(ll n) : n(n){ 
        sz = 1; while(sz < n + 1) sz <<= 1ll;
        h = __lg(sz);
        seg.assign(2 * sz, node());
        lz.assign(2 * sz, lazy());
    }

    void build(ll idx){
        while(idx > 1){
            idx >>= 1;
            seg[idx] = merge(seg[idx << 1], seg[idx << 1 | 1]);
            if(lz[idx].is_id()) continue;
            prop(seg[idx], lz[idx]);
        }
    }

    void push(ll idx){
        for(int i = h;i > 0;i--){
            ll cur = idx >> i;
            if(lz[cur].is_id()) continue;
            apply(cur << 1, lz[cur]);
            apply(cur << 1 | 1, lz[cur]);
            lz[cur] = lazy();
        }
    }

    void apply(ll idx, lazy& now){
        prop(seg[idx], now);
        if(idx < sz) comp(lz[idx], now); 
    }

    node merge(node& l, node& r){ // node + node
        return{
            l.v + r.v,
            l.c + r.c
        };
    }

    void prop(node& seg, lazy& lz){ // lazy -> node
        seg.v += lz.v * seg.c;
    }

    void comp(lazy& s, lazy& p){ // lazy -> lazy
        s.v += p.v;
    }

    node query(ll idx){ return query(idx, idx); }
    node query(ll st, ll en) {
        st = max(0ll, st); en = min(n, en);
        if(st > en) return node();

        ll l = st + sz, r = en + sz;
        push(l); push(r);

        node nl, nr;
        while(l <= r){
            if(l & 1) nl = merge(nl, seg[l++]);
            if(!(r & 1)) nr = merge(seg[r--], nr);
            l >>= 1; r >>= 1;
        }

        return merge(nl, nr);
    }

    void update(ll idx, lazy lz){ update(idx, idx, lz); }
    void update(ll st, ll en, lazy lz){
        st = max(0ll, st); en = min(n, en);
        if(st > en) return;

        ll l = st + sz, r = en + sz;
        ll tl = l, tr = r;
        push(l); push(r);

        while(l <= r){
            if(l & 1) apply(l++, lz);
            if(!(r & 1)) apply(r--, lz);
            l >>= 1; r >>= 1l;
        }
        
        build(tl); build(tr);
    }

    void set(ll idx, node now){
        if(idx < 0 || idx > n) return;
        idx += sz;
        seg[idx] = now;
    }

    void init(){
        for(int i = sz - 1;i >= 1;i--){
            seg[i] = merge(seg[i << 1], seg[i << 1 | 1]);
        }
    }

    void clear(){ 
        fill(all(seg), node()); 
        fill(all(lz), lazy());
    }
};

template <typename T = ll> //query type
class _hld { // m = 0 정점, m = 1 간선
public:
    vector <vector <ll>> adj;
    vector <ll> sz, top, d, in, out, p;
    ll n, m, cnt = 0;

    _hld() {}
    _hld(ll n, ll m = 0){
        this->n = n; this->m = m;
        adj.resize(n + 1); sz.resize(n + 1);
        in.resize(n + 1); out.resize(n + 1);
        d.resize(n + 1); p.resize(n + 1);
        top.resize(n + 1); 
    }

    void add(ll s, ll e){
        addsol(s, e); addsol(e, s);
    }   
    void addsol(ll s, ll e, ll c = 1){ adj[s].push_back(e); }

    void dfs1(ll cur, ll pre){
        sz[cur] = 1;
        for(auto& nxt : adj[cur]){
            if(nxt == pre) continue;
            d[nxt] = d[cur] + 1; p[nxt] = cur;
            dfs1(nxt, cur); sz[cur] += sz[nxt];
            if(sz[nxt] > sz[adj[cur][0]]) swap(nxt, adj[cur][0]);
        }
    }

    void dfs2(ll cur, ll pre){
        in[cur] = ++cnt;
        for(auto& nxt : adj[cur]){
            if(nxt == pre) continue;
            top[nxt] = (nxt == adj[cur][0] ? top[cur] : nxt);
            dfs2(nxt, cur);
        }
        out[cur] = cnt;
    }

    void init(){
        for(int i = 1;i <= n;i++){
            if(sz[i]) continue;
            dfs1(i, -1); dfs2(i, -1);
        }
    }

    T query(ll st, ll en) { 
        //_seg<>::node ret;
        _prop<>::node ret;
        while(top[st] != top[en]){
            if(d[top[st]] < d[top[en]]) swap(st, en);
            ll cur = top[st];
            ret = seg.merge(ret, seg.query(in[cur], in[st], 0, n));
            st = p[cur];
        }

        if(d[st] > d[en]) swap(st, en);
        ret = seg.merge(ret, seg.query(in[st] + m, in[en], 0, n));
        return ret; 
    }

    //void add_seg(ll idx, ll val) { seg.add(in[idx], val); }
    /*
    void add_lazy(ll st, ll en, ll val){
        while(top[st] != top[en]){
            if(d[top[st]] < d[top[en]]) swap(st, en);
            ll cur = top[st];
            seg.add(in[cur], in[st], val);
            st = p[cur];
        }

        if(d[st] > d[en]) swap(st, en);
        seg.add(in[st] + m, in[en], val);
    }
    */
}; 

class _mt{ // 1-based index 
public:
    vector <ll> arr; ll n;
    vector <vector <ll>> tree;
    _mt(){} 
    _mt(ll n) : n(n), arr(n + 1), tree(4 * n + 1){}

    void set(ll idx, ll val){
        arr[idx] = val;
    }

    void init(){ init(1, n); }
    void init(ll l, ll r, ll node = 1){
        vector <ll>& cur = tree[node];
        if(l == r){
            cur.push_back(arr[l]);
            return;
        }

        ll mid = (l + r) >> 1ll;
        init(l, mid, node * 2);
        init(mid + 1, r, node * 2 + 1);

        vector <ll>& lv = tree[node * 2], &rv = tree[node * 2 + 1];
        cur.resize(lv.size() + rv.size());
        merge(all(lv), all(rv), cur.begin());
    }

    ll greater(ll s, ll e, ll val){ return greater(s, e, 1, n, val); }
    ll greater(ll s, ll e, ll l, ll r, ll val, ll node = 1){
        if(s > e) return 0;
        if(l > e || r < s) return 0;
        if(l >= s && r <= e){
            vector <ll>& cur = tree[node];
            return cur.end() - upper_bound(all(cur), val);
        }

        ll mid = (l + r) >> 1ll;
        ll ret = greater(s, e, l, mid, val, node * 2);
        ret += greater(s, e, mid + 1, r, val, node * 2 + 1);
        return ret;
    }

    ll less(ll s, ll e, ll val){ return less(s, e, 1, n, val); }
    ll less(ll s, ll e, ll l, ll r, ll val, ll node = 1){
        if(s > e) return 0;
        if(l > e || r < s) return 0;
        if(l >= s && r <= e){
            vector <ll>& cur = tree[node];
            return lower_bound(all(cur), val) - cur.begin();
        }

        ll mid = (l + r) >> 1ll;
        ll ret = less(s, e, l, mid, val, node * 2);
        ret += less(s, e, mid + 1, r, val, node * 2 + 1);
        return ret;
    }
};

class _fw{
public:
    ll n; _fw(){}
    _fw(ll n) : n(n){ bit.assign(n + 1, node()); }

    class node{
    public:
        ll v;
        node() : node(0){} // identity
        node(ll v) : v(v){}
        operator ll(){ // query type
            return v;
        }
    }; vector <node> bit;

    node merge(const node& l, const node& r){
        return{
            l.v + r.v
        };
    }

    node inv(const node& a){
        return{
            -a.v
        };
    }

    // range query -> need inv
    node query(ll idx){ return query(idx, idx); }
    node query(ll l, ll r){ 
        if(l > r) return node();
        return merge(inv(cal(l - 1)), cal(r)); 
    }

    // return 1 ~ idx
    node cal(ll idx){
        node ret = node();
        for(int i = idx;i > 0;i -= i & -i) ret = merge(ret, bit[i]);
        return ret;
    }

    void add(ll idx, ll v){
        for(int i = idx;i <= n;i += i & -i) bit[i] = merge(bit[i], v);
    }
};

class _fw2d{
public:
    ll n, m; _fw2d(){}
    _fw2d(ll n, ll m) : n(n), m(m){ 
        bit.assign(n + 1, vector<node>(m + 1, node())); 
    }

    class node{
    public:
        ll v;
        node() : node(0){} // identity
        node(ll v) : v(v){}
        operator ll(){ // query type
            return v;
        }
    }; vector <vector<node>> bit;

    node merge(const node& l, const node& r){
        return{
            l.v + r.v
        };
    }

    node inv(const node& a){
        return{
            -a.v
        };
    }

    // range query -> need inv
    node query(ll x, ll y){ return query(x, y, x, y); }
    node query(ll x1, ll y1, ll x2, ll y2){ 
        if(x1 > x2 || y1 > y2) return node();
        node a = cal(x2, y2), b = cal(x1 - 1, y2);
        node c = cal(x2, y1 -1), d = cal(x1 - 1, y1 - 1);

        node ab = merge(a, inv(b)), cd = merge(inv(c), d);
        return merge(ab, cd); 
    }

    // return (1, 1) ~ (cx, cy)
    node cal(ll cx, ll cy){
        node ret = node();
        for(int i = cx;i > 0;i -= i & -i){
            for(int j = cy;j > 0;j -= j & -j){
                ret = merge(ret, bit[i][j]);
            }
        }

        return ret;
    }

    void add(ll cx, ll cy, ll v){
        for(int i = cx;i <= n;i += i & -i){
            for(int j = cy;j <= m;j += j & -j){
                bit[i][j] = merge(bit[i][j], v);
            }
        }
    }
};

class _pst{
public:
    ll n, q, lg, nc, vc;
    vector <ll> lv, rv, root;
    _pst(){}
    _pst(ll n, ll q) : n(n), q(q){ // q -> update size
        lg = __lg(n + 1) + 2; clear();
    }

    class node{
    public:
        ll v;
        node() : node(0){} // identity
        node(ll v) : v(v) {}
        operator ll() const{ // query
            return v;
        }
    }; vector <node> seg;

    node merge(const node& l, const node& r){
        return{
            l.v + r.v
        };
    }

    ll clone(ll idx){
        ++nc;
        seg[nc] = seg[idx];
        lv[nc] = lv[idx]; rv[nc] = rv[idx];
        return nc;
    }

    void update(ll idx, const node& val){ update(vc, idx, val); }
    void update(ll ver, ll idx, const node& val){
        ++vc;
        root[vc] = update(root[ver], 0, n, idx, val);
    }

    ll update(ll idx, ll s, ll e, ll pos, const node& val){
        ll cur = clone(idx);
        if(s == e){
            seg[cur] = val;
            return cur;
        }

        ll m = (s + e) >> 1ll;
        if(pos <= m) lv[cur] = update(lv[cur], s, m, pos, val);
        else rv[cur] = update(rv[cur], m + 1, e, pos, val);

        seg[cur] = merge(seg[lv[cur]], seg[rv[cur]]);
        return cur;
    }

    node query(ll ver, ll idx){ return query(ver, idx, idx); }
    node query(ll ver, ll l, ll r){
        l = max(0ll, l); r = min(n, r);
        if(l > r) return node();
        return query(root[ver], 0, n, l, r);
    }

    node query(ll idx, ll s, ll e, ll l, ll r){
        if(!idx) return node();
        if(r < s || e < l) return node();
        if(l <= s && r >= e) return seg[idx];

        ll m = (s + e) >> 1ll;
        node ln = query(lv[idx], s, m, l, r);
        node rn = query(rv[idx], m + 1, e, l, r);
        return merge(ln, rn);
    }

    void clear(){
        nc = vc = 0;
        ll mx = (q + 1) * lg + 5;
        lv.assign(mx, 0); rv.assign(mx, 0);
        root.assign(q + 1, 0); seg.assign(mx, node());
    }
};
