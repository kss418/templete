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
public:
    using node = typename policy::node;
    node op(const node& l, const node& r) const{ return policy::op(l, r); }
    node id() const{ return node(); }
private:
    int n, sz; vector <node> seg;
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
public:
    using node = typename policy::node;
    using lazy = typename policy::lazy;
    node op(const node& l, const node& r) const{ return policy::op(l, r); }
    node id() const{ return node(); }
private:
    vector <node> seg; vector <lazy> lz; int sz, h, n;
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

template <class policy>
class _hld { // 1-based index, m=0 vertex, m=1 edge(child-index)
public:
    using node = typename policy::node;
    policy seg;
private:
    vector <vector <int>> adj, tmp;
    vector <int> sz, top, d, in, out, p;
    int n, m, cnt = 0; bool built = 0;
    void build_tree(int cur, int pre = -1){
        for(auto& nxt : tmp[cur]){
            if(pre == nxt) continue;
            adj[cur].push_back(nxt);
            build_tree(nxt, cur);
        }
        tmp[cur].clear();
    }

    void dfs1(int cur){
        sz[cur] = 1;
        for(auto& nxt : adj[cur]){
            d[nxt] = d[cur] + 1; p[nxt] = cur;
            dfs1(nxt); sz[cur] += sz[nxt];
            if(sz[nxt] > sz[adj[cur][0]]) swap(nxt, adj[cur][0]);
        }
    }

    void dfs2(int cur){
        in[cur] = ++cnt;
        for(auto& nxt : adj[cur]){
            top[nxt] = (nxt == adj[cur][0] ? top[cur] : nxt);
            dfs2(nxt);
        }
        out[cur] = cnt;
    }

    vector<pair<int,int>> get_path(int st, int en){
        vector<pair<int,int>> ret;
        while(top[st] != top[en]){
            if(d[top[st]] < d[top[en]]) swap(st, en);
            int cur = top[st], l = in[cur], r = in[st];
            if(m == 1 && p[cur] == 0) l++; 
            if(l <= r) ret.push_back({l, r});
            st = p[cur];
        }
        if(d[st] > d[en]) swap(st, en);
        int l = in[st] + m, r = in[en];
        if(l <= r) ret.push_back({l, r});
        return ret;
    }
public:
    _hld(int n = 0, int m = 0){ clear(n, m); }
    void clear(int n, int m = 0){
        this->n = n; this->m = m; cnt = 0;
        adj.assign(n + 1, {}); tmp.assign(n + 1, {}); 
        top.assign(n + 1, 0); d.assign(n + 1, 0); 
        in.assign(n + 1, 0); out.assign(n + 1, 0);
        p.assign(n + 1, 0); sz.assign(n + 1, 0);
        seg.clear(n); built = 0;
    }

    void addsol(int s, int e){ tmp[s].push_back(e); }
    void add(int s, int e){ addsol(s, e); addsol(e, s); }
    void init(){
        assert(!built); built = 1;
        for(int i = 1;i <= n;i++){
            if(sz[i]) continue; top[i] = i;
            build_tree(i); dfs1(i); dfs2(i);
        }
        tmp.clear(); tmp.shrink_to_fit();
    }

    node query(int st, int en) { 
        assert(built); node ret = seg.id();
        auto path = get_path(st, en);
        for(auto &[l, r] : path) ret = seg.op(ret, seg.query(l, r));
        return ret;
    }

    node query_sub(int x){ assert(built); return seg.query(in[x] + m, out[x]); }
    void set(int x, const node& v)
        requires requires(policy s, int i, const node& nv){ s.set(i, nv); }
    { assert(built); seg.set(in[x], v); }

    void update(int x, const node& v)
        requires requires(policy s, int i, const node& nv){ s.update(i, nv); }
    { assert(built); seg.update(in[x], v); }

    template<class P = policy>
    void update(int st, int en, const typename P::lazy& lz)
        requires requires(P s, int l, int r, const typename P::lazy& z){ s.update(l, r, z); }
    {
        auto path = get_path(st, en); assert(built);
        for(auto &[l, r] : path) seg.update(l, r, lz);
    }

