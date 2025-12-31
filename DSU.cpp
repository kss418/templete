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

struct uf_policy{
    struct node{
        node(){} // identity
    };
    static void merge(node& a, node& b){}
};

template <class policy = uf_policy>
class _uf{ // 1-based index
private:
    using node = typename policy::node;
    vector<node> arr;
    vector<int> p, sz; int n, cc;
    node id() const{ return node(); }
public:
    _uf(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n; cc = n;
        p.assign(n + 1, -1); sz.assign(n + 1, 1);
        arr.assign(n + 1, id());
    }

    int find(int x){ // O(1)
        if(p[x] == -1) return x;
        return p[x] = find(p[x]);
    }

    bool merge(int a, int b){ // O(1) + policy::merge
        a = find(a); b = find(b);
        if(a == b) return false;
        if(sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; cc--;
        policy::merge(arr[a], arr[b]); arr[b] = id();
        return true;
    }

    int count() const{ return cc; } // O(1)
    node& get(int x){ return arr[find(x)]; } // O(1)
    void set(int x, const node& v){ arr[find(x)] = v; } // O(1)
    int size(int x){ return sz[find(x)]; } // O(1)
    bool root(int x){ return find(x) == x; } // O(1)
    bool same(int a, int b){ return find(a) == find(b); } // O(1)
};

// WEIGHTED UNION FIND
class _wuf { 
public:
    vector <ll> p, d, dis; ll n; 
    _wuf(){}
    _wuf(ll n) { 
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

class _ufset{ // 1-based index 
private:
    vector <int> p, sz, l, r; int n, cc;
public:
    _ufset(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n; cc = n;
        p.assign(n + 2, -1); sz.assign(n + 2, 1);
        l.resize(n + 2); r.resize(n + 2);
        iota(all(l), 0); iota(all(r), 0);
    }

    int find(int num){ // O(1) 
        if(p[num] == -1) return num;
        return p[num] = find(p[num]);
    }

    bool merge(int a, int b){ // O(1)
        a = find(a); b = find(b);
        if (a == b) return false; 
        if (sz[a] < sz[b]) swap(a, b);
        p[b] = a; sz[a] += sz[b]; cc--;
        l[a] = min(l[a], l[b]);
        r[a] = max(r[a], r[b]);
        return true;
    }

    int count() const{ return cc; } // O(1)
    int size(int x){ return sz[find(x)]; } // O(1)
    bool root(int x){ return find(x) == x; } // O(1)
    bool same(int a, int b) { return find(a) == find(b); } // O(1)
    int prv(int x){ return find(l[find(x)] - 1); } // O(1)
    int nxt(int x){ return find(r[find(x)] + 1); } // O(1)
};
