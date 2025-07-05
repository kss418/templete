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

// BFS
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

// 다익
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

// LCA
class _lca { // 1-based index
public:
    ll n, m; vector <ll> d;
    vector <vector<ll>> p;
    vector <vector<pll>> adj;

    class node{
    public:
        ll v;
        node() : node(0) {}
        node(ll v) : v(v) {}

        operator ll(){ // query
            return v;
        }
    };
    vector <vector<node>> arr;

    _lca() {}
    _lca(ll n, bool m = 0) { //n 개수
        this->n = n; this->m = m; 
        p.resize(log2(n + 1) + 1, vector<ll>(n + 1, -1));
        d.resize(n + 1); adj.resize(n + 1);
        fill(d.begin(), d.end(), -1);

        if(m) arr.resize(log2(n + 1) + 1, vector<node>(n + 1, node()));
    }

    void add(ll a, ll b, ll c = 1) { // 양방향
        adj[a].push_back({b, c});
        adj[b].push_back({a, c});
    }

    void addsol(ll a, ll b, ll c = 1) { // 단방향
        adj[a].push_back({b, c});
    }

    node merge(const node& a, const node& b){
        return {
            max(a.v, b.v)
        };
    }

    node merge(ll a, const node& b){
        return{
            max(a, b.v)
        };
    }

    void mktree(ll cur) {
        for (auto& [nxt, co] : adj[cur]) {
            if (d[nxt] != -1) continue;
            p[0][nxt] = cur;
            d[nxt] = d[cur] + 1;

            if(m) arr[0][nxt] = co;
            mktree(nxt);
        }
    }

    void init() {
        for(int i = 1;i <= n;i++){
            if(d[i] != -1) continue;
            d[i] = 0; mktree(i);
        }

        for (int i = 1; i <= log2(n); i++) {
            for (int j = 1; j <= n; j++) {
                if (p[i - 1][j] == -1) continue;
                p[i][j] = p[i - 1][p[i - 1][j]];
                if(m) arr[i][j] = merge(arr[i - 1][p[i - 1][j]], arr[i - 1][j]);
            }
        }
    }

    ll ret(ll a, ll b) {
        if (d[a] < d[b]) swap(a, b);

        ll diff = d[a] - d[b];
        if (diff) for (int i = log2(diff); i >= 0; i--) {
            if (diff & (1ll << i)) a = p[i][a];
        }
        if (a == b) return a;

        for (int i = log2(n); i >= 0; i--) {
            if (a == b) break;
            if (p[i][a] == -1) continue;
            if (p[i][a] == p[i][b]) continue;

            a = p[i][a]; b = p[i][b];
        }

        return p[0][a];
    }

    ll num(ll a, ll b){
        if(!m) return MINF;

        node ret = node();
        if (d[a] < d[b]) swap(a, b);

        ll diff = d[a] - d[b];
        if (diff) for (int i = log2(diff); i >= 0; i--) {
            if (diff & (1ll << i)){
                ret = merge(ret, arr[i][a]);
                a = p[i][a];
            }
        }
        if (a == b) return ret;

        for (int i = log2(n); i >= 0; i--) {
            if (a == b) break;
            if (p[i][a] == -1) continue;
            if (p[i][a] == p[i][b]) continue;

            ret = merge(ret, arr[i][a]);
            ret = merge(ret, arr[i][b]);
            a = p[i][a]; b = p[i][b];
        }

        ret = merge(ret, arr[0][a]);
        ret = merge(ret, arr[0][b]);
        return ret;
    }
};

// 플로이드
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

// SPFA
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

// 희소 배열
class _st { // 1-base index
public:
    ll n, m; vector <vector<ll>> num;
    _st(ll n, ll m) { //n 개수 m 깊이
        this->n = n; this->m = m;
        num.resize(log2(m + 1) + 1, vector<ll>(n + 1, 0));
    }

