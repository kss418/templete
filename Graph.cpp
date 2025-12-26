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

    ll shift(ll a, ll diff){
        if(diff <= 0) return a;
        for (int i = log2(diff); i >= 0; i--) {
            if (diff & (1ll << i)) a = p[i][a];
        }

        return a;
    }

    ll ret(ll a, ll b) {
        if (d[a] < d[b]) swap(a, b);

        a = shift(a, d[a] - d[b]);
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

// 희소 배열
class _st { // 1-base index
public:
    ll n, m; vector <vector<ll>> lift, sum;
    _st(){};
    _st(ll n, ll m) : n(n), m(m) { //n 개수 m 깊이
        lift.resize(log2(m + 1) + 1, vector<ll>(n + 1, 0));
        sum.resize(log2(m + 1) + 1, vector<ll>(n + 1, 0));
    }

    void add(ll st, ll en, ll co = 1) {
        lift[0][st] = en;
        sum[0][st] = co;
    }

    void init() {
        for (int i = 1; i <= log2(m + 1); i++) {
            for (int j = 1; j <= n;j++){
                ll mid = lift[i - 1][j];
                lift[i][j] = lift[i - 1][mid];
                sum[i][j] = min(sum[i - 1][j] + sum[i - 1][mid], INF);
            }
        }
    }

    pll ret(ll n, ll d) { //n 노드 d 깊이
        ll co = 0;
        for (int i = log2(m + 1); i >= 0; i--) {
            if (d < sum[i][n]) continue;
            co += sum[i][n];
            d -= sum[i][n];
            n = lift[i][n];
        }

        return {n, co}; // 정점, 연산 횟수
    }
};

// MST
template <typename T = ll>
class _mst {
public:
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
    vector <edge> adj;

    _mst(ll n) {
        this->n = n;
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
        adj.push_back({ st, en, c });
    }

    void init(ll num = 0) { // num 만큼 적게 간선 연결
        cnt = 0; result = 0;
        sort(all(adj));
        for(auto& i : adj) {
            auto [st, en, c] = i;
            if (cnt >= n - 1 - num) break;
            if (same(st, en)) continue; merge(st, en);
            result += c; cnt++;
        }

        if(cnt < n - 1 - num) result = MINF;
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
    vector <ll> mn, seq, num;
    ll n, cnt;
    stack <pll> st;

    _bcc(ll n) : n(n) {
        adj.resize(n + 1); mn.resize(n + 1);
        seq.resize(n + 1); num.resize(n + 1);
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

class _3cycle{ // 1-based index
public:
    vector <vector<ll>> tmp, adj;
    vector <ll> cnt;
    ll n;

    _3cycle(){}
    _3cycle(ll n) : n(n){
        tmp.resize(n + 1); adj.resize(n + 1);
        cnt.resize(n + 1);
    }

    void addsol(ll st, ll en){
        tmp[st].push_back(en);
    }

    void add(ll st, ll en){
        addsol(st, en);
        addsol(en, st);
    }

    void init(){
        for(int i = 1;i <= n;i++){
            for(auto& j : tmp[i]){
                if(tmp[i].size() < tmp[j].size()) continue;
                if(tmp[i].size() == tmp[j].size() && i <= j) continue;
                adj[i].push_back(j);
            }
        }
    }

    ll ret(){ // Counting 3-Cycle
        ll c = 0; init();
        for(int i = 1;i <= n;i++){
            for(auto& j : adj[i]) cnt[j]++;
            for(auto& j : adj[i]){
                for(auto& k : adj[j]) if(cnt[k]) c++;
            }
            for(auto& j : adj[i]) cnt[j]--;
        }

        return c;
    } 
};

class _4cycle{ // 1-based index
public:
    vector <vector<ll>> adj;
    vector <ll> cnt;
    ll n;

    _4cycle(){}
    _4cycle(ll n) : n(n){
        adj.resize(n + 1); cnt.resize(n + 1);
    }

    void addsol(ll st, ll en){
        adj[st].push_back(en);
    }

    void add(ll st, ll en){
        addsol(st, en);
        addsol(en, st);
    }

    bool less(ll a, ll b){
        if(adj[a].size() < adj[b].size()) return 1;
        return adj[a].size() == adj[b].size() && a <= b;
    }

    ll ret(ll mod = 0){ // Counting 4-Cycle
        ll c = 0;
        for(int i = 1;i <= n;i++){
            vector <ll> use;
            for(auto& j : adj[i]){
                if(less(i, j)) continue;
                for(auto& k : adj[j]){
                    if(less(i, k)) continue;
                    if(!cnt[k]) use.push_back(k);
                    c += cnt[k]++;
                    if(mod) c %= mod;
                }
            }

            for(auto& j : use) cnt[j] = 0;
        }

        return c;
    } 
};