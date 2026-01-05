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
private:
    vector <vector<int>> adj;
    vector <int> cnt, pass; int n, m, seq = 1;
    int push(){ assert(seq < n); return seq++; }
    vector <int> tf(const string& s) const{  
        vector <int> ret;
        for(auto& i : s){
            if('a' <= i && i <= 'z') ret.push_back(i - 'a');
            else if('A' <= i && i <= 'Z') ret.push_back(i - 'A');
            else ret.push_back(i - '0');
        }
        return ret;
    }

    int get_node(const vector <int>& v) const{
        int cur = 0;
        for(auto& c : v){
            assert(0 <= c && c < m);
            if (adj[cur].empty() || !adj[cur][c]) return -1;
            cur = adj[cur][c];
        }
        return cur;
    }
public:
    _trie(int n = 0, int m = 0){ clear(n, m); } // O(n)
    void clear(int n, int m){ // O(n)
        this->m = m; this->n = n; seq = 1;
        cnt.assign(n, 0); adj.assign(n, {}); pass.assign(n, 0);
    }

    int size() const { return pass.empty() ? 0 : pass[0]; } // O(1)
    void insert(const string& s){ insert(tf(s)); } // O(|s|)
    void insert(const vector <int>& v){ // O(|v|)
        int cur = 0; pass[cur]++;
        for(auto &c : v){
            assert(0 <= c && c < m);
            if(adj[cur].empty()) adj[cur].resize(m);
            if(!adj[cur][c]) adj[cur][c] = push();
            cur = adj[cur][c]; pass[cur]++;
        }
        cnt[cur]++;
    }

    bool erase(const string& s) { return erase(tf(s)); } // O(|s|)
    bool erase(const vector <int>& v){ // O(|v|)
        int node = get_node(v);
        if(node == -1 || !cnt[node]) return 0; 
        int cur = 0; cnt[node]--; pass[cur]--;
        for(auto &c : v){ cur = adj[cur][c]; pass[cur]--; }
        return 1;
    }

    int count(const string& s) const{ return count(tf(s)); } // O(|s|)
    int count(const vector <int>& v) const{ // O(|v|)
        int node = get_node(v);
        return (node == -1 ? 0 : cnt[node]);
    }

    int prefix_count(const string& prefix) const { return prefix_count(tf(prefix)); } // O(|s|)
    int prefix_count(const vector<int>& v) const { // O(|v|)
        int node = get_node(v);
        return (node == -1 ? 0 : pass[node]);
    }

    int rank(const string& s) const{ return rank(tf(s)); } // O(|s| * m)
    int rank(const vector <int>& v) const{ // O(|v| * m)
        int ret = 0, cur = 0;
        for(auto& c : v){
            assert(0 <= c && c < m);
            ret += cnt[cur];
            if(!adj[cur].empty()){
                for(int x = 0;x < c;x++){
                    int nxt = adj[cur][x];
                    if(nxt) ret += pass[nxt];
                }
                int nxt = adj[cur][c];
                if(!nxt) return ret; cur = nxt;
            }
            else return ret;
        }
        return ret;
    }

    vector <int> kth(int k) const{ // O(max_len * m)
        if(k <= 0 || k > pass[0]) return {};
        vector <int> ret; int cur = 0;
        while(1){
            if(k <= cnt[cur]) break;
            k -= cnt[cur];
            for(int c = 0;c < m;c++){
                int nxt = adj[cur][c];
                if(!nxt) continue;
                if(pass[nxt] >= k){
                    ret.push_back(c);
                    cur = nxt; break;
                }
                else k -= pass[nxt];
            }
        }
        return ret;
    }

    int go(int cur, int c) const{ // O(1)
        if(cur < 0 || cur >= seq) return -1;
        assert(0 <= c && c < m);
        if(adj[cur].empty()) return -1;
        int nxt = adj[cur][c];
        return nxt ? nxt : -1;
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