    void add(ll st, ll en) {
        num[0][st] = en;
    }

    void init() {
        for (int i = 1; i <= log2(m + 1); i++) {
            for (int j = 1; j <= n; j++) num[i][j] = num[i - 1][num[i - 1][j]];
        }
    }

    ll ret(ll n, ll d) { //n 노드 d 깊이
        for (int i = log2(m + 1); i >= 0; i--) {
            if (d & (1ll << i)) n = num[i][n];
        }

        return n;
    }
};

// MST
template <typename T = ll>
class _mst {
public:
    vector<vector<pair<T, ll>>> adj;
    vector<ll> p, size; T result = 0;
    ll n, cnt = 0;
    class edge {
    public:
        ll s, e; T c;
        bool operator>(const edge& ot) const {
            return c > ot.c;
        }

        bool operator<(const edge& ot) const {
            return c < ot.c;
        }
    };
    priority_queue<edge, vector<edge>, greater<edge>> pq;

    _mst(ll n) {
        this->n = n;
        adj.resize(n + 1);
        p.resize(n + 1, -1); size.resize(n + 1, 1);
    }

    ll find(ll num) {
        if (p[num] == -1) return num;
        return p[num] = find(p[num]);
    }

    void merge(ll a, ll b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (a > b) swap(a, b);
        p[b] = a, size[a] += size[b];
    }

    ll same(ll a, ll b) {
        if (find(a) == find(b)) return 1;
        return 0;
    }

    void add(ll st, ll en, T c = 1) { // 양방향
        adj[st].push_back({ c, en });
        adj[en].push_back({ c, st });
        pq.push({ st, en, c });
        pq.push({ en, st, c });
    }

    void init(ll num = 0) { // num 만큼 적게 간선 연결
        cnt = 0; result = 0;
        while (!pq.empty()) {
            auto [st, en, c] = pq.top(); pq.pop();
            if (same(st, en)) continue; merge(st, en);
            result += c; cnt++;
            if (cnt == n - 1 - num) break;
        }
    }

    T ret() {
        return result;
    }
};

//SCC
class _scc { // 1-based index
public:
    ll n, dcnt = 0, scnt = 0;
    vector <ll> d, sn, ind; // dfsn, scc번호, ind
    vector<vector<ll>> adj, scc; // 간선, scc
    vector <bool> fin;
    stack <ll> st;
    vector <set <ll>> sccset; // 위상정렬 간선

    _scc(ll n) {
        this->n = n;
        d.resize(n + 1); fin.resize(n + 1);
        adj.resize(n + 1); sn.resize(n + 1);
    }

    void add(ll st, ll en) {
        adj[st].push_back(en);
    }

    ll dfs(ll cur) {
        d[cur] = ++dcnt;
        st.push(cur);

        ll mn = d[cur];
        for (auto& nxt : adj[cur]) {
            if (!d[nxt]) mn = min(mn, dfs(nxt));
            else if (!fin[nxt]) mn = min(mn, d[nxt]);
        }

        if (mn == d[cur]) {
            vector <ll> curscc;
            while (1) {
                ll t = st.top(); st.pop();
                curscc.push_back(t);
                fin[t] = 1; sn[t] = scnt;
                if (t == cur) break;
            }

            sort(curscc.begin(), curscc.end());
            scc.push_back(curscc); scnt++;
        }

        return mn;
    }

    void init() {
        for (int i = 1; i <= n; i++) {
            if (d[i]) continue;
            dfs(i);
        }
    }

    vector <vector <ll>> ret() { // scc 반환
        return scc;
    }

    ll num(ll a) { // scc 번호 반환
        return sn[a];
    }

