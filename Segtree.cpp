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
template <typename T = ll> // query type
class _seg { // 구간 예외 처리하기
public:
    ll n;
    class node{
    public:
        ll v;
        node() : node(0){}
        node(ll v) : v(v) {}
        
        node operator += (node ot){ // add
            return node();
        }

        operator T(){ // update -> query
            return v;
        }
    };
    vector <node> seg;

    _seg(){}
    _seg(ll n) { this->n = n; seg.resize(4 * n + 1, node()); }

    node merge(node l, node r){
        return{
            l.v + r.v
        };
    }

    T query(ll st, ll en) { return query(st, en, 0, n); }
    node query(ll st, ll en, ll l, ll r, ll node = 1) {
        if (en < l || st > r) return _seg::node();
        if (st <= l && en >= r) return seg[node];
        ll mid = (l + r) >> 1;
        return merge(query(st, en, l, mid, node * 2), query(st, en, mid + 1, r, node * 2 + 1));
    }

    node update(ll idx, ll val) { return update(idx, val, 0, n); }
    node update(ll idx, ll val, ll l, ll r, ll node = 1) {
        if (idx < l || idx > r) return seg[node];
        if (l == r) {
            seg[node] = val;
            return seg[node];
        }
        ll mid = (l + r) >> 1;
        return seg[node] = merge(update(idx, val, l, mid, node * 2), update(idx, val, mid + 1, r, node * 2 + 1));;
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

    void clear(){ seg.clear(); seg.assign(4 * n + 1, node()); }
};

//LAZY PROP
template <typename T = ll> // query type
class _prop { // 구간 예외 처리하기
public:
    class node{
    public:
        ll v, len;
        node() : node(0, 1){}
        node(ll v, ll len) : v(v), len(len){}
        
        node operator += (node ot){ // add
            return node();
        }

        operator T(){ // update -> query
            return v;
        }
    };

    class lazy_type{
    public:
        ll v;
        lazy_type() : lazy_type(0){}
        lazy_type(ll v) : v(v){}
    };

    vector<node> seg; ll n;
    vector<lazy_type> lazy;

    node merge(node l, node r){ 
        return { l.v + r.v, l.len + r.len };
    }

    bool empty(lazy_type lazy){ // prop 시 return 여부 결정
        return !lazy.v;
    }

    void prop(lazy_type& s, lazy_type p){ // lazy -> lazy prop 연산
        s.v += p.v;
    }

    void cal(lazy_type lazy, node& seg){ //lazy로 인한 seg 값 계산
        seg.v += lazy.v * seg.len;
    }

    void erase(lazy_type& lazy){ // lazy값 초기화
        lazy = 0;
    }

    _prop() {}
    _prop(ll n) {
        this->n = n; 
        seg.resize(4 * n + 1, node()); lazy.resize(4 * n + 1, lazy_type());
    }

    void propagate(ll l, ll r, ll node) {
        if (empty(lazy[node])) return;
        if (l != r) {
            prop(lazy[node * 2], lazy[node]);
            prop(lazy[node * 2 + 1], lazy[node]);
        }
        cal(lazy[node], seg[node]);
        erase(lazy[node]);
    }


    void add(ll st, ll en, ll val) { add(st, en, val, 0, n); }
    void add(ll st, ll en, ll val, ll l, ll r, ll node = 1) {
        propagate(l, r, node);

        if (st > r || en < l) return;
        if (l >= st && r <= en) {
            lazy[node].v += val; propagate(l, r, node);
            return;
        }

        ll mid = (l + r) >> 1;
        add(st, en, val, l, mid, node * 2);
        add(st, en, val, mid + 1, r, node * 2 + 1);

        seg[node] = merge(seg[node * 2], seg[node * 2 + 1]);
    }

    T query(ll st, ll en) { return query(st, en, 0, n); }
    node query(ll st, ll en, ll l, ll r, ll node = 1) {
        propagate(l, r, node);

        if (st > r || en < l) return _prop::node();
        if (l >= st && r <= en) return seg[node];

        ll mid = (l + r) >> 1;
        return merge(query(st, en, l, mid, node * 2), query(st, en, mid + 1, r, node * 2 + 1));
    }
};

template <typename T = ll> //query type
class _hld { // 구간 예외 처리하기
public:
    vector <vector <ll>> adj;
    vector <ll> sz, top, d, in, out, p;
    ll n, cnt = 0;
    class node{
    public:
        T v;
        node() : node(0){}
        node(ll v) {
            this->v = v;
        }

        node operator + (node ot){ // update, query
            T now = max(v, ot.v);
            return node(now);
        }

        node operator += (T ot){ // add
            v += ot;
            return node(v);
        }

        operator T(){ // update -> query
            return v;
        }
    };
    vector <node> seg;

    _hld(ll n){
        this->n = n;
        adj.resize(n + 1); sz.resize(n + 1);
        in.resize(n + 1); out.resize(n + 1);
        d.resize(n + 1); p.resize(n + 1);
        top.resize(n + 1); seg.resize(4 * n + 1);
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

    void init(ll cur = 1){
        dfs1(cur, -1); dfs2(cur, -1);
    }

    node query(ll st, ll en, ll l, ll r, ll node = 1) {
        if (en < l || st > r) return _hld::node();
        if (st <= l && en >= r) return seg[node];
        ll mid = (l + r) >> 1;
        return query(st, en, l, mid, node * 2) + query(st, en, mid + 1, r, node * 2 + 1);
    }
    T query(ll st, ll en) { 
        node ret;
        while(top[st] != top[en]){
            if(d[top[st]] < d[top[en]]) swap(st, en);
            ll cur = top[st];
            ret = ret + query(in[cur], in[st], 0, n);
            st = p[cur];
        }

        if(d[st] > d[en]) swap(st, en);
        ret = ret + query(in[st], in[en], 0, n);
        return ret; 
    }

    void update(ll idx, ll val) { update(in[idx], val, 0, n); }
    node update(ll idx, ll val, ll l, ll r, ll node = 1) {
        if (idx < l || idx > r) return seg[node];
        if (l == r) return seg[node] = val;
        ll mid = (l + r) >> 1;

        seg[node] = update(idx, val, l, mid, node * 2) + update(idx, val, mid + 1, r, node * 2 + 1);
        return seg[node];
    }

    void segadd(ll idx, ll val) { segadd(in[idx], val, 0, n); }
    void segadd(ll idx, ll val, ll l, ll r, ll node = 1) {
        if (idx < l || idx > r) return;
        seg[node] += val;
        if (l == r) return;
        ll mid = (l + r) >> 1;

        segadd(idx, val, l, mid, node * 2);
        segadd(idx, val, mid + 1, r, node * 2 + 1);
    }
};