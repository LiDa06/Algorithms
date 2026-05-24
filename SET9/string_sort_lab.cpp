#include <bits/stdc++.h>
using namespace std;

#define endl '\n'
#define ff first
#define ss second
#define sz(a) (int)a.size()
#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

typedef long long ll;
typedef long double ld;

struct Counters {
  unsigned long long charComparisons = 0;
  unsigned long long charInspections = 0;

  void reset() {
    charComparisons = 0;
    charInspections = 0;
  }
};

class LexicographicComparator {
 public:
  explicit LexicographicComparator(Counters* counters = nullptr)
      : counters_(counters) {}

  int compare(const string& a, const string& b) const {
    const size_t m = min(a.size(), b.size());
    for (size_t i = 0; i < m; ++i) {
      if (counters_) {
        ++counters_->charComparisons;
        counters_->charInspections += 2;
      }
      const unsigned char ca = static_cast<unsigned char>(a[i]);
      const unsigned char cb = static_cast<unsigned char>(b[i]);
      if (ca < cb) return -1;
      if (ca > cb) return 1;
    }
    if (a.size() < b.size()) return -1;
    if (a.size() > b.size()) return 1;
    return 0;
  }

  bool less(const string& a, const string& b) const {
    return compare(a, b) < 0;
  }

 private:
  Counters* counters_;
};

class StringGenerator {
 public:
  static constexpr int MIN_LENGTH = 10;
  static constexpr int MAX_LENGTH = 200;
  static constexpr int DEFAULT_MAX_N = 3000;
  static constexpr int ALPHABET_SIZE = 74;

  static const string& alphabet() {
    static const string value =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789"
        "!@#%:;^&*()-";
    return value;
  }

  explicit StringGenerator(uint32_t seed = 123456789) : rng_(seed) {
    if (static_cast<int>(alphabet().size()) != ALPHABET_SIZE) {
      throw runtime_error("Alphabet size is not 74");
    }
  }

  string randomString(int minLen = MIN_LENGTH, int maxLen = MAX_LENGTH) {
    uniform_int_distribution<int> lengthDist(minLen, maxLen);
    uniform_int_distribution<int> charDist(
        0, static_cast<int>(alphabet().size()) - 1);
    const int len = lengthDist(rng_);
    string s;
    s.reserve(len);
    for (int i = 0; i < len; ++i) {
      s.push_back(alphabet()[charDist(rng_)]);
    }
    return s;
  }

  vector<string> randomArray(size_t n) {
    vector<string> a;
    a.reserve(n);
    for (size_t i = 0; i < n; ++i) {
      a.push_back(randomString());
    }
    return a;
  }

  vector<string> randomArrayWithCommonPrefix(size_t n,
                                             size_t prefixLength = 40) {
    string prefix;
    prefix.reserve(prefixLength);
    uniform_int_distribution<int> charDist(
        0, static_cast<int>(alphabet().size()) - 1);
    for (size_t i = 0; i < prefixLength; ++i) {
      prefix.push_back(alphabet()[charDist(rng_)]);
    }

    vector<string> a;
    a.reserve(n);
    uniform_int_distribution<int> totalLenDist(
        max<int>(MIN_LENGTH, static_cast<int>(prefixLength) + 1), MAX_LENGTH);
    for (size_t i = 0; i < n; ++i) {
      const int totalLen = totalLenDist(rng_);
      string s = prefix;
      while (static_cast<int>(s.size()) < totalLen) {
        s.push_back(alphabet()[charDist(rng_)]);
      }
      a.push_back(std::move(s));
    }
    return a;
  }

  vector<string> reversedSortedFrom(vector<string> a) const {
    sort(a.begin(), a.end(), greater<string>());
    return a;
  }

  vector<string> almostSortedFrom(vector<string> a, int blockSize = 100) {
    sort(a.begin(), a.end());
    for (size_t start = 0; start < a.size();
         start += static_cast<size_t>(blockSize)) {
      const size_t finish =
          min(a.size(), start + static_cast<size_t>(blockSize));
      if (finish - start < 2) continue;
      uniform_int_distribution<size_t> dist(start, finish - 1);
      size_t i = dist(rng_);
      size_t j = dist(rng_);
      while (j == i) j = dist(rng_);
      swap(a[i], a[j]);
    }
    return a;
  }