    vector <ll> top() { //위상정렬
        deque <ll> q; vector <ll> ts;
        sccset.resize(n + 1); ind.resize(n + 1);

        for (auto& i : scc) {
            for (auto& cur : i) {
                for (auto nxt : adj[cur]) {
                    if (num(cur) == num(nxt)) continue;
                    if (sccset[num(cur)].count(num(nxt))) continue;
                    sccset[num(cur)].insert(num(nxt));
                    ind[num(nxt)]++;
                }
            }
        }

        for (int i = 0; i < scnt; i++) {
            if (ind[i]) continue;
            q.push_back(i);
        }

        while (!q.empty()) {
            ll cur = q.front(); q.pop_front();
            ts.push_back(cur);

            for (auto& nxt : sccset[cur]) {

                if (!--ind[nxt]) q.push_back(nxt);
            }
        }

        return ts;
    }
};

//2SAT
class _2sat { // 1-based index
public:
    ll n, dcnt = 0, scnt = 0;
    vector <ll> d, sn, ind; // dfsn, scc번호, ind
    vector<vector<ll>> adj, scc; // 간선, scc
    vector <bool> fin;
    stack <ll> st;
    vector <set <ll>> sccset; // 위상정렬 간선

    _2sat(ll n) {
        this->n = n;
        d.resize(2 * n + 1); fin.resize(2 * n + 1);
        adj.resize(2 * n + 1); sn.resize(2 * n + 1);
    }

    ll no(ll cur) {
        if (cur > n) return cur - n;
        return cur + n;
    }

    void add(ll st, ll en) {
        adj[no(st)].push_back(en);
        adj[no(en)].push_back(st);
    }

    ll dfs(ll cur) {
        d[cur] = ++dcnt;
        st.push(cur);

        ll mn = d[cur];
        for (auto& nxt : adj[cur]) {
            if (!d[nxt]) mn = min(mn, dfs(nxt));
            else if (!fin[nxt]) mn = min(mn, d[nxt]);
        }

        if (mn == d[cur]) {
            vector <ll> curscc;
            while (1) {
                ll t = st.top(); st.pop();
                curscc.push_back(t);
                fin[t] = 1; sn[t] = scnt;
                if (t == cur) break;
            }

            sort(curscc.begin(), curscc.end());
            scc.push_back(curscc); scnt++;
        }

        return mn;
    }

    void init() {
        for (int i = 1; i <= 2 * n; i++) {
            if (d[i]) continue;
            dfs(i);
        }
    }

    vector <vector <ll>> sccret() { // scc 반환
        return scc;
    }

    ll num(ll a) { // scc 번호 반환
        return sn[a];
    }

    vector <ll> top() { //위상정렬
        deque <ll> q; vector <ll> ts;
        sccset.resize(2 * n + 1); ind.resize(2 * n + 1);

        for (auto& i : scc) {
            for (auto& cur : i) {
                for (auto nxt : adj[cur]) {
                    if (num(cur) == num(nxt)) continue;
                    if (sccset[num(cur)].count(num(nxt))) continue;
                    sccset[num(cur)].insert(num(nxt));
                    ind[num(nxt)]++;
                }
            }
        }

        for (int i = 0; i < scnt; i++) {
            if (ind[i]) continue;
            q.push_back(i);
        }

        while (!q.empty()) {
            ll cur = q.front(); q.pop_front();
            ts.push_back(cur);

            for (auto& nxt : sccset[cur]) {

                if (!--ind[nxt]) q.push_back(nxt);
            }
        }

        return ts;
    }

    ll ret() { // 2sat 여부 반환
        ll flag = 1;
        for (int i = 1; i <= n; i++) {
            if (num(i) != num(no(i))) continue;
            flag = 0; break;
        }

        return flag;
    }

    vector <ll> val() { // 참 일때 값 반환
        vector <ll> ret;
        ret.resize(2 * n + 1, -1);
        vector <ll> track = top();
        for (auto& i : track) {
            for (auto& cur : scc[i]) {
                if (ret[cur] != -1) continue;
                ret[cur] = 0; ret[no(cur)] = 1;
            }
        }

        return ret;
    }
};

