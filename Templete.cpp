#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
#define ordered_set tree<int, null_type, less<int>, rb_tree_tag,tree_order_statistics_node_update>
#define fastio cin.tie(0), cout.tie(0), ios::sync_with_stdio(0);
#define all(x) (x).begin(), (x).end()
#define x first 
#define y second
using namespace std; typedef long long ll;
using ull = unsigned long long;
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

// 유파
class _uf { 
public:
    ll n; vector <ll> p, size;
    _uf(ll n) { 
        this->n = n;
        p.resize(n + 1); size.resize(n + 1);
        fill(p.begin(), p.end(), -1);
        fill(size.begin(), size.end(), 1);
    }

    ll find(ll num) { 
        if (p[num] == -1) return num;
        return p[num] = find(p[num]);
    }

    void merge(ll a, ll b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (a < b) swap(a, b);
        p[b] = a, size[a] += size[b];
    }

    ll same(ll a, ll b) {
        if (find(a) == find(b)) return 1;
        return 0;
    }
};

//이분 탐색
class _bs { // 0-based index
public:
    ll l, r;
    _bs(ll l, ll r) {
        this->l = l; this->r = r;
    }

    ll f(ll cur) {

    }

    ll ret() {
        ll lo = l, hi = r, mid = -1;
        while (hi > lo) {
            mid = (hi + lo + 1) >> 1ll;
            if (f(mid)) lo = mid;
            else hi = mid - 1;
        }

        return lo;
    }
};

// LIS
template <typename T = ll>
class _lis {
public:
    ll n; vector<T> arr;
    vector<T> result, rec;
    vector<T> num;

    _lis(ll n) {
        this->n = n;
        arr.resize(n + 1); rec.resize(n + 1);
    }

    void add(ll idx, ll n) {
        arr[idx] = n;
    }

    void init() {
        for (int i = 1; i <= n; i++) {
            ll cur = arr[i];
            auto nxt = upper_bound(num.begin(), num.end(), cur) - num.begin();
            if (nxt == num.size()) num.push_back(cur);
            else num[nxt] = cur;
            rec[i] = nxt + 1;
        }

        ll cnt = num.size();
        result.resize(cnt);
        for (int i = n; i >= 1; i--) {
            if (rec[i] != cnt) continue;
            cnt--; result[cnt] = arr[i];
        }
    }

    vector <ll> ret() {
        return result;
    }
};

//LCS
class _lcs {
public:
    string a, b; vector<vector<ll>> dp;
    ll n, m;

    _lcs(string a, string b) {
        this->a = a; this->b = b;
        this->n = a.size(), this->m = b.size();
        dp.resize(n + 1, vector<ll>(m + 1, 0));
    }

    void init() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i] == b[j]) {
                    dp[i + 1][j + 1] = dp[i][j] + 1;
                }
                else {
                    dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
                }
            }
        }
    }

    ll ret() { // lcs 길이 반환
        return dp[n][m];
    }

    string track() { // lcs string 반환
        string ret;
        ll cy = n, cx = m;
        while (cy && cx) {
            if (a[cy - 1] == b[cx - 1]) {
                ret.push_back(a[cy - 1]); cy--; cx--;
            }
            else if (dp[cy - 1][cx] > dp[cy][cx - 1]) cy--;
            else cx--;
        }

        reverse(ret.begin(), ret.end());
        return ret;
    }
};

//MO's
class _mos{
public:
    ll sq; vector <ll> result, arr;
    class query{
    public:
        ll l, r, n, sq;
        bool operator <(query& ot){
            if((l / sq) == (ot.l / sq)) return r < ot.r;
            return (l / sq < ot.l / sq);
        }
    };
    vector <query> q;

    _mos(ll n, ll m){ // 배열 크기, 쿼리 크기
        sq = sqrt(n); arr.resize(n + 1);
        result.resize(m + 1);
    }

    void add(ll l, ll r, ll n){ // 쿼리 추가
        q.push_back({l, r, n, sq});
    }

    void con(ll idx, ll val){ // 배열 삽입
        arr[idx] = val;
    }

    ll push(ll idx){
        return arr[idx];
    }

    ll pop(ll idx){
        return -arr[idx];
    }

    void init(){
        sort(q.begin(), q.end());

        ll s = q[0].l, e = q[0].r, cur = 0;
        for(int i = s;i <= e;i++) cur += push(i);
        result[q[0].n] = cur;

        for(int i = 1;i < m;i++){
            while(q[i].l > s) cur += pop(s++);
            while(q[i].r < e) cur += pop(e--);
            while(q[i].l < s) cur += push(--s);
            while(q[i].r > e) cur += push(++e);
            result[q[i].n] = cur;
        }
    }

    void print(){
        for(int i = 1;i <= m;i++) cout << result[i] << "\n";
    }
};

//CCW
class _ccw{
public:
    _ccw() {}

    ll ret(pll v1, pll v2, pll v3){
        ll num = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y;
        num -= v1.y * v2.x + v2.y * v3.x + v3.y * v1.x;

        if(num > 0) return 1;
        else if(num < 0) return -1;
        return 0;
    }
};

//삼분 탐색
