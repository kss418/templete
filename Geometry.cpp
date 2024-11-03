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