//FLOW
class _flow {
public:
    ll n;
    vector <ll> d, w;
    deque <ll> q;

    class edge {
    public:
        ll nxt, c, f;
        edge* rev;

        edge(ll nxt, ll c) {
            this->nxt = nxt; this->c = c;
            this->rev = nullptr; this->f = 0;
        }

        void flow(ll num) { f += num; rev->f -= num; }
        ll cap() { return c - f; }
    };
    vector <vector<edge*>> adj;

    _flow(ll n) {
        this->n = n;
        d.resize(n + 1); w.resize(n + 1);
        adj.resize(n + 1);
    }

    void addsol(ll st, ll en, ll c = 1) {
        adj[st].push_back(new edge(en, c));
        adj[en].push_back(new edge(st, 0));
        adj[st].back()->rev = adj[en].back();
        adj[en].back()->rev = adj[st].back();
    }


    bool bfs(ll st, ll en) { // sink 도달 여부 반환
        fill(d.begin(), d.end(), -1);
        d[st] = 0;

        q.push_back(st);
        while (!q.empty()) {
            ll cur = q.front(); q.pop_front();
            for (auto& edge : adj[cur]) {
                ll nxt = edge->nxt;
                if (d[nxt] != -1) continue;
                if (edge->cap() <= 0) continue;

                d[nxt] = d[cur] + 1; q.push_back(nxt);
            }
        }

        return d[en] != -1;
    }

    ll dfs(ll cur, ll en, ll val) { // sink 도달 최대 유량 반환
        if (cur == en) return val;

        while (w[cur] < adj[cur].size()) {
            ll i = w[cur]; w[cur]++;
            edge* edge = adj[cur][i];
            ll nxt = edge->nxt;

            if (d[nxt] != d[cur] + 1) continue;
            if (edge->cap() <= 0) continue;

            ll mn = dfs(nxt, en, min(edge->cap(), val));
            if (mn <= 0) continue;
            edge->flow(mn);

            return mn;
        }

        return 0;
    }

    ll ret(ll st, ll en) {
        ll r = 0;
        while (bfs(st, en)) {
            fill(w.begin(), w.end(), 0);
            while (1) {
                ll f = dfs(st, en, INF);
                if (!f) break;
                r += f;
            }
        }

        return r;
    }
};

//이분 매칭
class _bm { // 1-based index
public:
    ll as, bs; vector<vector<ll>> adj;
    vector<ll> av, bv, v;

    _bm(ll a, ll b) {
        as = a; bs = b;
        av.resize(a + 1, -1); bv.resize(b + 1, -1);
        adj.resize(a + 1); v.resize(a + 1);
    }

    void addsol(ll st, ll en) {
        adj[st].push_back(en);
    }

    bool dfs(ll a) {
        v[a] = 1;
        for (auto& b : adj[a]) {
            if (bv[b] == -1 || !v[bv[b]] && dfs(bv[b])) {
                av[a] = b; bv[b] = a;
                return 1;
            }
        }
        return 0;
    }

    ll ret() {
        memset(&av[0], -1, sizeof(av[0]) * av.size());
        memset(&bv[0], -1, sizeof(bv[0]) * bv.size());
        ll result = 0;

        for (int i = 1; i <= as; i++) {
            if (av[i] != -1) continue;
            memset(&v[0], 0, sizeof(v[0]) * v.size());
            if (dfs(i)) result++;
        }

        return result;
    }

    void clear() {
        for (int i = 1; i <= as; i++) adj[i].clear();
    }
};

//MCMF
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

//위상 정렬
class _tsort {
public:
    ll n; vector<vector<ll>> adj;
    vector<ll> ind, sort;
    _tsort(ll n) {
        this->n = n; adj.resize(n + 1);
        ind.resize(n + 1);
    }

    void addsol(ll st, ll en) {
        adj[st].push_back(en);
        ind[en]++;
    }

