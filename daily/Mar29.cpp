#include <bits/stdc++.h>
using namespace std;

using ll = long long;

class Solution {
 public:
  /**
   * @brief LC2580: Count ways to group overlapping ranges
   * M
   *
   * @param ranges
   * @return int
   */
  int countWays(vector<vector<int>>& ranges) {
    static constexpr int mod = 1e9 + 7;

    sort(ranges.begin(), ranges.end());
    int n = ranges.size();
    ll res = 1;
    for (int i = 0; i < n;) {
      int r = ranges[i][1];
      int j = i + 1;
      while (j < n && ranges[j][0] <= r) {
        r = max(r, ranges[j][1]);
        j++;
      }
      res = res * 2 % mod;
      i = j;
    }
    return res;
  }

  vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
    // sort(points.begin(), points.end(),
    //  [&](const vector<int>& u, const vector<int>& v) {
    //  return u[0] * u[0] + u[1] * u[1] < v[0] * v[0] + v[1] * v[1];
    //  });
    sort(points.begin(), points.end(),
         [&](const vector<int>& lhs, const vector<int>& rhs) {
           return lhs[0] * lhs[0] + lhs[1] * lhs[1] <
                  rhs[0] * rhs[0] + rhs[1] * rhs[1];
         });
    return {points.begin(), points.begin() + k};
  }

  /**
   * @brief LC159: Longest substring w/ at most two
   * distinct chars
   *
   * SLiding window problem
   *
   * @param s
   * @return int
   */
  int lengthOfLongestSubstringTwoDistinct(string s) {
    size_t n = s.length();
    if (n < 3) return n;

    // le and ri pointer of window
    int le = 0, ri = 0;
    map<char, int> cache;
    int max_len = 2;

    while (ri < n) {
      // lt 3 chars
      cache[s[ri]] = ri;
      ri++;

      // 3 chars window
      if (cache.size() == 3) {
        int del_idx = INT_MAX;
        for (pair<char, int> ele : cache) {
          del_idx = min(del_idx, ele.second);
        }
        // delete leftmost char
        cache.erase(s[del_idx]);
        le = del_idx + 1;
      }
      max_len = max(max_len, ri - le);
    }

    return max_len;
  }

  /**
   * @brief LC243: shortest word distance
   *
   * @param wordsDict
   * @param word1
   * @param word2
   * @return int
   */
  int shortestDistance(vector<string>& wordsDict, string word1, string word2) {
    int n = wordsDict.size();
    int idx1 = -1, idx2 = -1;
    int ans = n;
    for (int i = 0; i < n; i++) {
      string word = wordsDict[i];
      if (word == word1) {
        idx1 = i;
      } else if (word == word2) {
        idx2 = i;
      }
      if (idx1 >= 0 && idx2 >= 0) {
        ans = min(ans, abs(idx1 - idx2));
      }
    }

    return ans;
  }

  /**
   * @brief LC245: Shortest word distance III
   * the word1 and word2 maybe same
   *
   * @param wordsDict
   * @param word1
   * @param word2
   * @return int
   */
  int shortestWordDistance(vector<string>& wordsDict, string word1,
                           string word2) {
    int n = wordsDict.size();
    int ans = n;
    if (word1 == word2) {
      int prev =
          -1;  // this is the core, flag to check if this is first or second
      for (int i = 0; i < n; i++) {
        string word = wordsDict[i];
        if (word == word1) {
          if (prev >= 0) {
            ans = min(ans, i - prev);
          }
          prev = i;
        }
      }
    } else {
      int idx1 = -1, idx2 = -1;
      for (int i = 0; i < n; i++) {
        string word = wordsDict[i];
        if (word == word1) {
          idx1 = i;
        } else if (word == word2) {
          idx2 = i;
        }
        if (idx1 >= 0 && idx2 >= 0) {
          ans = min(ans, abs(idx1 - idx2));
        }
      }
    }

    return ans;
  }
};

/**
 * @brief LC244: disign the ds for WordDistance
 *
 * This also use a hashmap
 *
 */
class WordDistance {
 private:
  unordered_map<string, vector<int>> idmap;

 public:
  WordDistance(vector<string>& wordsDict) {
    int len = wordsDict.size();
    for (int i = 0; i < len; i++) {
      string word = wordsDict[i];
      idmap[word].emplace_back(i);
    }
  }

  int shortest(string word1, string word2) {
    vector<int> idxes1 = idmap[word1];
    vector<int> idxes2 = idmap[word2];
    int size1 = idxes1.size(), size2 = idxes2.size();
    int pos1 = 0, pos2 = 0;
    int ans = INT_MAX;
    while (pos1 < size1 && pos2 < size2) {
      int idx1 = idxes1[pos1], idx2 = idxes2[pos2];
      ans = min(ans, abs(idx1 - idx2));
      if (idx1 < idx2) {
        pos1++;
      } else {
        pos2++;
      }
    }
    return ans;
  }
};

class Dijkstra {
  /**
   * @brief LC743: Network Delay Time
   * [M]
   * use dijkstra algorithm
   *
   * @param times
   * @param n
   * @param k
   * @return int
   */
  int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<int>> g(n, vector<int>(n, INT_MAX / 2));  // neighbor matrix
    for (auto& t : times) {
      g[t[0] - 1][t[1] - 1] = t[2];
    }

    vector<int> dis(n, INT_MAX / 2), done(n);
    dis[k - 1] = 0;
    while (true) {
      int x = -1;
      for (int i = 0; i < n; i++) {
        if (!done[i] && (x < 0 || dis[i] < dis[x])) {
          x = i;
        }
      }

      if (x < 0) {
        return *max_element(dis.begin(), dis.end());
      }

      if (dis[x] == INT_MAX / 2) {  // some node unreachable
        return -1;
      }

      done[x] = true;  // shortest path
      for (int y = 0; y < n; y++) {
        // update x neighbor shortest path
        dis[y] = min(dis[y], dis[x] + g[x][y]);
      }
    }
  }
};