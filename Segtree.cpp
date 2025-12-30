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

struct seg_policy{
    struct node {
        ll v;
        node(ll v) : v(v){}
        node() : node(0){} // identity
    };

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
    node id() const{ return node(); }
public:
    _seg(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n;
        sz = 1; while(sz < n + 1) sz <<= 1;
        seg.assign(2 * sz, id());
    }

    void build(const vector<node>& arr){ // O(n)
        if(arr.empty()){ clear(0); return; }
        clear((int)arr.size() - 1);
        for(int i = 0;i < (int)arr.size();i++) seg[i + sz] = arr[i];
        for(int i = sz - 1;i >= 1;i--) seg[i] = op(seg[i << 1], seg[i << 1 | 1]);
    }

    node query(int st, int en){ // O(log n)
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

    void set(int idx, const node& v){ // O(log n)
        if(idx < 0 || idx > n) return;
        int p = idx + sz; seg[p] = v;
        for(p >>= 1; p; p >>= 1) seg[p] = op(seg[p << 1], seg[p << 1 | 1]);
    }

    void update(int idx, const node& v){ // O(log n)
        if(idx < 0 || idx > n) return;
        int p = idx + sz; seg[p] = op(seg[p], v);
        for(p >>= 1; p; p >>= 1) seg[p] = op(seg[p << 1], seg[p << 1 | 1]);
    }
};

struct prop_policy{
    struct node {
        ll v; int sz;
        node(ll v, int sz) : v(v), sz(sz){}
        node() : node(0, 0){} // identity
    };
    struct lazy{
        ll v;
        lazy(ll v) : v(v){}
        lazy() : lazy(0){} // identity
    };

    static node op(const node& l, const node& r){
        return node{
            l.v + r.v,
            l.sz + r.sz
        };
    }
    static bool is_lz_id(const lazy& now){ return now.v == 0; }
    static void apply(node& seg, const lazy& lz){ seg.v += lz.v * seg.sz; }
    static void compose(lazy& s, const lazy& p){ s.v += p.v; }
};

template <class policy = prop_policy>
class _prop { 
private:
    using node = typename policy::node;
    using lazy = typename policy::lazy;
    vector <node> seg; vector <lazy> lz; int sz, h, n;
    node op(const node& l, const node& r) const{ return policy::op(l, r); }
    node id() const{ return node(); }
    lazy lz_id() const{ return lazy(); }
    bool is_lz_id(const lazy& now) const{ return policy::is_lz_id(now); }
    void prop(node& seg, const lazy& lz) const{ policy::apply(seg, lz); }
    void comp(lazy& s, const lazy& p) const{ policy::compose(s, p); }

    void push(int idx){
        for(int i = h;i > 0;i--){
            int cur = idx >> i;
            if(is_lz_id(lz[cur])) continue;
            apply(cur << 1, lz[cur]);
            apply(cur << 1 | 1, lz[cur]);
            lz[cur] = lz_id();
        }
    }

    void apply(int idx, const lazy& now){
        prop(seg[idx], now);
        if(idx < sz) comp(lz[idx], now); 
    }

    void init(int idx){
        while(idx > 1){
            idx >>= 1;
            seg[idx] = op(seg[idx << 1], seg[idx << 1 | 1]);
            if(is_lz_id(lz[idx])) continue;
            prop(seg[idx], lz[idx]);
        }
    }
public:
    _prop(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n;
        sz = 1; while(sz < n + 1) sz <<= 1;
        h = __lg(sz);
        seg.assign(2 * sz, id());
        lz.assign(2 * sz, lz_id());
    }

    void build(const vector<node>& arr){ // O(n)
        if(arr.empty()){ clear(0); return; }
        clear((int)arr.size() - 1);
        for(int i = 0;i < (int)arr.size();i++) seg[i + sz] = arr[i];
        for(int i = sz - 1;i >= 1;i--) seg[i] = op(seg[i << 1], seg[i << 1 | 1]);
    }

    node query(int idx){ return query(idx, idx); } // O(log n)
    node query(int st, int en) { // O(log n)
        st = max(0, st); en = min(n, en);
        if(st > en) return id();

        int l = (int)st + sz, r = (int)en + sz;
        push(l); push(r);

        node nl = id(), nr = id();
        while(l <= r){
            if(l & 1) nl = op(nl, seg[l++]);
            if(!(r & 1)) nr = op(seg[r--], nr);
            l >>= 1; r >>= 1;
        }

        return op(nl, nr);
    }

