#include <bits/stdc++.h>
#define fastio cin.tie(0), cout.tie(0), ios::sync_with_stdio(0);
using namespace std; typedef long long ll;
using ull = unsigned long long;
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
ll n, m, k, t; string s;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

//KMP
class _kmp { // 0-based index
public:
    string s; ll n;
    vector <ll> f;

    _kmp(string& s) {
       this->s = s; this->n = s.size();
    }

    void init(string& m){ // fail 함수 계산
        ll nxt = 0; f.resize(m.size(), 0);
        for(int cur = 1;cur < m.size();cur++){
            while(nxt && m[cur] != m[nxt]) nxt = f[nxt - 1];
            if(m[cur] == m[nxt]) f[cur] = ++nxt;
        }
    }

    vector <ll> ret(string& m){ // index 반환
        init(m); vector <ll> r;
        ll nxt = 0;
        for(int cur = 0;cur < n;cur++){
            while(nxt && s[cur] != m[nxt]) nxt = f[nxt - 1];
            if(s[cur] == m[nxt]) nxt++;
            if(nxt == m.size()){
                r.push_back(cur - nxt + 1);
                nxt = f[nxt - 1];
            }
        }

        return r;
    }
};

//TRIE
class _trie { // 0-based index
public:
    ll n, m, seq = 2;
    vector <vector<ll>> adj;
    vector <ll> chk;

    _trie(ll k, ll n, ll m = 26) { // 문자열 개수, 문자열 길이, 문자 개수
       this->m = m; this->n = n; 
       chk.resize(n * k + 1); adj.resize(n * k + 1);
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

    void erase(string& s){
        ll cur = 1;
        for(auto &st : s){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            if(!adj[cur][st - 'a']) return;
            cur = adj[cur][st - 'a'];
        }
        chk[cur] = 0;
    }

    bool find(string& s){
        ll cur = 1;
        for(auto &st : s){
            if(adj[cur].empty()) adj[cur].resize(m + 1);
            if(!adj[cur][st - 'a']) return 0;
            cur = adj[cur][st - 'a'];
        }
        return chk[cur];
    }
};

//HASH
class _hash { // 0-based index
public:
    // ll mod[3] = { MOD, 998244353, 922337203685471 };
    // ll key[3] = {29, 31, 26};
    ll n, m, mod, key;
    vector <ll> h;

    _hash(ll n, ll m, ll mod, ll key = 2) {
        this->n = n; this->m = m;
        this->mod = mod; this->key = key;
        h.resize(n + 1, 0);
    }

    ll mo(ll n) {
        if (n % mod >= 0) return n % mod;
        else return (n % mod) + mod;
    }

    void init(string& arr) {
        ll pow = 1;
        for (int i = 0; i < m; i++) {
            h[0] = mo(h[0] + arr[m - i - 1] * pow);
            if (i != m - 1) pow = mo(key * pow);
        }

        for (int i = 1; i <= n - m; i++) {
            h[i] = mo(key * mo(h[i - 1] - arr[i - 1] * pow) + arr[i + m - 1]);
        }
    }

    ll ret(ll a) {
        return mo(h[a]);
    }
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