    void init() {
        deque <ll> q;
        for (int i = 1; i <= n; i++) {
            if (ind[i]) continue;
            q.push_back(i);
        }

        while (!q.empty()) {
            ll cur = q.front(); q.pop_front();
            sort.push_back(cur);

            for (auto& nxt : adj[cur]) {
                if (!--ind[nxt]) q.push_back(nxt);
            }
        }
    }

    vector <ll> ret() { return sort; }
};

//BCC
class _bcc{ // 1-based index
public:
    vector <vector <ll>> adj;
    vector <ll> mn, seq;
    ll n, cnt;
    stack <pll> st;

    _bcc(ll n) : n(n) {
        adj.resize(n + 1);
    }

    void add(ll s, ll e){
        adj[s].push_back(e);
        adj[e].push_back(s);
    }

    void clear(){
        seq.clear(); mn.clear(); cnt = 0;
        seq.resize(n + 1); mn.resize(n + 1);
    }

    void dfs_point(vector <ll>& arr, ll cur, ll pre = -1){
        bool flag = 0; ll now = 0;
        seq[cur] = mn[cur] = ++cnt;

        for(auto& nxt : adj[cur]){
            if(nxt == pre) continue;
            
            if(seq[nxt]) mn[cur] = min(mn[cur], seq[nxt]);
            else{   
                dfs_point(arr, nxt, cur); now++;
                mn[cur] = min(mn[cur], mn[nxt]);
                if(pre != -1 && mn[nxt] >= seq[cur]) flag = 1, num[cur]++;
            }
        }

        if(pre == -1 && now >= 2) flag = 1, num[cur] = now;
        if(flag) arr.push_back(cur);
    }

    void dfs_line(vector <pll>& arr, ll cur, ll pre = -1){
        seq[cur] = mn[cur] = ++cnt;

        for(auto& nxt : adj[cur]){
            if(nxt == pre) continue;
            
            if(seq[nxt]) mn[cur] = min(mn[cur], seq[nxt]);
            else{   
                dfs_line(arr, nxt, cur);
                mn[cur] = min(mn[cur], mn[nxt]);
                ll l = min(cur, nxt), r = max(cur, nxt);
                if(mn[nxt] > seq[cur]) arr.push_back({l, r});
            }
        }
    }

    void dfs_ret(vector<vector <pll>>& arr, ll cur, ll pre = -1){
        seq[cur] = mn[cur] = ++cnt;

        for(auto& nxt : adj[cur]){
            if(nxt == pre) continue;
            
            if(seq[cur] > seq[nxt]) st.push({cur, nxt});
            if(seq[nxt]) mn[cur] = min(mn[cur], seq[nxt]);
            else{   
                dfs_ret(arr, nxt, cur); 
                mn[cur] = min(mn[cur], mn[nxt]);
                if(mn[nxt] >= seq[cur]){
                    vector <pll> now;
                    while(st.top() != pll(cur, nxt)){
                        now.push_back(st.top());
                        st.pop();
                    }
                    now.push_back({cur, nxt});
                    st.pop();
                    arr.push_back(now);
                }
            }
        }
    }

    // 단절점 반환
    vector <ll> point(){
        vector <ll> ret; clear();
        for(int i = 1;i <= n;i++){
            if(seq[i]) continue;
            dfs_point(ret, i);
        }

        return ret;
    }

    // 단절선 반환
    vector <pll> line(){
        vector <pll> ret; clear();
        for(int i = 1;i <= n;i++){
            if(seq[i]) continue;
            dfs_line(ret, i);
        }

        return ret;
    }

    // bcc 반환, 테스트 해봐야 함
    vector <vector <pll>> ret(){
        vector <vector <pll>> ret; clear();
        for(int i = 1;i <= n;i++){
            if(seq[i]) continue;
            dfs_ret(ret, i);
        }

        return ret;
    }
};