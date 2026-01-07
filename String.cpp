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
template <class T = ll>
class _kmp{ // 0-based index
private:
    int m;
    vector <T> pat; vector <int> f;
    void build(){
        m = (int)pat.size(); int nxt = 0; f.assign(m, 0);
        for(int cur = 1;cur < m;cur++){
            while(nxt && pat[cur] != pat[nxt]) nxt = f[nxt - 1];
            if(pat[cur] == pat[nxt]) f[cur] = ++nxt;
        }
    }

    static vector <T> tf(const string& s){
        vector <T> ret; ret.reserve(s.size());
        for(unsigned char c : s) ret.push_back(T(c));
        return ret;
    }
public:
    _kmp() : m(0){}
    _kmp(const string& s){ set(s); } // O(|s|)
    _kmp(const vector <T>& v){ set(v); } // O(|v|)
    void clear(){ pat.clear(); f.clear(); m = 0; } // O(1)
    void set(const string& s){ pat = tf(s); build(); } // O(|s|)
    void set(const vector<T>& v){ pat = v; build(); } // O(|v|)

    const vector<int>& fail() const{ return f; } // O(1)
    vector <int> match(const string& s) const{ return match(tf(s));} // O(|s| + m)
    vector <int> match(const vector <T>& v) const{ // O(|v| + m)
        vector <int> ret; int n = (int)v.size();
        if(!m) return ret; int state = 0; ret.reserve(n);
        for(int i = 0;i < n;i++){
            state = go(state, v[i]);
            if(state == m) ret.push_back(i - m + 1);
        }
        return ret;
    }

    int go(int state, const T& ch) const{ // amortized O(1)
        if(!m) return 0;
        if(state == m) state = f[m - 1];
        while(state && ch != pat[state]) state = f[state - 1];
        if(ch == pat[state]) state++;
        return state;
    }

    vector<int> border() const{ // O(m)
        vector <int> ret; if(!m) return ret;
        for(int x = f[m - 1];x > 0;x = f[x - 1]) ret.push_back(x);
        return ret;
    }
};

//SUFFIX-ARRAY
template <class T = ll>
class _sa{
private:
    vector <T> arr; vector <int> sa, pos, lcp, cnt, tmp_sa; int n, d; 
    void build(){ build_sa(); build_lcp(); }
    template <class F>
    void counting_sort(int sz, const F& f){
        cnt.assign(sz, 0);
        for(int i = 0;i < n;i++) cnt[f(sa[i])]++;
        for(int i = 1;i < sz;i++) cnt[i] += cnt[i - 1];
        for(int i = n - 1;i >= 0;i--) tmp_sa[--cnt[f(sa[i])]] = sa[i];
        sa.swap(tmp_sa);
    }

    void build_sa(){
        vector <int> tmp(n); vector <pair<T, int>> num; 
        for(int i = 0;i < n;i++) num.push_back({arr[i], i});
        sort(all(num)); pos[num[0].y] = 0;
        tmp_sa.resize(n); int r = 0;
        for(int i = 1; i < n; i++){
            if(num[i].x != num[i - 1].x) r++;
            pos[num[i].y] = r;
        }
        for(d = 1; ;d <<= 1){
            int sz = r + 1;
            counting_sort(sz + 1, [&](int i){ return i + d < n ? pos[i + d] + 1 : 0; });
            counting_sort(sz + 1, [&](int i){ return pos[i] + 1; });
            tmp[sa[0]] = 0; int nr = 0;
            for(int i = 1;i < n;i++){
                int pre = sa[i - 1], cur = sa[i];
                int p1 = pos[pre], c1 = pos[cur];
                int p2 = pre + d < n ? pos[pre + d] : -1;
                int c2 = cur + d < n ? pos[cur + d] : -1;
                if(p1 != c1 || p2 != c2) nr++; tmp[cur] = nr;
            }
            for(int i = 0;i < n;i++) pos[i] = tmp[i]; r = nr;
            if(r == n - 1) break;
        }
    }

    void build_lcp(){
        int k = 0;
        for(int cur = 0;cur < n;cur++){
            k = max(k - 1, 0);
            if(pos[cur] == n - 1) continue;
            int nxt = sa[pos[cur] + 1];
            while(cur + k < n && nxt + k < n && arr[cur + k] == arr[nxt + k]) k++;
            lcp[pos[cur]] = k;
        }
    }
public:
    _sa(){ clear(0); }
    _sa(const string& s){ set(s); }
    _sa(const vector <T>& v) { set(v); }
    void set(const string& s){ clear(s.size()); for(auto& i : s) arr.push_back((T)i); build(); }
    void set(const vector <T>& v){ clear(v.size()); this->arr = v; build(); }

    void clear(int n){
        this->n = n; d = 1; sa.assign(n, 0); arr.clear(); arr.reserve(n);
        iota(all(sa), 0); lcp.assign(n, 0); pos.assign(n, 0);
    }

    const vector<int>& get_sa() const{ return sa; }
    const vector<int>& get_pos() const{ return pos; }
    const vector<int>& get_lcp() const{ return lcp; }
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
template <class T = ll>
class _z { // 0-based index
private:
    vector<T> arr; vector<int> z; int n = 0;
    void build(){
        n = (int)arr.size(); z.assign(n, 0);
        if(!n) return;
        z[0] = n; int l = 0, r = 0;
        for(int i = 1; i < n; i++){
            if(i <= r) z[i] = min(r - i + 1, z[i - l]);
            while(i + z[i] < n && arr[z[i]] == arr[i + z[i]]) z[i]++;
            if(i + z[i] - 1 > r){ l = i; r = i + z[i] - 1; }
        }
    }

    static vector<T> tf(const string& s){
        vector<T> ret; ret.reserve(s.size());
        for(unsigned char c : s) ret.push_back((T)c);
        return ret;
    }
public:
    _z(){}
    _z(const string& s){ set(s); } // O(|s|)
    _z(const vector<T>& v){ set(v); } // O(|v|)
    void clear(){ arr.clear(); z.clear(); n = 0; } // O(1)
    void set(const string& s){ arr = tf(s); build(); } // O(|s|)
    void set(const vector<T>& v){ arr = v; build(); } // O(|v|)
    const vector<int>& result() const{ return z; } // O(1)
};
