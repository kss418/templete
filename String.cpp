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

//KMP
class _kmp { // 0-based index
public:
    vector <ll> f, v; ll n;

    _kmp(){}
    _kmp(string& s) : _kmp(move(tf(s))){};
    _kmp(const vector <ll>& v){ this->v = v; this->n = v.size(); }

    void init(const vector <ll>& m){ // fail 함수 계산
        ll nxt = 0; f.clear(); f.resize(m.size(), 0);
        for(int cur = 1;cur < m.size();cur++){
            while(nxt && m[cur] != m[nxt]) nxt = f[nxt - 1];
            if(m[cur] == m[nxt]) f[cur] = ++nxt;
        }
    }
    
    vector <ll> ret(string& s){ return ret(move(tf(s)));}
    vector <ll> ret(const vector <ll>& m){ // index 반환
        init(m); vector <ll> r;
        ll nxt = 0;
        for(int cur = 0;cur < n;cur++){
            while(nxt && v[cur] != m[nxt]) nxt = f[nxt - 1];
            if(v[cur] == m[nxt]) nxt++;
            if(nxt == m.size()){
                r.push_back(cur - nxt + 1);
                nxt = f[nxt - 1];
            }
        }

        return r;
    }

    vector <ll> tf(string& s){
        vector <ll> ret;
        for(auto& i : s) ret.push_back(i);
        return ret;
    }
};

//TRIE
class _trie { // 0-based index
public:
    ll n, m, seq = 2;
    vector <vector<ll>> adj;
    vector <bool> chk;
    vector <ll> num;

    _trie(){}
    _trie(ll k, ll n, ll m = 26) { // 문자열 개수, 문자열 길이, 문자 개수
       this->m = m; this->n = n; 
       chk.resize(n * k + 1); adj.resize(n * k + 1);
       num.resize(n * k + 1);
    }

    void insert(string& s) { insert(move(tf(s))); }
    void insert(const vector <ll>& a){
        ll cur = 1;
        for(auto &i : a){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            if(!adj[cur][i]) adj[cur][i] = seq++;
            cur = adj[cur][i]; num[cur] = i;
        }
        chk[cur] = 1;
    }

    void erase(string& s) { erase(move(tf(s))); }
    void erase(const vector <ll>& a){
        ll cur = 1;
        for(auto &i : a){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            if(!adj[cur][i]) return;
            cur = adj[cur][i]; num[cur] = -INF;
        }
        chk[cur] = 0;
    }

    bool find(string& s) { return find(move(tf(s))); }
    bool find(const vector <ll>& a){
        ll cur = 1;
        for(auto &i : a){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            if(!adj[cur][i]) return 0;
            cur = adj[cur][i];
        }
        return chk[cur];
    }

    vector <ll> tf(string& s){  
        vector <ll> ret;
        for(auto& i : s) {
            if(i >= 'a') ret.push_back(i - 'a');
            else ret.push_back(i - 'A');
        }
        return ret;
    }
};

//HASH
class _hash { // 0-based index
public:
//  ll mod[3] = { 1000000007, 998244353, 1000000009 };
//  ll key[3] = { 29, 31, 26 };
    ll mod, key, size, h;
    deque <ll> q, pre;

    _hash() {}
    _hash(ll mod, ll key = 2) {
        this->mod = mod; this->key = key;
        this->size = 0; this->h = 0;
    }

    ll pow_mod(ll a, ll b, ll c) {
        if (!b) return 1;
        ll ret = pow_mod(a, b / 2, c) % c;

        if (b % 2) return ((ret * ret) % c * (a % c)) % c;
        return (ret * ret) % c;
    }

    ll prime_inv(ll a, ll b) {
        return pow_mod(a, b - 2, b);
    }

    ll inv(ll a, ll b, ll m) { // (a * b^-1) % m 반환 m == prime
        return (a % m * prime_inv(b, m) % m) % m;
    }

    ll mo(ll n) {
        if (n % mod >= 0) return n % mod;
        else return (n % mod) + mod;
    }

    void push_back(ll c){
        q.push_back(c);
        h *= key; h = mo(h);
        h += c; h = mo(h);
        pre.push_back(h); size++;
    }

    void pop_back(){
        ll c = q.back(); q.pop_back();
        h -= c; h = mo(h);
        h *= inv(1, key, mod); h = mo(h);
        if(!pre.empty()) pre.pop_back(); size--;
    }

    void push_front(ll c){
        q.push_front(c);
        h += pow_mod(key, size, mod) * c;
        h = mo(h); size++;
    }

    void pop_front(){
        ll c = q.front(); q.pop_front();
        h -= pow_mod(key, size - 1, mod) * c;
        h = mo(h); size--;
    }

    ll ret() { return mo(h); }
    ll ret(ll a, ll b){
        if(a > b) swap(a, b);
        if(!a) return ret(b);
        return mo(pre[b] - mo(pre[a - 1] * pow_mod(key, b - a + 1, mod)));
    }
    ll ret(ll a){ return mo(pre[a]); }
};