 private:
  mt19937 rng_;
};

class StringSortTester {
 private:
  using SortFunction = function<void(vector<string>&, Counters&)>;

 public:
  static constexpr int ALPHABET_SIZE = StringGenerator::ALPHABET_SIZE;

  struct ResultRow {
    string dataset;
    string algorithm;
    int n = 0;
    int repetitions = 0;
    double avgMicroseconds = 0.0;
    double medianMicroseconds = 0.0;
    double avgCharComparisons = 0.0;
    double avgCharInspections = 0.0;
  };

  StringSortTester() { buildRankTable(); }

  void writeCsvHeader(ostream& out) const {
    out << "dataset,algorithm,n,repetitions,avg_us,median_us,avg_char_"
           "comparisons,avg_char_inspections\n";
  }

  void writeCsvRow(ostream& out, const ResultRow& r) const {
    out << r.dataset << ',' << r.algorithm << ',' << r.n << ',' << r.repetitions
        << ',' << fixed << setprecision(3) << r.avgMicroseconds << ',' << fixed
        << setprecision(3) << r.medianMicroseconds << ',' << fixed
        << setprecision(3) << r.avgCharComparisons << ',' << fixed
        << setprecision(3) << r.avgCharInspections << '\n';
  }

  void runAll(const vector<pair<string, vector<string>>>& datasets, int maxN,
              int step, int repetitions, ostream& out) {
    writeCsvHeader(out);
    const vector<pair<string, SortFunction>> algorithms = {
        {"standard_quicksort",
         [this](vector<string>& a, Counters& c) { standardQuickSort(a, c); }},
        {"standard_mergesort",
         [this](vector<string>& a, Counters& c) { standardMergeSort(a, c); }},
        {"ternary_string_quicksort",
         [this](vector<string>& a, Counters& c) {
           ternaryStringQuickSort(a, c);
         }},
        {"lcp_string_mergesort",
         [this](vector<string>& a, Counters& c) { lcpStringMergeSort(a, c); }},
        {"msd_radix_sort",
         [this](vector<string>& a, Counters& c) { msdRadixSort(a, c, false); }},
        {"msd_radix_sort_with_switch",
         [this](vector<string>& a, Counters& c) { msdRadixSort(a, c, true); }}};

    for (const auto& [datasetName, base] : datasets) {
      if (static_cast<int>(base.size()) < maxN) {
        throw runtime_error("Dataset is smaller than maxN");
      }
      for (int n = step; n <= maxN; n += step) {
        vector<string> sample(base.begin(), base.begin() + n);
        for (const auto& [algorithmName, algorithm] : algorithms) {
          ResultRow row = benchmark(datasetName, algorithmName, sample,
                                    repetitions, algorithm);
          writeCsvRow(out, row);
          cerr << "done: " << datasetName << " / " << algorithmName
               << " / n=" << n << '\n';
        }
      }
    }
  }

  void standardQuickSort(vector<string>& a, Counters& counters) const {
    if (a.empty()) return;
    LexicographicComparator cmp(&counters);
    standardQuickSortImpl(a, 0, static_cast<int>(a.size()) - 1, cmp);
  }

  void standardMergeSort(vector<string>& a, Counters& counters) const {
    if (a.size() < 2) return;
    LexicographicComparator cmp(&counters);
    vector<string> aux(a.size());
    standardMergeSortImpl(a, aux, 0, static_cast<int>(a.size()), cmp);
  }

  void ternaryStringQuickSort(vector<string>& a, Counters& counters) const {
    if (a.empty()) return;
    ternaryStringQuickSortImpl(a, 0, static_cast<int>(a.size()) - 1, 0,
                               counters);
  }

  void lcpStringMergeSort(vector<string>& a, Counters& counters) const {
    if (a.size() < 2) return;
    vector<LcpNode> sorted =
        lcpMergeSortImpl(a, 0, static_cast<int>(a.size()), counters);
    for (size_t i = 0; i < sorted.size(); ++i) {
      a[i] = std::move(sorted[i].s);
    }
  }

