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
const ll N = 100000;
const ll MOD = 998244353;

void insertionSort(vector <int> &a){
  int n = a.size();
  for (int i = 0; i < n; ++i){
    ll mn = i;

    for (int j = i + 1; j < n; ++j){
      if (a[j] < a[mn]){
        mn = j;
      }
    }

    swap(a[i], a[mn]);
  }
}

vector <int> mergeSort(vector <int> a){
  if (a.size() <= 15){
    insertionSort(a);
    return a;
  }

  int n = a.size();
  vector <int> l(n / 2);
  for (int i = 0; i < n / 2; ++i){
    l[i] = a[i];
  }
  vector <int> r((n + 1) / 2);
  for (int i = n / 2; i < n; ++i){
    r[i - n / 2] = a[i];
  }

  l = mergeSort(l);
  r = mergeSort(r);
  a.clear();
  ll id1 = 0;
  ll id2 = 0;
  while (id1 < l.size() && id2 < r.size()){
    if (l[id1] < r[id2]){
      a.push_back(l[id1++]);
    } else {
      a.push_back(r[id2++]);
    }
  }
  while (id1 < l.size()){
    a.push_back(l[id1++]);
  }
  while (id2 < r.size()){
    a.push_back(r[id2++]);
  }

  return a;
}

void solve() {
  int n; cin >> n;
  vector <int> a(n);
  for (auto &x:a) cin >> x;

  a = mergeSort(a);
  for (auto x: a){
    cout << x << " ";
  }
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