//AHO-CORASICK
class _ac{
public:
    ll n, m, seq = 2;
        vector <vector<ll>> adj;
        vector <ll> chk, f;

    _ac(ll k, ll n, ll m = 26) { // 문자열 개수, 문자열 길이, 문자 개수
        this->m = m; this->n = n; 
        chk.resize(n * k + 1); adj.resize(n * k + 1);
        f.resize(n * k + 1);
    }

    void insert(string& s){
        ll cur = 1;
        for(auto &st : s){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            if(!adj[cur][st - 'a']) adj[cur][st - 'a'] = seq++;
            cur = adj[cur][st - 'a'];
        }
        chk[cur] = 1;
    }
    
    void init(){
        deque <ll> q;
        q.push_back(1);
        f[1] = 1;
        
        while(!q.empty()){
            ll cur = q.front(); q.pop_front();
            if(adj[cur].empty()) adj[cur].resize(m + 1);

            for(int i = 0;i < m;i++){
                ll nxt = adj[cur][i];
                if(!nxt) continue;

                if(cur == 1) f[nxt] = 1;
                else{
                    ll dest = f[cur];

                    if(adj[dest].empty()) adj[dest].resize(m + 1);
                    while(dest != 1 && !adj[dest][i]) {
                        dest = f[dest];               
                        if(adj[dest].empty()) adj[dest].resize(m + 1);
                    }
                    if(adj[dest][i]) dest = adj[dest][i];
                    f[nxt] = dest;
                }

                if(chk[f[nxt]]) chk[nxt] = 1;
                q.push_back(nxt);
            }
        }
    }

    bool find(string& s){
        ll cur = 1;
        for(auto& st : s){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            while(cur != 1 && !adj[cur][st - 'a']) {
                cur = f[cur];
                if(adj[cur].empty()) adj[cur].resize(m + 1);
            }

            ll nxt = adj[cur][st - 'a'];
            if(nxt) cur = nxt;
            if(chk[cur]) return 1;
        }

        return 0;
    }
};

//SUFFIX-ARRAY
class _sa{
public:
    ll n, d, k = 0; vector <ll> arr;
    vector <ll> sa, pos, tmp, lcp;
    bool cmp(ll a, ll b){
        if(pos[a] != pos[b]) return pos[a] < pos[b];
        a += d; b += d;
        if(a < n && b < n) return pos[a] < pos[b];
        return a > b;
    }  

    void init(string& s){ for(auto& i : s) arr.push_back(i); init(); }
    void init(vector <ll>& a){ this->arr = a; init(); }

    void init(){
        this->n = arr.size(); tmp.resize(n);
        sa.resize(n); pos.resize(n);
        iota(all(sa), 0); lcp.resize(n);
        for(int i = 0;i < n;i++) pos[i] = arr[i];
        get_sa(); get_lcp();
    }

    void get_sa(){
        for(d = 1; ;d <<= 1){
            sort(all(sa), [this](ll a, ll b) { return cmp(a, b); }); tmp[0] = 0;
            for(int i = 0;i < n - 1;i++) {
                tmp[i + 1] = tmp[i] + cmp(sa[i], sa[i + 1]);
            }
            for(int i = 0;i < n;i++) pos[sa[i]] = tmp[i];
            if(tmp[n - 1] == n - 1) break;
        }
    }

    void get_lcp(){
        for(int cur = 0;cur < n;cur++){
            k = max(k - 1, 0ll);
            if(pos[cur] == n - 1) continue;
            ll nxt = sa[pos[cur] + 1];
            while(arr[cur + k] == arr[nxt + k]) k++;
            lcp[pos[cur]] = k;
        }
    }

    _sa(string& s){ init(s); }
    _sa(vector <ll>& a) { init(a); }
};

//MANACHER
class _mnc { // 0-based index
public:
    vector <ll> dp, v; ll n, sum = 0, m = 0;

    _mnc(){}
    _mnc(string& s) : _mnc(move(tf(s))){};
    _mnc(const vector <ll>& v){ init(v); }
    
    void init(const vector <ll>& arr){
        for(auto& i : arr){
            v.push_back(INF);
            v.push_back(i);
        }
        v.push_back(INF); this->n = v.size();
        dp.resize(n + 1, 0); cal();
    }

    void cal(){
        ll r = -1, c = -1;
        for(int i = 0;i < n;i++){
            if(i <= r) dp[i] = min(r - i, dp[2 * c - i]);
            while(i + dp[i] + 1 < n && i - dp[i] - 1 >= 0){
                if(v[i + dp[i] + 1] == v[i - dp[i] - 1]) dp[i]++;
                else break;
            }
            if(i + dp[i] > r) c = i, r = i + dp[i];
            sum += dp[i] / 2 + 1;
            m = max(m, dp[i]);
        }
    }

    ll cnt(){ return sum; } // 팰린드롬 개수
    ll mx(){ return m; } // 가장 긴 팰린드롬 길이

    vector <ll> tf(string& s){
        vector <ll> ret;
        for(auto& i : s) ret.push_back(i);
        return ret;
    }
};