  void msdRadixSort(vector<string>& a, Counters& counters,
                    bool switchToQuickSort) const {
    if (a.size() < 2) return;
    vector<string> aux(a.size());
    msdRadixSortImpl(a, 0, static_cast<int>(a.size()), 0, aux, counters,
                     switchToQuickSort);
  }

 private:
  struct LcpNode {
    string s;
    size_t lcpWithPrevious = 0;
  };

  array<int, 256> rank_{};

  void buildRankTable() {
    rank_.fill(-1);
    string chars = StringGenerator::alphabet();
    sort(chars.begin(), chars.end(), [](char a, char b) {
      return static_cast<unsigned char>(a) < static_cast<unsigned char>(b);
    });
    chars.erase(unique(chars.begin(), chars.end()), chars.end());
    if (static_cast<int>(chars.size()) != ALPHABET_SIZE) {
      throw runtime_error(
          "Alphabet contains duplicate symbols or has wrong size");
    }
    for (int i = 0; i < static_cast<int>(chars.size()); ++i) {
      rank_[static_cast<unsigned char>(chars[i])] = i + 1;
    }
  }

  int charRankAt(const string& s, size_t depth, Counters& counters) const {
    ++counters.charInspections;
    if (depth >= s.size()) return 0;
    const int r = rank_[static_cast<unsigned char>(s[depth])];
    if (r < 0) {
      throw runtime_error(
          "String contains a character outside the configured alphabet");
    }
    return r;
  }

  static bool isSortedAscending(const vector<string>& a) {
    return is_sorted(a.begin(), a.end());
  }

  static double average(const vector<double>& values) {
    return accumulate(values.begin(), values.end(), 0.0) /
           static_cast<double>(values.size());
  }

  static double median(vector<double> values) {
    sort(values.begin(), values.end());
    const size_t n = values.size();
    if (n % 2 == 1) return values[n / 2];
    return (values[n / 2 - 1] + values[n / 2]) / 2.0;
  }

  ResultRow benchmark(const string& dataset, const string& algorithmName,
                      const vector<string>& sample, int repetitions,
                      const SortFunction& algorithm) const {
    vector<double> times;
    vector<double> comparisons;
    vector<double> inspections;
    times.reserve(repetitions);
    comparisons.reserve(repetitions);
    inspections.reserve(repetitions);

    for (int rep = 0; rep < repetitions; ++rep) {
      vector<string> data = sample;
      Counters counters;
      const auto begin = chrono::steady_clock::now();
      algorithm(data, counters);
      const auto end = chrono::steady_clock::now();
      if (!isSortedAscending(data)) {
        throw runtime_error("Algorithm produced an unsorted array: " +
                            algorithmName);
      }
      const double us = chrono::duration<double, micro>(end - begin).count();
      times.push_back(us);
      comparisons.push_back(static_cast<double>(counters.charComparisons));
      inspections.push_back(static_cast<double>(counters.charInspections));
    }

    ResultRow row;
    row.dataset = dataset;
    row.algorithm = algorithmName;
    row.n = static_cast<int>(sample.size());
    row.repetitions = repetitions;
    row.avgMicroseconds = average(times);
    row.medianMicroseconds = median(times);
    row.avgCharComparisons = average(comparisons);
    row.avgCharInspections = average(inspections);
    return row;
  }

  static void standardQuickSortImpl(vector<string>& a, int left, int right,
                                    const LexicographicComparator& cmp) {
    int i = left;
    int j = right;
    const string pivot = a[left + (right - left) / 2];

    while (i <= j) {
      while (cmp.less(a[i], pivot)) ++i;
      while (cmp.less(pivot, a[j])) --j;
      if (i <= j) {
        swap(a[i], a[j]);
        ++i;
        --j;
      }
    }
    if (left < j) standardQuickSortImpl(a, left, j, cmp);
    if (i < right) standardQuickSortImpl(a, i, right, cmp);
  }

