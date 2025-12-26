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

template <typename T = ll>
class _fft{
public:
    const ld PI = acos(-1);
    using cpd = complex <double>;

    void fft(vector <cpd>& v, bool inv = 0){
        ll n = v.size();
        for(int i = 1,j = 0;i < n;i++){
            ll bit = n >> 1ll;
            for(;j >= bit;bit >>= 1ll) j -= bit;
            j += bit;
            if(i < j) swap(v[i], v[j]);
        }

        for(int k = 1;k < n;k <<= 1ll){
            double a = PI / k * (inv ? 1 : -1);
            cpd w(cos(a), sin(a));
            for(int i = 0;i < n;i += 2 * k){
                cpd wp(1, 0);
                for(int j = 0;j < k;j++){
                    cpd x = v[i + j], y = v[i + j + k] * wp;
                    v[i + j] = x + y;
                    v[i + j + k] = x - y;
                    wp *= w;
                }
            }
        }

        if(inv) for(int i = 0;i < n;i++) v[i] /= n;
    }  
        
    vector <T> mul(const vector <T>& a,const vector <T>& b){
        vector<cpd> av(all(a)), bv(all(b));
        ll n = 2;
        while(n < a.size() + b.size()) n <<= 1;
        av.resize(n); bv.resize(n);
        fft(av); fft(bv);
        for(int i = 0;i < n;i++) av[i] *= bv[i];
        fft(av, 1);

        vector <T> ret(n);
        for(int i = 0;i < n;i++) ret[i] = round(av[i].real());
    
        return ret;
    }
        
    vector <T> carry(vector <T>& a, vector <T>& b){
        reverse(all(a)); reverse(all(b));
        vector <T> ret = mul(a, b);

        for(int i = 0;i < ret.size();i++){
            if(ret[i] < 10) continue;
            if(i < ret.size() - 1) ret[i + 1] += ret[i] / 10;
            else ret.push_back(ret[i] / 10);
            ret[i] %= 10;
        }
        
        while(!ret.empty() && !ret.back()) ret.pop_back();
        if(ret.empty()) return vector<T>(1, 0);
        reverse(all(ret));
        return ret;
    }
};