    template<class P = policy>
    void update_sub(int x, const typename P::lazy& lz)
        requires requires(P s, int l, int r, const typename P::lazy& z){ s.update(l, r, z); }
    { assert(built); seg.update(in[x] + m, out[x], lz); }
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

template <class policy>
concept has_inv = requires(const typename policy::node& a){
    { policy::inv(a) } -> std::same_as<typename policy::node>;
};

struct fw_policy{
    struct node{
        ll v;
        node(ll v) : v(v){}
        node() : node(0){} // identity
    };
    static node op(const node& l, const node& r){
        return {l.v + r.v};
    }
    static node inv(const node& a){
        return {-a.v};
    }
};

template <class policy = fw_policy>
class _fw{ // 0-based index
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
        bit.assign(n + 2, id());
    }

    void build(const vector<node>& arr){ // O(n)
        if(arr.empty()){ clear(0); return; }
        clear((int)arr.size() - 1);
        for(int i = 0;i <= n;i++) bit[i + 1] = arr[i];
        for(int i = 1;i <= n + 1;i++){
            int j = i + (i & -i);
            if(j <= n + 1) bit[j] = op(bit[j], bit[i]);
        }
    }

    // range query -> need inv
    node query(int idx) requires has_inv<policy> { return query(idx, idx); } // O(log n)
    node query(int l, int r) requires has_inv<policy> { // O(log n)
        l = max(l, 0); r = min(r, n);
        if(l > r) return id();
        return op(pre(r), inv(pre(l - 1))); 
    }

    // return 0 ~ idx
    node pre(int idx){ // O(log n)
        node ret = id(); idx = min(idx, n);
        for(int i = idx + 1;i > 0;i -= i & -i) ret = op(ret, bit[i]);
        return ret;
    }

    void set(int idx, const node& v) requires has_inv<policy> { // O(log n)
        if(idx < 0 || idx > n) return;
        node cur = query(idx, idx), d = op(v, inv(cur));         
        update(idx, d);                
    }

    void update(int idx, const node& v){ // O(log n)
        if(idx < 0 || idx > n) return;
        for(int i = idx + 1;i <= n + 1;i += i & -i) bit[i] = op(bit[i], v);
    }
};

template <class policy = fw_policy>
class _fw2d{
private:
    using node = typename policy::node;
    vector<vector<node>> bit; int n, m;
    node op(const node& l, const node& r) const { return policy::op(l, r); }
    node id() const { return node(); }
    node inv(const node& a) const { return policy::inv(a); }
public:
    _fw2d(int n = 0, int m = 0){ clear(n, m); } // O(nm)
    void clear(int n, int m){ // O(nm)
        this->n = n; this->m = m;
        bit.assign(n + 1, vector<node>(m + 1, id()));
    }

    // return (1,1) ~ (x,y)
    node pre(int y, int x){ // O(log n log m)
        if(x <= 0 || y <= 0) return id();
        node ret = id(); x = min(x, m); y = min(y, n);
        for(int i = y; i > 0; i -= i & -i){
            for(int j = x; j > 0; j -= j & -j){
                ret = op(ret, bit[i][j]);
            }
        }
        return ret;
    }

    // range query -> need inv
    node query(int y, int x) requires has_inv<policy> { return query(y, x, y, x); } // O(log n log m)
    node query(int y1, int x1, int y2, int x2) requires has_inv<policy> { // O(log n log m)
        x1 = max(x1, 1); y1 = max(y1, 1);
        x2 = min(x2, m); y2 = min(y2, n);
        if(x1 > x2 || y1 > y2) return id();
        node a = pre(y2, x2), b = pre(y2, x1 - 1);
        node c = pre(y1 - 1, x2), d = pre(y1 - 1, x1 - 1);
        node ab = op(a, inv(b)), cd = op(inv(c), d);
        return op(ab, cd);
    }

    void update(int y, int x, const node& v){ // O(log n log m)
        if(x <= 0 || y <= 0 || x > m || y > n) return;
        for(int i = y; i <= n; i += i & -i){
            for(int j = x; j <= m; j += j & -j){
                bit[i][j] = op(bit[i][j], v);
            }
        }
    }

    void set(int y, int x, const node& v) requires has_inv<policy> { // O(log n log m)
        if(x <= 0 || y <= 0 || x > m || y > n) return;
        node cur = query(y, x), d = op(v, inv(cur));
        update(y, x, d);
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
