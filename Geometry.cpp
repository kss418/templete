#include <bits/stdc++.h>
#define fastio cin.tie(0), cout.tie(0), ios::sync_with_stdio(0);
#define all(x) (x).begin(), (x).end()
#define x first 
#define y second
using namespace std; typedef long long ll;
using ld = long double; using pld = pair<ld, ld>;
using ull = unsigned long long;
using pll = pair<ll, ll>; using tll = tuple<ll, ll, ll>;
ll n, m, k, t; string s;
constexpr ll INF = 0x3f3f3f3f3f3f3f3f;

//CCW
class _ccw{
public:
    _ccw() {}

    ll ret(ll x1, ll y1, ll x2, ll y2, ll x3, ll y3){
        return ret({x1, y1}, {x2, y2}, {x3, y3});
    }
    ll ret(pll v1, pll v2, pll v3){
        ll num = (v2.x - v1.x) * (v3.y - v1.y);
        num -= (v3.x - v1.x) * (v2.y - v1.y);

        if(num > 0) return 1; 
        else if(num < 0) return -1; 
        return 0;
    }
};

//LINE INTERSECTION
class _li{
public:
    _li() {}

    ll ccw(pll v1, pll v2, pll v3){
        ll num = (v2.x - v1.x) * (v3.y - v1.y);
        num -= (v3.x - v1.x) * (v2.y - v1.y);

        if(num > 0) return 1; 
        else if(num < 0) return -1; 
        return 0;
    }

    bool ret(pair<pll, pll> a, pair<pll, pll> b) {
        return ret(a.x, a.y, b.x, b.y);
    }
    bool ret(pll av1, pll av2, pll bv1, pll bv2){
        ll ac = ccw(av1, av2, bv1) * ccw(av1, av2, bv2);
        ll bc = ccw(bv1, bv2, av1) * ccw(bv1, bv2, av2);

        if(!ac && !bc){
            if(av1 > av2) swap(av1, av2);
            if(bv1 > bv2) swap(bv1, bv2);
            return bv1 <= av2 && av1 <= bv2;
        }

        return ac <= 0 && bc <= 0;
    }
};

//CONVEX HULL
class _ch{
public:
    _ch() {}
    vector <ll> num;
    vector <pll> point;
    ll cnt = 0;

    class node{
    public:
        ll x, y, dx, dy, idx;
        node(pll a, ll idx) : node(a.x, a.y, 1, 0, idx) {};
        node(ll x, ll y, ll dx, ll dy, ll idx){
            this->x = x; this->y = y;
            this->dx = dx; this->dy = dy;
            this->idx = idx;
        }

        bool operator <(node& ot){
            if(dy * ot.dx != dx * ot.dy) return dy * ot.dx < dx * ot.dy;
            if(y != ot.y) return y < ot.y;
            return x < ot.x;
        }
        operator pll(){ return { x, y }; }
    };
    vector <node> arr;

    ll ccw(pll v1, pll v2, pll v3){
        ll num = (v2.x - v1.x) * (v3.y - v1.y);
        num -= (v3.x - v1.x) * (v2.y - v1.y);

        if(num > 0) return 1;
        else if(num < 0) return -1;
        return 0;
    }
    
    void add(ll a, ll b) { add({a, b}); }
    void add(pll a){ arr.push_back({a, cnt++}); }

    void init(){
        sort(all(arr));
        for(int i = 1;i < arr.size();i++){
            arr[i].dx = arr[i].x - arr[0].x;
            arr[i].dy = arr[i].y - arr[0].y;
        }
        sort(arr.begin() + 1, arr.end());

        num.push_back(0);
        if(arr.size() == 1) {
            point.push_back(arr[0]); return;
        }
        num.push_back(1);
        ll nxt = 2;

        while(nxt < arr.size()){
            while(num.size() >= 2){
                ll fi = num.back(); num.pop_back();
                ll se = num.back();
                ll chk = ccw(arr[se], arr[fi], arr[nxt]);
                if(chk > 0){ num.push_back(fi); break; }
            }
            num.push_back(nxt++);
        }

        for(auto& i : num) point.push_back(arr[i]);
    }

    vector <pll> ret(){ return point; } // 좌표 반환
};

