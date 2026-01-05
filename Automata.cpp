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

template <int M>
class _trie { // 0-based index
private:
    vector <array<int, M>> adj;
    vector <int> cnt, pass;
    int new_node(){
        adj.push_back(array<int, M>{}); cnt.push_back(0); pass.push_back(0);
        return (int)adj.size() - 1;
    }
    vector <int> tf(const string& s) const{  
        vector <int> ret;
        for(auto& i : s){
            if('a' <= i && i <= 'z') ret.push_back(i - 'a');
            else if('A' <= i && i <= 'Z') ret.push_back(i - 'A');
            else ret.push_back(i - '0');
        }
        return ret;
    }

    int get_state(const vector <int>& v) const{
        int cur = 0;
        for(auto& c : v){
            assert(0 <= c && c < M);
            if (!adj[cur][c]) return -1;
            cur = adj[cur][c];
        }
        return cur;
    }
public:
    _trie(){ clear(); } // O(1)
    void clear(){ // O(1)
        adj.clear(); cnt.clear(); pass.clear(); new_node();
    }

    void reserve(int max_node){ // O(1)
        adj.reserve(max_node);
        cnt.reserve(max_node);
        pass.reserve(max_node);
    }

    int size() const { return pass.empty() ? 0 : pass[0]; } // O(1)
    void insert(const string& s){ insert(tf(s)); } // O(|s|)
    void insert(const vector <int>& v){ // O(|v|)
        int cur = 0; pass[cur]++;
        for(auto &c : v){
            assert(0 <= c && c < M);
            if(!adj[cur][c]) adj[cur][c] = new_node();
            cur = adj[cur][c]; pass[cur]++;
        }
        cnt[cur]++;
    }

    bool erase(const string& s) { return erase(tf(s)); } // O(|s|)
    bool erase(const vector <int>& v){ // O(|v|)
        int state = get_state(v);
        if(state == -1 || !cnt[state]) return 0; 
        int cur = 0; cnt[state]--; pass[cur]--;
        for(auto &c : v){ cur = adj[cur][c]; pass[cur]--; }
        return 1;
    }

    int count(const string& s) const{ return count(tf(s)); } // O(|s|)
    int count(const vector <int>& v) const{ return count(get_state(v)); } // O(|v|)
    int count(int state) const{ return (state == -1 ? 0 : cnt[state]); } // O(1);

    int prefix_count(const string& prefix) const { return prefix_count(tf(prefix)); } // O(|s|)
    int prefix_count(const vector<int>& v) const { return prefix_count(get_state(v)); } // O(|v|)
    int prefix_count(int state) const{ return (state == -1 ? 0 : pass[state]); } // O(1)

    int rank(const string& s) const{ return rank(tf(s)); } // O(|s| * m)
    int rank(const vector <int>& v) const{ // O(|v| * m)
        int ret = 0, cur = 0;
        for(auto& c : v){
            assert(0 <= c && c < M);
            ret += cnt[cur];
            for(int x = 0;x < c;x++){
                int nxt = adj[cur][x];
                if(nxt) ret += pass[nxt];
            }
            int nxt = adj[cur][c];
            if(!nxt) return ret; cur = nxt;
        }
        return ret;
    }