  static void standardMergeSortImpl(vector<string>& a, vector<string>& aux,
                                    int left, int right,
                                    const LexicographicComparator& cmp) {
    if (right - left <= 1) return;
    const int mid = left + (right - left) / 2;
    standardMergeSortImpl(a, aux, left, mid, cmp);
    standardMergeSortImpl(a, aux, mid, right, cmp);

    int i = left;
    int j = mid;
    int k = left;
    while (i < mid && j < right) {
      if (cmp.compare(a[i], a[j]) <= 0) {
        aux[k++] = std::move(a[i++]);
      } else {
        aux[k++] = std::move(a[j++]);
      }
    }
    while (i < mid) aux[k++] = std::move(a[i++]);
    while (j < right) aux[k++] = std::move(a[j++]);
    for (int p = left; p < right; ++p) {
      a[p] = std::move(aux[p]);
    }
  }

  void ternaryStringQuickSortImpl(vector<string>& a, int left, int right,
                                  int depth, Counters& counters) const {
    if (left >= right) return;

    const int pivotIndex = left + (right - left) / 2;
    swap(a[left], a[pivotIndex]);
    const int pivot = charRankAt(a[left], depth, counters);

    int lt = left;
    int gt = right;
    int i = left + 1;
    while (i <= gt) {
      const int current = charRankAt(a[i], depth, counters);
      ++counters.charComparisons;
      if (current < pivot) {
        swap(a[lt++], a[i++]);
      } else if (current > pivot) {
        swap(a[i], a[gt--]);
      } else {
        ++i;
      }
    }

    ternaryStringQuickSortImpl(a, left, lt - 1, depth, counters);
    if (pivot > 0) {
      ternaryStringQuickSortImpl(a, lt, gt, depth + 1, counters);
    }
    ternaryStringQuickSortImpl(a, gt + 1, right, depth, counters);
  }

  pair<int, size_t> compareFromDepth(const string& a, const string& b,
                                     size_t depth, Counters& counters) const {
    size_t i = depth;
    const size_t m = min(a.size(), b.size());
    while (i < m) {
      ++counters.charComparisons;
      counters.charInspections += 2;
      const unsigned char ca = static_cast<unsigned char>(a[i]);
      const unsigned char cb = static_cast<unsigned char>(b[i]);
      if (ca < cb) return {-1, i};
      if (ca > cb) return {1, i};
      ++i;
    }
    if (a.size() < b.size()) return {-1, m};
    if (a.size() > b.size()) return {1, m};
    return {0, m};
  }

  vector<LcpNode> lcpMergeSortImpl(const vector<string>& a, int left, int right,
                                   Counters& counters) const {
    if (right - left == 1) {
      return vector<LcpNode>{{a[left], 0}};
    }
    const int mid = left + (right - left) / 2;
    vector<LcpNode> first = lcpMergeSortImpl(a, left, mid, counters);
    vector<LcpNode> second = lcpMergeSortImpl(a, mid, right, counters);
    return mergeLcpSortedLists(std::move(first), std::move(second), counters);
  }