    void update(int idx, const lazy& lz){ update(idx, idx, lz); } // O(log n)
    void update(int st, int en, const lazy& lz){ // O(log n)
        st = max(0, st); en = min(n, en);
        if(st > en) return;

        int l = (int)st + sz, r = (int)en + sz;
        int tl = l, tr = r;
        push(l); push(r);

        while(l <= r){
            if(l & 1) apply(l++, lz);
            if(!(r & 1)) apply(r--, lz);
            l >>= 1; r >>= 1;
        }
        
        init(tl); init(tr);
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

struct fw_policy{
    struct node{
        ll v;
        node(ll v) : v(v){}
        node() : node(0){} // identity
    };
    static node op(const node& l, const node& r){
        return node(l.v + r.v);
    }
    static node inv(const node& a){
        return node(-a.v);
    }
};

template <class policy = fw_policy>
class _fw{ // 1-based index
private:
    using node = typename policy::node;
    vector <node> bit; int n;
    node op(const node& l, const node& r) const{ return policy::op(l, r); }
    node id() const{ return node(); }
    node inv(const node& a) const{ return policy::inv(a); }
public:
    _fw(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n;
        bit.assign(n + 1, id());
    }

    void build(const vector<node>& arr){ // O(n)
        if(arr.empty()){ clear(0); return; }
        clear((int)arr.size() - 1);
        for(int i = 1;i <= n;i++) bit[i] = arr[i];
        for(int i = 1;i <= n;i++){
            int j = i + (i & -i);
            if(j <= n) bit[j] = op(bit[j], bit[i]);
        }
    }

    // range query -> need inv
    node query(int idx){ return query(idx, idx); } // O(log n)
    node query(int l, int r){ // O(log n)
        l = max(l, 1); r = min(r, n);
        if(l > r) return id();
        return op(pre(r), inv(pre(l - 1))); 
    }

    // return 1 ~ idx
    node pre(int idx){ // O(log n)
        node ret = id(); idx = min(idx, n);
        for(int i = idx;i > 0;i -= i & -i) ret = op(ret, bit[i]);
        return ret;
    }

    void set(int idx, const node& v){ // O(log n)
        if(idx <= 0 || idx > n) return;
        node cur = query(idx, idx), d = op(v, inv(cur));         
        update(idx, d);                
    }

    void update(int idx, const node& v){ // O(log n)
        if(idx <= 0 || idx > n) return;
        for(int i = idx;i <= n;i += i & -i) bit[i] = op(bit[i], v);
    }
};

template <class policy = fw_policy>
class _fw2d{
public:
    using node = typename policy::node;
    int n, m; vector <vector<node>> bit;
    node op(const node& l, const node& r) const{ return policy::op(l, r); }
    node id() const{ return node(); }
    node inv(const node& a) const{ return policy::inv(a); }
    _fw2d() : n(0), m(0){}
    _fw2d(int n, int m){ clear(n, m); }
    void clear(int n, int m){
        this->n = n; this->m = m;
        bit.assign(n + 1, vector<node>(m + 1, id()));
    }
    void build(const vector<vector<node>>& arr){ // O(nm log^2)
        if(arr.empty() || arr[0].empty()){ clear(0, 0); return; }
        clear((int)arr.size() - 1, (int)arr[0].size() - 1);
        for(int i = 1;i <= n;i++){
            for(int j = 1;j <= m;j++){
                update(i, j, arr[i][j]);
            }
        }
    }

    // range query -> need inv
    node query(int x, int y){ return query(x, y, x, y); }
    node query(int x1, int y1, int x2, int y2){ 
        if(x1 > x2 || y1 > y2) return id();
        node a = pre(x2, y2), b = pre(x1 - 1, y2);
        node c = pre(x2, y1 -1), d = pre(x1 - 1, y1 - 1);

        node ab = op(a, inv(b)), cd = op(inv(c), d);
        return op(ab, cd); 
    }

    // return (1, 1) ~ (cx, cy)
    node pre(int cx, int cy){
        node ret = id();
        for(int i = cx;i > 0;i -= i & -i){
            for(int j = cy;j > 0;j -= j & -j){
                ret = op(ret, bit[i][j]);
            }
        }

        return ret;
    }

    void update(int cx, int cy, const node& v){
        for(int i = cx;i <= n;i += i & -i){
            for(int j = cy;j <= m;j += j & -j){
                bit[i][j] = op(bit[i][j], v);
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