    vector <int> kth(int k) const{ // O(max_len * m)
        if(k <= 0 || k > pass[0]) return {};
        vector <int> ret; int cur = 0;
        while(1){
            if(k <= cnt[cur]) break;
            k -= cnt[cur];
            for(int c = 0;c < M;c++){
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

    int go(int state, int c) const{ // O(1)
        if(state < 0 || state >= (int)adj.size()) return -1;
        assert(0 <= c && c < M);
        int nxt = adj[state][c];
        return nxt ? nxt : -1;
    }
};

class _xor_trie { // 0-based index
private:
    vector <array<int, 2>> adj;
    vector <int> cnt, pass; int n, m, seq = 1;
    int push(){ assert(seq < n); return seq++; }
    int get_state(u64 x) const{
        int cur = 0;
        for(int i = m;i >= 0;i--){
            bool bit = (x >> i) & 1ull; int nxt = adj[cur][bit];
            if(!nxt) return -1; cur = nxt;
        }
        return cur;
    }

    u64 arg(u64 x, bool is_max) const{
        if(!size()) return 0; int cur = 0; u64 ret = 0;
        for(int i = m;i >= 0;i--){
            bool bit = (x >> i) & 1ull, need = bit ^ is_max;
            int nxt = adj[cur][need];
            if(nxt && pass[nxt] > 0){
                ret |= (u64)(need) << i; cur = nxt;
            }
            else{
                ret |= (u64)(need ^ 1) << i;
                cur = adj[cur][need ^ 1];
            }
        }
        return ret;
    }
public:
    _xor_trie(int items = 0, int max_bit = 0){ clear(items, max_bit); } // O(max_items)
    void clear(int items, int max_bit){ // O(items)
        this->m = max_bit; int sz = items * (m + 1) + 1; this->n = sz; seq = 1;
        cnt.assign(sz, 0); adj.assign(sz, {0, 0}); pass.assign(sz, 0);
    }

    int size() const { return pass.empty() ? 0 : pass[0]; } // O(1)
    void insert(u64 x){ // O(max_bit)
        int cur = 0; pass[cur]++;
        for(int i = m;i >= 0;i--){
            bool bit = (x >> i) & 1ull;
            if(!adj[cur][bit]) adj[cur][bit] = push();
            cur = adj[cur][bit]; pass[cur]++;
        }
        cnt[cur]++;
    }

    bool erase(u64 x){ // O(max_bit)
        int state = get_state(x);
        if(state == -1 || !cnt[state]) return 0; 
        int cur = 0; cnt[state]--; pass[cur]--;
        for(int i = m;i >= 0;i--){
            bool bit = (x >> i) & 1ull;
            cur = adj[cur][bit]; pass[cur]--;
        }
        return 1;
    }

    u64 max(u64 x) const{ return x ^ argmax(x); } // O(max_bit)
    u64 argmax(u64 x) const{ return arg(x, 1); } // O(max_bit)
    u64 min(u64 x) const{ return x ^ argmin(x); } // O(max_bit)
    u64 argmin(u64 x) const{ return arg(x, 0); } // O(max_bit)

    int greater(u64 x, u64 k){ return size() - less(x, k + 1); } // O(max_bit)
    int eqaul(u64 x, u64 k){ return less(x, k + 1) - less(x, k); } // O(max_bit)
    int less(u64 x, u64 k){ // O(max_bit)
        int cur = 0, ret = 0;
        for(int i = m;i >= 0;i--){
            bool kb = (k >> i) & 1ull, xb = (x >> i) & 1ull;
            if(kb){   
                if(adj[cur][xb]) ret += pass[adj[cur][xb]];
                if(!adj[cur][xb ^ 1]) return ret;
            }
            else if(!adj[cur][xb]) return ret;
            cur = adj[cur][xb ^ kb];
        }
        return ret;
    }

    int count(u64 x) const{ return count_state(get_state(x)); } // O(max_bit)
    int count_state(int state) const{ return (state == -1 ? 0 : cnt[state]); } // O(1);
    int sub(u64 x) const{ return sub_state(get_state(x)); } // O(max_bit)
    int sub_state(int state) const{ return (state == -1 ? 0 : pass[state]); } // O(1)

    int go(int state, bool bit) const{ // O(1)
        if(state < 0 || state >= seq) return -1;
        int nxt = adj[state][bit];
        return nxt ? nxt : -1;
    }
};

template <int M>
class _ac{
private:
    vector <array<int, M>> adj;
    vector <ll> cnt; vector<int> f; bool built;
    vector <int> tf(const string& s) const{
        vector <int> ret; ret.reserve(s.size());
        for(auto& i : s){
            if('a' <= i && i <= 'z') ret.push_back(i - 'a');
            else if('A' <= i && i <= 'Z') ret.push_back(i - 'A');
            else ret.push_back(i - '0');
        }
        return ret;
    }

    int new_node(){
        adj.push_back(array<int, M>{}); f.push_back(0); cnt.push_back(0);
        return (int)adj.size() - 1;
    }
public:
    _ac(){ clear(); } // O(1)
    void clear(){ // O(1)
        built = 0; adj.clear(); 
        f.clear(); cnt.clear(); new_node();
    }
    
    void reserve(int max_node){ // O(1)
        adj.reserve(max_node);
        f.reserve(max_node);
        cnt.reserve(max_node);
    }

    int size() const{ return (int)adj.size(); } // O(1)
    void insert(const string& s){ insert(tf(s)); } // O(|s|)
    void insert(const vector <int>& v){ // O(|v|)
        assert(!built); int cur = 0;
        for(auto &c : v){
            assert(0 <= c && c < M);
            if(!adj[cur][c]) adj[cur][c] = new_node();
            cur = adj[cur][c];
        }
        cnt[cur]++;
    }
    
    void build(){ // O(max_node * m)
        assert(!built); built = 1; deque <int> q;
        for(int c = 0;c < M;c++){
            int nxt = adj[0][c];
            if(!nxt) continue;
            f[nxt] = 0; q.push_back(nxt);
        }
        
        while(!q.empty()){
            int cur = q.front(); q.pop_front();
            for(int c = 0;c < M;c++){
                int nxt = adj[cur][c];
                if(!nxt) continue;
                int dest = f[cur];
                while(dest && !adj[dest][c]) dest = f[dest];
                if(adj[dest][c]) dest = adj[dest][c];
                f[nxt] = dest; cnt[nxt] += cnt[f[nxt]]; q.push_back(nxt);
            }
        }
    }

    ll count(const string& s) const{ return count(tf(s)); } // O(|s|)
    ll count(const vector <int>& v) const{ // O(|v|)
        assert(built); int cur = 0; ll ret = 0;
        for(auto& c : v){
            assert(0 <= c && c < M);
            cur = go(cur, c);
            ret += cnt[cur];
        }
        return ret;
    }

    int go(int state, int c) const{ // amortized O(1)
        assert(0 <= c && c < M && built);
        while(state && !adj[state][c]) state = f[state];
        if(adj[state][c]) state = adj[state][c];
        else state = 0;
        return state;
    }
};
