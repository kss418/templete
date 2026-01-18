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

class _flow {
private:
    vector <int> d, w; int n;
    class edge {
    public:
        int nxt, rev; ll c;
        edge() = default;
        edge(int nxt, int rev, ll c) : nxt(nxt), rev(rev), c(c) {}
    };
    vector <vector<edge>> adj;
    bool bfs(int st, int en) { 
        memset(d.data(), -1, sizeof(int) * (n + 1));
        queue <int> q; q.push(st); d[st] = 0;
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            for (auto& e : adj[cur]) {
                if(e.c <= 0) continue;
                int nxt = e.nxt;
                if (d[nxt] != -1) continue;
                d[nxt] = d[cur] + 1; q.push(nxt);
            }
        }
        return d[en] != -1;
    }

    ll dfs(int cur, int en, ll val) { 
        if (cur == en) return val;
        int &i = w[cur], sz = (int)adj[cur].size();
        for(;i < sz;++i){
            edge &e = adj[cur][i];
            if(e.c <= 0) continue;
            int nxt = e.nxt;
            if(d[nxt] != d[cur] + 1) continue;
            ll mn = dfs(nxt, en, min(val, e.c));
            if(mn <= 0) continue;
            e.c -= mn; adj[nxt][e.rev].c += mn;
            return mn;
        }
        return 0;
    }
public:
    _flow(int n = 0){ clear(n); }
    void clear(int n){
        this->n = n; adj.assign(n + 1, {});
        d.assign(n + 1, -1); w.assign(n + 1, 0);
    }

    void addsol(int st, int en, ll c = 1) {
        int re = (int)adj[en].size();
        int rs = (int)adj[st].size();
        adj[st].push_back({en, re, c});
        adj[en].push_back({st, rs, 0});
    }

    ll ret(int st, int en) {
        ll r = 0;
        while(bfs(st, en)){
            memset(w.data(), 0, sizeof(int) * (n + 1));
            while(1){
                ll f = dfs(st, en, INF);
                if(!f) break; r += f;
            }
        }
        return r;
    }
};

class _mcmf {
public:
    ll n;
    vector <ll> w, in, dist, v;
    deque <ll> q;

    class edge {
    public:
        ll nxt, c, cost, f;
        edge* rev;

        edge(ll nxt, ll cap, ll cost) {
            this->nxt = nxt; this->c = cap; this->cost = cost;
            this->rev = nullptr; this->f = 0;
        }

        void flow(ll num) { f += num; rev->f -= num; }
        ll cap() { return c - f; }
    };
    vector <vector<edge*>> adj;

    _mcmf(ll n) {
        this->n = n; adj.resize(n + 1);
        w.resize(n + 1); dist.resize(n + 1);
        v.resize(n + 1); in.resize(n + 1);
    }

    void addsol(ll st, ll en, ll c, ll cost) {
        adj[st].push_back(new edge(en, c, cost));
        adj[en].push_back(new edge(st, 0, -cost));
        adj[st].back()->rev = adj[en].back();
        adj[en].back()->rev = adj[st].back();
    }


    bool bfs(ll st, ll en) { // sink 도달 여부 반환
        memset(&dist[0], 0x3f, sizeof(dist[0]) * dist.size());
        memset(&in[0], 0, sizeof(in[0]) * in.size());
        dist[st] = 0; in[st] = 1;

        q.push_back(st);
        while (!q.empty()) {
            ll cur = q.front(); q.pop_front(); in[cur] = 0;
            for (auto& edge : adj[cur]) {
                ll nxt = edge->nxt;
                if (!edge->cap()) continue;
                if (dist[nxt] <= dist[cur] + edge->cost) continue;

                dist[nxt] = dist[cur] + edge->cost;
                if (!in[nxt]) { q.push_back(nxt); in[nxt] = 1; }
            }
        }

        return dist[en] < INF;
    }

    ll dfs(ll cur, ll en, ll val) { // sink 도달 최대 유량 반환
        v[cur] = 1; if (cur == en) return val;

        while (w[cur] < adj[cur].size()) {
            ll i = w[cur]; w[cur]++;
            edge* edge = adj[cur][i];
            ll nxt = edge->nxt;

            if (!edge->cap() || v[nxt]) continue;
            if (dist[nxt] != dist[cur] + edge->cost) continue;

            ll mn = dfs(nxt, en, min(edge->cap(), val));
            if (mn <= 0) continue; edge->flow(mn);

            return mn;
        }

        return 0;
    }

    pll ret(ll st, ll en) { // cost, flow 반환
        ll flow = 0, cost = 0;
        while (bfs(st, en)) {
            memset(&v[0], 0, sizeof(v[0]) * v.size());
            memset(&w[0], 0, sizeof(w[0]) * w.size());
            while (1) {
                ll f = dfs(st, en, INF);
                if (!f) break;
                flow += f; cost += dist[en] * f;
                memset(&v[0], 0, sizeof(v[0]) * v.size());
            }
        }

        return { flow, cost };
    }
};