  vector<LcpNode> mergeLcpSortedLists(vector<LcpNode>&& left,
                                      vector<LcpNode>&& right,
                                      Counters& counters) const {
    vector<LcpNode> out;
    out.reserve(left.size() + right.size());

    size_t i = 0;
    size_t j = 0;
    size_t hLeft = 0;
    size_t hRight = 0;
    bool hasPrevious = false;

    auto appendRemaining = [&](vector<LcpNode>& list, size_t& index,
                               size_t hHead) {
      bool first = true;
      while (index < list.size()) {
        size_t h = first && hasPrevious ? hHead : list[index].lcpWithPrevious;
        if (!hasPrevious) h = 0;
        out.push_back({std::move(list[index].s), h});
        hasPrevious = true;
        ++index;
        first = false;
      }
    };

    while (i < left.size() && j < right.size()) {
      bool takeLeft = false;
      size_t winnerLcpWithPrevious = 0;
      size_t lcpBetweenHeads = 0;

      if (!hasPrevious) {
        const auto [cmp, h] =
            compareFromDepth(left[i].s, right[j].s, 0, counters);
        takeLeft = (cmp <= 0);
        winnerLcpWithPrevious = 0;
        lcpBetweenHeads = h;
      } else if (hLeft > hRight) {
        takeLeft = true;
        winnerLcpWithPrevious = hLeft;
        lcpBetweenHeads = hRight;
      } else if (hRight > hLeft) {
        takeLeft = false;
        winnerLcpWithPrevious = hRight;
        lcpBetweenHeads = hLeft;
      } else {
        const auto [cmp, h] =
            compareFromDepth(left[i].s, right[j].s, hLeft, counters);
        takeLeft = (cmp <= 0);
        winnerLcpWithPrevious = hLeft;
        lcpBetweenHeads = h;
      }

      if (takeLeft) {
        out.push_back({std::move(left[i].s), winnerLcpWithPrevious});
        hasPrevious = true;
        ++i;
        hLeft = (i < left.size() ? left[i].lcpWithPrevious : 0);
        hRight = lcpBetweenHeads;
      } else {
        out.push_back({std::move(right[j].s), winnerLcpWithPrevious});
        hasPrevious = true;
        ++j;
        hRight = (j < right.size() ? right[j].lcpWithPrevious : 0);
        hLeft = lcpBetweenHeads;
      }
    }

    if (i < left.size()) appendRemaining(left, i, hLeft);
    if (j < right.size()) appendRemaining(right, j, hRight);
    return out;
  }

  void msdRadixSortImpl(vector<string>& a, int left, int right, int depth,
                        vector<string>& aux, Counters& counters,
                        bool switchToQuickSort) const {
    const int n = right - left;
    if (n <= 1) return;

    if (switchToQuickSort && n < ALPHABET_SIZE) {
      ternaryStringQuickSortImpl(a, left, right - 1, depth, counters);
      return;
    }

    constexpr int R = ALPHABET_SIZE;
    vector<int> count(R + 2, 0);

    for (int i = left; i < right; ++i) {
      const int c = charRankAt(a[i], static_cast<size_t>(depth), counters);
      ++count[c + 1];
    }
    for (int r = 0; r <= R; ++r) {
      count[r + 1] += count[r];
    }

    vector<int> start = count;
    for (int i = left; i < right; ++i) {
      const int c = charRankAt(a[i], static_cast<size_t>(depth), counters);
      aux[start[c]++] = std::move(a[i]);
    }
    for (int i = 0; i < n; ++i) {
      a[left + i] = std::move(aux[i]);
    }

    for (int c = 1; c <= R; ++c) {
      const int bucketLeft = left + count[c];
      const int bucketRight = left + count[c + 1];
      if (bucketRight - bucketLeft > 1) {
        msdRadixSortImpl(a, bucketLeft, bucketRight, depth + 1, aux, counters,
                         switchToQuickSort);
      }
    }
  }
};

int main(int argc, char** argv) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const string outputPath = (argc >= 2 ? argv[1] : "results.csv");
  const int repetitions = (argc >= 3 ? stoi(argv[2]) : 7);
  const uint32_t seed =
      (argc >= 4 ? static_cast<uint32_t>(stoul(argv[3])) : 20260522u);
  const int maxN = StringGenerator::DEFAULT_MAX_N;
  const int step = 100;

  StringGenerator generator(seed);
  vector<string> randomData = generator.randomArray(maxN);
  vector<string> reversedData = generator.reversedSortedFrom(randomData);
  vector<string> almostSortedData = generator.almostSortedFrom(randomData);
  vector<string> commonPrefixData =
      generator.randomArrayWithCommonPrefix(maxN, 40);

  vector<pair<string, vector<string>>> datasets = {
      {"random", std::move(randomData)},
      {"reversed", std::move(reversedData)},
      {"almost_sorted", std::move(almostSortedData)},
      {"common_prefix", std::move(commonPrefixData)}};

  ofstream out(outputPath);
  if (!out) {
    cerr << "Cannot open output file: " << outputPath << '\n';
    return 1;
  }

  try {
    StringSortTester tester;
    tester.runAll(datasets, maxN, step, repetitions, out);
  } catch (const exception& e) {
    cerr << "Error: " << e.what() << '\n';
    return 2;
  }

  cerr << "CSV saved to " << outputPath << '\n';
  return 0;
}
