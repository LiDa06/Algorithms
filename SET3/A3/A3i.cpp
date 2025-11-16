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
mt19937_64 rnd(20062007);

void insertionSort(vector<int>& a, int l, int r) {
  for (int i = l; i <= r; ++i) {
    int mn = i;

    for (int j = i + 1; j <= r; ++j) {
      if (a[j] < a[mn]) {
        mn = j;
      }
    }

    swap(a[i], a[mn]);
  }
}

void heapify(vector<int>& a, int i, int heapSize, int l) {
  int largest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;

  if (left < heapSize && a[l + left] > a[l + largest]) largest = left;
  if (right < heapSize && a[l + right] > a[l + largest]) largest = right;

  if (largest != i) {
    swap(a[l + i], a[l + largest]);
    heapify(a, largest, heapSize, l);
  }
}

void heapSort(vector<int>& a, int l, int r) {
  int n = r - l + 1;

  for (int i = n / 2 - 1; i >= 0; i--) heapify(a, i, n, l);

  for (int i = n - 1; i > 0; i--) {
    swap(a[l], a[l + i]);  
    heapify(a, 0, i, l); 
  }
}

int partition(vector<int>& a, int l, int r) {
  int pivotIndex = l + rnd() % (r - l + 1);
  int pivot = a[pivotIndex];
  swap(a[pivotIndex], a[r]);
  int i = l;
  for (int j = l; j < r; j++) {
    if (a[j] <= pivot) {
      swap(a[i], a[j]);
      i++;
    }
  }
  swap(a[i], a[r]);
  return i;
}

void quickSort(vector<int>& a, int l, int r, int deep) {
  int n = r - l + 1;

  if (n <= 16) {
    insertionSort(a, l, r);
    return;
  }

  if (deep == 0) {
    heapSort(a, l, r);
    return;
  }

  int pivot = partition(a, l, r);
  quickSort(a, l, pivot - 1, deep - 1);
  quickSort(a, pivot + 1, r, deep - 1);
}

void solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  for (auto& x : a) cin >> x;

  quickSort(a, 0, n - 1, 2 * (int)log2(n));
  for (auto x : a) {
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
