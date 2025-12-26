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

class _bfs { // 0-based index
public:
    ll n, m;
    ll dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
    // ll dx[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
    // ll dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    class node {
    public:
        ll y, x;
        ll num() const{
            return y * 200 + x;
        }
    };
    deque <pair <node, ll>> q; vector <ll> v;
    _bfs(){}
    _bfs(ll n, ll m) { 
        this->n = n; this->m = m; 
        v.resize(n * m + 1, INF);
    }

    bool outrange(node cur){
        auto[cy, cx] = cur;
        if(cy < 0 || cx < 0 || cy >= n || cx >= m) return 1;
        return 0;
    }

    void init(node st) {
        q.push_back({st, 0});
        while (!q.empty()) {
            auto [cur, cc] = q.front(); q.pop_front();
            auto [cy, cx] = cur;
            if (v[cur.num()] <= cc) continue; v[cur.num()] = cc;

            for(int i = 0;i < 4;i++){
                ll nx = cx + dx[i], ny = cy + dy[i];
                node nxt = {ny, nx};
                if (outrange(nxt)) continue;
                q.push_back({nxt, cc + 1});
            }
        }
    }

    ll ret(node cur) { return v[cur.num()]; }
};

class _dij {
public:
    class node{
    public:
        ll d;
        node() : node(0){}
        node(ll d) : d(d){}
        ll num() const{ return d; }
        bool operator<(const node& ot) const{ return num() < ot.num(); }
        bool operator>(const node& ot) const{ return num() > ot.num(); }
        bool operator==(const node& ot) const{ return num() == ot.num(); }
        bool operator<=(const node& ot) const{ return num() <= ot.num(); }
        node operator+(const node& ot) const{
            return {d + ot.d};
        }
        operator ll(){ return d; }
    };
    node mx(){ return {INF}; }
    node mn(){ return {0}; }

    using ptl = pair <node, ll>;
    ll n; vector <node> d;
    vector <ll> pre;
    vector <vector<ptl>> adj;
    priority_queue <ptl, vector<ptl>, greater<ptl>> pq;

    _dij(){}
    _dij(ll n) { this->n = n; adj.resize(n + 1); }

    void add(ll st, ll en, node c) { // 양방향
        adj[st].push_back({ c,en });
        adj[en].push_back({ c,st });
    }
    void addsol(ll st, ll en, node c) { // 단방향
        adj[st].push_back({ c,en });
    }

    void init(ll st) {
        d.clear(); pre.clear();
        d.resize(n + 1, mx()); pre.resize(n + 1, -1); 
        pq.push({ mn(), st });
        d[st] = mn();

        while (!pq.empty()) {
            auto [cn, cur] = pq.top(); pq.pop();
            if(cn > d[cur]) continue;
            
            for (auto& i : adj[cur]) {
                auto [nn, nxt] = i;
                node pl = nn + cn;
        
                if (d[nxt] <= pl) continue;
                d[nxt] = pl; pre[nxt] = cur; 
                pq.push({ pl, nxt });
            }
        }
    }

    node ret(ll n) { return d[n]; }
};

template <typename T = ll> // 1-based index
class _floyd { // ret(i,j) == INF 처리하기
public:
    ll n; vector <vector<T>> d, nxt;
    vector <ll> tr;

    _floyd(ll n) {
        this->n = n;
        d.resize(n + 1, vector<T>(n + 1, INF));
        nxt.resize(n + 1, vector<T>(n + 1, 0));
    }

    void add(ll st, ll en, T c = 1) { //양방향
        d[st][en] = min(d[st][en], c);
        d[en][st] = min(d[en][st], c);
        nxt[st][en] = en; nxt[en][st] = st;
    }

    void addsol(ll st, ll en, T c = 1) { //단방향
        d[st][en] = min(d[st][en], c);
        nxt[st][en] = en;
    }

    void init() {
        for (int k = 1; k <= n; k++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    if (d[i][j] <= d[i][k] + d[k][j]) continue;
                    d[i][j] = d[i][k] + d[k][j];
                    nxt[i][j] = nxt[i][k];
                }
            }
        }
    }

    T ret(ll st, ll en) {
        return d[st][en];
    }

    vector <ll> track(ll st, ll en) {
        ll cur = st; tr.clear();

        while (cur != en) {
            tr.push_back(cur);
            cur = nxt[cur][en];
        }
        tr.push_back(en);

        return tr;
    }
};

template <typename T = ll>
class _spfa {
public:
    using ptl = pair <T, ll>;
    ll n; vector <ll> pre, cnt;
    vector <T> d; vector <bool> in;
    deque <ll> q;
    vector <vector <ptl>> adj;

    _spfa(ll n) {
        this->n = n;
        pre.resize(n + 1, -1); d.resize(n + 1, INF);
        in.resize(n + 1); adj.resize(n + 1);
        cnt.resize(n + 1);
    }

    void addsol(ll st, ll en, T c = 1) { // 단방향
        adj[st].push_back({ c, en });
    }

    ll init(ll st, ll fi = 0) { // cycle 0 else 1 / fi = d[st]
        d[st] = fi; in[st] = 1;
        q.push_back(st);

        while (!q.empty()) {
            ll cur = q.front(); q.pop_front();
            in[cur] = 0; cnt[cur]++;
            ll cd = d[cur];
            if (cnt[cur] > n) return 0;

            for (auto& nn : adj[cur]) {
                auto [nd, nxt] = nn;
                if (d[nxt] <= nd + cd) continue;
                d[nxt] = nd + cd;
                pre[nxt] = cur;

                if (in[nxt]) continue;
                in[nxt] = 1;
                q.push_back(nxt);
            }
        }

        return 1;
    }

    T ret(ll num) { // 거리 반환
        return d[num];
    }

    vector <ll> track(ll st, ll en) { // st -> en 경로 반환
        vector <ll> ret;
        ll cur = en;
        while (cur != -1) {
            ret.push_back(cur);
            cur = pre[cur];
        }
        reverse(ret.begin(), ret.end());

        return ret;
    }
};