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

//KMP
class _kmp{ // 0-based index
private:
    vector <ll> pat; vector <int> f;
    void build(){
        int m = (int)pat.size(), nxt = 0; f.assign(m, 0);
        for(int cur = 1;cur < m;cur++){
            while(nxt && pat[cur] != pat[nxt]) nxt = f[nxt - 1];
            if(pat[cur] == pat[nxt]) f[cur] = ++nxt;
        }
    }

    static vector <ll> tf(const string& s){
        vector <ll> ret;
        for(unsigned char c : s) ret.push_back(ll(c));
        return ret;
    }
public:
    _kmp(){}
    _kmp(const string& s){ set(s); }
    _kmp(const vector <ll>& v){ set(v); }
    void clear(){ pat.clear(); f.clear(); }
    void set(const string& s){ pat = tf(s); build(); }
    void set(const vector<ll>& v){ pat = v; build(); }
    
    const vector<int>& fail() const{ return f; }
    vector <int> match(const string& s) const{ return match(tf(s));}
    vector <int> match(const vector <ll>& v) const{
        vector <int> result;
        int n = (int)v.size(), m = (int)pat.size();
        if(!m) return result; int nxt = 0;
        for(int cur = 0;cur < n;cur++){
            while(nxt && v[cur] != pat[nxt]) nxt = f[nxt - 1];
            if(v[cur] == pat[nxt]) nxt++;
            if(nxt == m){
                result.push_back(cur - m + 1);
                nxt = f[nxt - 1];
            }
        }
        return result;
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

//z
class _z { // 0-based index
public:
    vector <ll> result, a; ll n;
    ll l = 0, r = 0;
        
    _z(){}
    _z(string& s) : _z(move(tf(s))){};
    _z(const vector <ll>& a){ 
        this->a = a; this->n = a.size();
        result.resize(n); init(); 
    }
    
    void init(){ // z array 계산
        result[0] = n;
        for(int i = 1;i < n;i++){
            if(i <= r) result[i] = min(r - i + 1, result[i - l]);
            else result[i] = 0;
            while(i + result[i] < n && a[i + result[i]] == a[result[i]]) result[i]++;
            if(i + result[i] - 1 > r){ l = i; r = i + result[i] - 1; }
        }
    }
        
    vector <ll> tf(string& s){
        vector <ll> ret;
        for(auto& i : s) ret.push_back(i);
        return ret;
    }
    
    vector <ll> ret(){ return result; }
};