//MATRIX
class _matrix{ //0-based index
public:
    ll n, m, mod;
    vector <vector<ll>> arr;

    _matrix(){}
    _matrix(ll n, ll m, ll mod = 998244353){
        this->n = n; this->m = m; this->mod = mod;
        arr.resize(n, vector<ll>(m));
    }
    void add(ll n, ll m, ll num){ arr[n][m] += num % mod; }
    void update(ll n, ll m, ll num){ arr[n][m] = num % mod; }

    _matrix operator *(_matrix& ot){
        _matrix ret(n, ot.m, mod);
        for(int i = 0;i < n;i++){
            for(int j = 0;j < ot.m;j++){
                for(int k = 0;k < m;k++){
                    ret.arr[i][j] += (arr[i][k] * ot.arr[k][j]) % mod;
                    ret.arr[i][j] %= mod;
                }
            }
        }
        return ret;
    }

    _matrix operator +(_matrix &ot){
        _matrix ret(n, m, mod);
        for(int i = 0;i < n;i++){
            for(int j = 0;j < m;j++) ret.arr[i][j] = arr[i][j] + ot.arr[i][j];
        }
        return ret;
    }
    
    _matrix pow(_matrix& cur, ll k){ // n * n matrix
        _matrix ret(n, m, mod);
        if(!k){
            for(int i = 0;i < n;i++) ret.arr[i][i] = 1;
            return ret;
        }

        ret = pow(cur, k >> 1);
        ret = ret * ret;
        if(k & 1) ret = ret * cur;
        return ret;
    }

    ll ret(ll n, ll m) { return arr[n][m]; }
    friend ostream& operator<<(ostream& out, _matrix &cur){
        for(int i = 0;i < cur.n;i++){
            for(int j = 0;j < cur.m;j++) out << cur.arr[i][j] << " ";
            out << "\n";
        }
        return out;
    }
};

//AREA
class _area{
public:
    _area() {}

    ll ret(vector <pll>& v){ // div by 2
        ll num = 0;
        for(int i = 0;i < v.size();i++){
            auto [x1, y1] = v[i];
            auto [x2, y2] = v[(i + 1) % v.size()];
            num += x1 * y2 - x2 * y1;
        }

        return abs(num);
    }
};

//LINE
class _line{ // a >= 1e12 예외 처리 하기
public:
    ld a, b;
    _line(){}
    _line(pld v1, pld v2){
        ld dx = v2.x - v1.x;
        ld dy = v2.y - v1.y;
        if(v2.x == v1.x) a = 1e12 + 1;
        else a = dy / dx;

        if(a >= 1e12) b = v1.x;
        else b = -a * v1.x + v1.y;
    }
    _line(ld slope, pld v){ 
        a = slope; 
        b = (a >= 1e12) ? v.x : -a * v.x + v.y; 
    }

    ld integral(ld s, ld e){ return integral(e) - integral(s); }
    ld integral(ld x){ return (a * x * x) / (ld)2 + b * x; }
    ld f(ld x) { return a * x + b; }
    pld intersect(_line& ot){
        if(a == ot.a) return {1e12, 1e12};
        if(a == 1e12 + 1){ return {b, ot.f(b)}; }
        if(ot.a == 1e12 + 1){ return {ot.b, f(ot.b)}; }

        ld rx = (ot.b - b) / (a - ot.a), ry = f(rx);
        return {rx, ry};
    }
};

// 점 다각형 내부 판별
class _inter{ 
public:
    class _li{
    public:
        _li() {}

        ll ccw(pll v1, pll v2, pll v3){
            ll num = (v2.x - v1.x) * (v3.y - v1.y);
            num -= (v3.x - v1.x) * (v2.y - v1.y);

            if(num > 0) return 1; 
            else if(num < 0) return -1; 
            return 0;
        }

        bool ret(pair<pll, pll> a, pair<pll, pll> b) {
            return ret(a.x, a.y, b.x, b.y);
        }
        bool ret(pll av1, pll av2, pll bv1, pll bv2){
            ll ac = ccw(av1, av2, bv1) * ccw(av1, av2, bv2);
            ll bc = ccw(bv1, bv2, av1) * ccw(bv1, bv2, av2);

            if(!ac && !bc){
                if(av1 > av2) swap(av1, av2);
                if(bv1 > bv2) swap(bv1, bv2);
                return bv1 <= av2 && av1 <= bv2;
            }

            return ac <= 0 && bc <= 0;
        }
    }; _li li;

