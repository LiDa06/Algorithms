#include <bits/stdc++.h>

using namespace std;

#define ff first
#define ss second
#define endl '\n'
#define sz(x) (int)(x.size())
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

typedef long long ll;
typedef long double ld;

const ll INF = 1e18 + 1;
const ll N = 1e7;
const ll MOD = 998244353;
const ld eps = 1e-7;
mt19937 rnd(2006);

struct circle{
  ld x, y, r;
};

ld sq(ld x){
  return x * x;
}

bool is_inside(ld x, ld y, circle c){
  return sq(c.x - x) + sq(c.y - y) <= sq(c.r) + eps;
}

void solve() {
  circle a;
  cin >> a.x >> a.y >> a.r;
  circle b;
  cin >> b.x >> b.y >> b.r;
  circle c;
  cin >> c.x >> c.y >> c.r;

  
  ld x1 = max({a.x - a.r, b.x - b.r, c.x - c.r});
  ld y1 = max({a.y - a.r, b.y - b.r, c.y - c.r});
  ld x2 = min({a.x + a.r, b.x + b.r, c.x + c.r});
  ld y2 = min({a.y + a.r, b.y + b.r, c.y + c.r});
  
  ld lenX = x2 - x1;
  ld lenY = y2 - y1;

  int cnt_inside = 0;
  for (int i = 0; i < N; ++i){
    ld randX = x1 + (ld(rnd()) / UINT32_MAX) * lenX;
    ld randY = y1 + (ld(rnd()) / UINT32_MAX) * lenY;

    bool f = is_inside(randX, randY, a);
    f &= is_inside(randX, randY, b);
    f &= is_inside(randX, randY, c);

    if (f) cnt_inside++;
  }

  ld area = ((ld)cnt_inside / (ld)N) * lenX * lenY;

  //cout << x1 << " " << y1 << endl << x2 << " " << y2;
  cout << fixed << setprecision(20) << area;
}

int main() {
#ifdef LOCAL
  freopen("TASK.in", "r", stdin);
  freopen("TASK.out", "w", stdout);
#else

#endif  // LOCAL
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);
  solve();
  return 0;
}