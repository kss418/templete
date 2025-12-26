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