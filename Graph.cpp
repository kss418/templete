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
struct lca_policy{
    struct node{
        ll v;
        node(ll v = MINF) : v(v){}
    };
    struct cost{
        ll v;
        cost(ll v = 0) : v(v){}
    };
    static constexpr bool use_node = false;
    static node lift(const cost& c) { return node(c.v); }
    static node merge(const node& a, const node& b){ return { max(a.v, b.v) }; }
};

template <class policy = lca_policy>
class _lca { // 1-based index
private:
    using node = typename policy::node;
    using cost = typename policy::cost;
    using edge = pair<int, cost>;
    vector <vector<int>> p; vector <int> d, comp; 
    vector <vector<node>> arr;
    vector <vector<edge>> adj; int n, lg, cc;
    template<bool USE = policy::use_node>
    void shift_both(int& a, int& b, node* v = nullptr){
        for(int i = lg - 1;i >= 0;i--){
            if(p[i][a] == -1) continue;
            if(p[i][a] == p[i][b]) continue;
            if constexpr (USE){
                *v = policy::merge(*v, arr[i][a]);
                *v = policy::merge(*v, arr[i][b]);
            }
            a = p[i][a]; b = p[i][b];
        }
    }

    void build(int cur, int cid) {
        comp[cur] = cid;
        for (auto& [nxt, co] : adj[cur]) {
            if(d[nxt] != -1) continue;
            p[0][nxt] = cur; d[nxt] = d[cur] + 1;
            if constexpr (policy::use_node) arr[0][nxt] = policy::lift(co);
            build(nxt, cid);
        }
    }

    template<bool USE = false>
    int shift(int cur, int diff, node* v = nullptr){ // O(log n)
        if (diff <= 0) return cur;
        for (int i = lg;i >= 0; i--) {
            if(cur == -1) break;
            if(!(diff & (1ll << i))) continue;
            if constexpr (USE) *v = policy::merge(*v, arr[i][cur]);
            cur = p[i][cur];
        }
        return cur;
    }
public:
    _lca(int n = 0){ clear(n); } // O(1)
    void clear(int n){ // O(n)
        this->n = n; lg = 1; cc = 0;
        while ((1ll << lg) <= n) lg++; 
        p.assign(lg + 1, vector<int>(n + 1, -1)); 
        d.assign(n + 1, -1); adj.assign(n + 1, {}); comp.assign(n + 1, 0);
        if constexpr (policy::use_node) arr.assign(lg + 1, vector<node>(n + 1, policy::node()));
    }

    void add(int a, int b, cost co){ addsol(a, b, co); addsol(b, a, co); } // O(1)
    void addsol(int a, int b, cost co){ adj[a].push_back({b, co}); } // O(1)
    void build(){ // O(n log n)
        for(int i = 1;i <= n;i++){
            if(d[i] != -1) continue;
            d[i] = 0; ++cc; build(i, cc);
        }
        for (int i = 1; i <= lg; i++) {
            for (int j = 1; j <= n; j++) {
                if (p[i - 1][j] == -1) continue;
                p[i][j] = p[i - 1][p[i - 1][j]];
                if constexpr (policy::use_node) arr[i][j] = policy::merge(arr[i - 1][j], arr[i - 1][p[i - 1][j]]);
            }
        }
    }

    int component(int v) const { return comp[v]; } // O(1)
    bool same_comp(int a, int b) const { return comp[a] == comp[b]; } // O(1)

    int depth(int v) const { return d[v]; } // O(1)
    int parent(int v) const { return p[0][v]; } // O(1)
    int ancestor(int v, int k){  // O(log n)
        if(k < 0 || k > d[v]) return -1;
        return shift<false>(v, k); 
    } 
   
    int ret(int a, int b) { // O(log n)
        if(!same_comp(a, b)) return -1;
        if(d[a] < d[b]) swap(a, b);
        a = shift<false>(a, d[a] - d[b]);
        if (a == b) return a;
        shift_both<false>(a, b);
        return p[0][a];
    }

    int jump(int a, int b, int k){ // O(log n)
        if(!same_comp(a, b)) return -1;
        int l = ret(a, b), da = d[a] - d[l], db = d[b] - d[l];
        if(k < 0 || k > da + db) return -1;
        if(k <= da) return shift<false>(a, k);
        return shift<false>(b, da + db - k);
    }

    int dist(int a, int b){ // O(log n)
        if(!same_comp(a, b)) return -1;
        return d[a] + d[b] - 2 * d[ret(a, b)];
    }

    node query(int a, int b){ // O(log n)
        static_assert(policy::use_node); node ret = node();
        if(!same_comp(a, b)) return ret;
        if(d[a] < d[b]) swap(a, b);
        a = shift<true>(a, d[a] - d[b], &ret);
        if(a == b) return ret;
        shift_both<true>(a, b, &ret);
        ret = policy::merge(ret, arr[0][a]);
        return policy::merge(ret, arr[0][b]);
    }
};

// 희소 배열
class _st { // 0-base index
private:
    vector <vector<ll>> sum;
    vector <vector<int>> lift; ll m; int n, lg; 
public:
    _st(int n = 0, ll max_step = 0){ clear(n, max_step); }; // O(n log m)
    void clear(int n, ll m = 60){ // O(n log m)
        lg = 1; this->n = n; this->m = m;
        while((1ll << lg) <= m) lg++;
        lift.assign(lg + 1, vector<int>(n + 2, n + 1));
        sum.assign(lg + 1, vector<ll>(n + 2, INF));
    }