    _inter(){}
    bool ret(vector<pll>& p, pll now){
        ll cnt = 0, flag = 0, sz = p.size();
        if(sz < 3) return 0;
        pll nxt = {1e12, now.y + 1};

        for(int i = 0;i < sz;i++){
            cnt += li.ret({p[i], p[(i + 1) % sz]}, {now, nxt});
        }

        for(int i = 0;i < sz;i++){
            flag += li.ret({p[i], p[(i + 1) % sz]}, {now, now});
        }

        return (cnt % 2 | flag);
    }
}; 

//ROTATING CALIPUS
template <typename T = ll>
class _rc{
public:
    _rc() {}
    using ptl = pair<T, T>;
    vector <T> num;
    vector <ptl> point;
    ll cnt = 0;

    class node{
    public:
        T x, y, dx, dy; ll idx;
        node(ptl a, ll idx) : node(a.x, a.y, 1, 0, idx) {};
        node(T x, T y, T dx, T dy, ll idx){
            this->x = x; this->y = y;
            this->dx = dx; this->dy = dy;
            this->idx = idx;
        }

        bool operator <(node& ot){
            if(dy * ot.dx != dx * ot.dy) return dy * ot.dx < dx * ot.dy;
            if(y != ot.y) return y < ot.y;
            return x < ot.x;
        }
        operator ptl(){ return { x, y }; }
    };
    vector <node> arr;

    ll ccw(ptl v1, ptl v2, ptl v3){
        T num = (v2.x - v1.x) * (v3.y - v1.y);
        num -= (v3.x - v1.x) * (v2.y - v1.y);

        if(num > 0) return 1;
        else if(num < 0) return -1;
        return 0;
    }
    
    void add(T a, T b) { add({a, b}); }
    void add(ptl a){ arr.push_back({a, cnt++}); }

    void init_ch(){
        sort(all(arr));
        for(int i = 1;i < arr.size();i++){
            arr[i].dx = arr[i].x - arr[0].x;
            arr[i].dy = arr[i].y - arr[0].y;
        }
        sort(arr.begin() + 1, arr.end());

        num.push_back(0);
        if(arr.size() == 1) {
            point.push_back(arr[0]); return;
        }
        num.push_back(1);
        ll nxt = 2;

        while(nxt < arr.size()){
            while(num.size() >= 2){
                ll fi = num.back(); num.pop_back();
                ll se = num.back();
                ll chk = ccw(arr[se], arr[fi], arr[nxt]);
                if(chk > 0){ num.push_back(fi); break; }
            }
            num.push_back(nxt++);
        }

        for(auto& i : num) point.push_back(arr[i]);
    }

    T area(ll a, ll b, ll c){ // div by 2
        T num = 0;
        vector <ptl> v = {point[a], point[b], point[c]};
        for(int i = 0;i < v.size();i++){
            auto [x1, y1] = v[i];
            auto [x2, y2] = v[(i + 1) % v.size()];
            num += x1 * y2 - x2 * y1;
        }
        return abs(num);
    }

    T d(ptl a, ptl b) { // chk overflow
        T dx = a.x - b.x, dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    ll mx = 0; pair <ptl, ptl> result;
    void get(ll l, ll r){
        ll sz = point.size();
        for(int i = 0;i <= 1;i++){
            ll nxt = (r + i) % sz;
            ll now = d(point[l], point[nxt]);
            if(mx > now) continue;
            mx = now; result = { point[l], point[nxt] };
        }
    }

    void init_rc(){
        ll sz = point.size();
        if(sz == 2) get(0, 1);
        else{
            ll l = 0, r = 1; ld now = 0;
            while(area(sz - 1, l, r) < area(sz - 1, l, r + 1)) r++;
            while(r < 2 * sz && l < sz){
                get(l, r);
                if(area(l, l + 1, r) < area(l, l + 1, r + 1)) r++;
                else l++;
            }
        }
    }

    void init() { init_ch(); init_rc(); }
    pair <ptl, ptl> ret(){ return result; }
    ld dist(){ return sqrtl(mx); }
};