    void add(int st, int en, ll co = 1) { lift[0][st] = en; sum[0][st] = co; } // O(1)
    void build(){ // O(n log m)
        for(int i = 1;i <= lg;i++){
            for(int j = 0;j <= n;j++){
                int mid = lift[i - 1][j];
                if(mid == n + 1 || lift[i - 1][mid] == n + 1) continue;
                lift[i][j] = lift[i - 1][mid];
                sum[i][j] = min(sum[i - 1][j] + sum[i - 1][mid], INF);
            }
        }
    }

    int next(int v) const{ return (lift[0][v] == n + 1 ? -1 : lift[0][v]); } // O(1)
    pair<int, ll> ret(int cur, ll k) const{ // O(log m)
        ll co = 0;
        for(int i = lg; i >= 0; i--){
            if(cur == n + 1) break;
            if(k < sum[i][cur]) continue;
            co += sum[i][cur]; k -= sum[i][cur]; cur = lift[i][cur];
        }
        return {(cur == n + 1 ? -1 : cur), co}; // 도착 정점, 사용한 비용
    }
};

// MST
struct mst_policy{
    using cost = ll;
    static cost zero(){ return 0; }
    static cost add(const cost& a, const cost& b){ return a + b; }
};

template <class policy = mst_policy>
class _mst{
private:
    using cost = typename policy::cost;
    cost zero() const{ return policy::zero(); }
    cost add(const cost& a, const cost& b) const{ return policy::add(a, b); }
    cost result = zero(); int n; bool built = 0;
    class edge{
    public:
        int s, e; cost c;
        bool operator<(const edge& ot) const{ return c < ot.c; }
    };
    class _uf{
    private:
        vector <int> p, sz; int n;
    public:
        _uf(int n = 0){ clear(n); } // O(n)
        void clear(int n){ this->n = n; p.assign(n + 1, -1); sz.assign(n + 1, 1); }  // O(n)
        int find(int x){ // O(1)
            if(p[x] == -1) return x;
            return p[x] = find(p[x]);
        }

        bool merge(int a, int b){ // O(1) 
            a = find(a); b = find(b);
            if(a == b) return false;
            if(sz[a] < sz[b]) swap(a, b);
            p[b] = a; sz[a] += sz[b];
            return true;
        }
    }; _uf uf; vector<edge> adj;

    bool cal(int num = 0){ 
        assert(!built); built = 1;
        sort(all(adj)); int cnt = 0;
        for(auto& i : adj){
            auto& [st, en, c] = i;
            if(cnt >= n - num - 1) return 1;
            if(!uf.merge(st, en)) continue;
            result = add(result, c); cnt++;
        }
        return cnt >= n - num - 1;
    }
public:
    _mst(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n; result = zero(); 
        uf.clear(n); built = 0; adj.clear();
    }

    void add(int st, int en, cost c){ // O(1)
        assert(!built);
        adj.push_back({st, en, c}); 
    }

    bool init(int num = 0){ return cal(num);} // O(m log m)
    cost ret(){ // O(1)
        assert(built);
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
private:
    vector <vector<int>> tmp, adj;
    vector <int> cnt; int n; bool built = 0;
public: 
    _3cycle(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n);
        this->n = n; adj.assign(n + 1, {}); built = 0;
        cnt.assign(n + 1, 0); tmp.assign(n + 1, {});
    }

    void addsol(int st, int en){ tmp[st].push_back(en); } // O(1)
    void add(int st, int en){ addsol(st, en); addsol(en, st); } // O(1)

    void init(){ // O(n + m)
        assert(!built); built = 1;
        for(int i = 1;i <= n;i++){
            for(auto& j : tmp[i]){
                if(tmp[i].size() < tmp[j].size()) continue;
                if(tmp[i].size() == tmp[j].size() && i <= j) continue;
                adj[i].push_back(j);
            }
        }
    }

    ll ret(ll mod = 0){ // O(m sqrt m)
        assert(built); ll c = 0;
        for(int i = 1;i <= n;i++){
            for(auto& j : adj[i]) cnt[j]++;
            for(auto& j : adj[i]){
                for(auto& k : adj[j]) if(cnt[k]){
                    c++;
                    if(mod) c %= mod;
                }
            }
            for(auto& j : adj[i]) cnt[j]--;
        }
        return c;
    } 
};

class _4cycle{ // 1-based index
private:
    vector <vector<int>> tmp, adj;
    vector <int> cnt; int n; bool built = 0;
    bool less(int a, int b){
        if(tmp[a].size() < tmp[b].size()) return 1;
        return tmp[a].size() == tmp[b].size() && a <= b;
    }
public:
    _4cycle(int n = 0){ clear(n); } // O(n)
    void clear(int n){ // O(n)
        this->n = n; adj.assign(n + 1, {}); built = 0;
        cnt.assign(n + 1, 0); tmp.assign(n + 1, {});
    }

    void addsol(int st, int en){ tmp[st].push_back(en); } // O(1)
    void add(int st, int en){ addsol(st, en); addsol(en, st); } // O(1)

    void init(){ // O(n + m)
        assert(!built); built = 1;
        for(int i = 1;i <= n;i++){
            for(auto& j : tmp[i]){
                if(tmp[i].size() < tmp[j].size()) continue;
                if(tmp[i].size() == tmp[j].size() && i <= j) continue;
                adj[i].push_back(j);
            }
        }
    }

    ll ret(ll mod = 0){ // O(m sqrt m)
        assert(built); ll c = 0;
        for(int i = 1;i <= n;i++){
            vector <int> use;
            for(auto& j : adj[i]){
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
