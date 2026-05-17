#pragma once
/**
 * algorithms.h
 * C++ translation of guiff0/fisrtApp — Java algorithms library.
 * Covers: Arrays, Strings, Linked List, Stack, Binary Tree,
 *         Graphs, Dynamic Programming, Greedy, Heaps,
 *         Bit Manipulation, Trie, Huffman Encoding.
 *
 * Compile (C++17):  g++ -std=c++17 -O2 -o algorithms_test algorithms_test.cpp
 */

#include <algorithm>
#include <climits>
#include <cmath>
#include <deque>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// ARRAYS
// ─────────────────────────────────────────────────────────────────────────────

// Return indices of two numbers that add to target.
std::vector<int> twoSum(std::vector<int> nums, int target) {
    std::unordered_map<int, int> seen;
    for (int i = 0; i < (int)nums.size(); ++i) {
        int complement = target - nums[i];
        if (seen.count(complement))
            return {seen[complement], i};
        seen[nums[i]] = i;
    }
    return {};
}

// Kadane's algorithm — maximum subarray sum.
int maxSubarray(std::vector<int> nums) {
    int maxSum = nums[0], cur = nums[0];
    for (int i = 1; i < (int)nums.size(); ++i) {
        cur = std::max(nums[i], cur + nums[i]);
        maxSum = std::max(maxSum, cur);
    }
    return maxSum;
}

// Merge all overlapping intervals.
std::vector<std::vector<int>> mergeIntervals(std::vector<std::vector<int>> intervals) {
    std::sort(intervals.begin(), intervals.end());
    std::vector<std::vector<int>> merged = {intervals[0]};
    for (int i = 1; i < (int)intervals.size(); ++i) {
        if (intervals[i][0] <= merged.back()[1])
            merged.back()[1] = std::max(merged.back()[1], intervals[i][1]);
        else
            merged.push_back(intervals[i]);
    }
    return merged;
}

// Find minimum in a rotated sorted array (binary search).
int minRotatedArray(std::vector<int> nums) {
    int lo = 0, hi = (int)nums.size() - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (nums[mid] > nums[hi]) lo = mid + 1;
        else                      hi = mid;
    }
    return nums[lo];
}

// Two-pointer approach for trapping rainwater.
int trappingRainwater(std::vector<int> height) {
    int lo = 0, hi = (int)height.size() - 1;
    int leftMax = 0, rightMax = 0, water = 0;
    while (lo <= hi) {
        if (height[lo] <= height[hi]) {
            if (height[lo] >= leftMax) leftMax = height[lo];
            else                       water += leftMax - height[lo];
            ++lo;
        } else {
            if (height[hi] >= rightMax) rightMax = height[hi];
            else                        water += rightMax - height[hi];
            --hi;
        }
    }
    return water;
}

// All unique triplets that sum to zero.
std::vector<std::vector<int>> threeSum(std::vector<int> nums) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> result;
    int n = (int)nums.size();
    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        int lo = i + 1, hi = n - 1;
        while (lo < hi) {
            int s = nums[i] + nums[lo] + nums[hi];
            if (s == 0) {
                result.push_back({nums[i], nums[lo], nums[hi]});
                while (lo < hi && nums[lo] == nums[lo + 1]) ++lo;
                while (lo < hi && nums[hi] == nums[hi - 1]) --hi;
                ++lo; --hi;
            } else if (s < 0) ++lo;
            else              --hi;
        }
    }
    return result;
}

// Product of every element except self — O(n), no division.
std::vector<int> productExceptSelf(std::vector<int> nums) {
    int n = (int)nums.size();
    std::vector<int> out(n, 1);
    int prefix = 1;
    for (int i = 0; i < n; ++i) { out[i] = prefix; prefix *= nums[i]; }
    int suffix = 1;
    for (int i = n - 1; i >= 0; --i) { out[i] *= suffix; suffix *= nums[i]; }
    return out;
}

// Two-pointer approach for container with most water.
int containerWithMostWater(std::vector<int> height) {
    int lo = 0, hi = (int)height.size() - 1, maxWater = 0;
    while (lo < hi) {
        maxWater = std::max(maxWater, std::min(height[lo], height[hi]) * (hi - lo));
        if (height[lo] < height[hi]) ++lo;
        else                         --hi;
    }
    return maxWater;
}

// Remove duplicates in-place from sorted array; return new length.
int removeDuplicatesSorted(std::vector<int>& nums) {
    if (nums.empty()) return 0;
    int k = 1;
    for (int i = 1; i < (int)nums.size(); ++i)
        if (nums[i] != nums[i - 1])
            nums[k++] = nums[i];
    return k;
}

// Rotate array to the right by k steps (in-place, three-reverse trick).
void rotateArray(std::vector<int>& nums, int k) {
    int n = (int)nums.size();
    k %= n;
    std::reverse(nums.begin(), nums.end());
    std::reverse(nums.begin(), nums.begin() + k);
    std::reverse(nums.begin() + k, nums.end());
}

// ─────────────────────────────────────────────────────────────────────────────
// STRINGS
// ─────────────────────────────────────────────────────────────────────────────

bool anagramCheck(std::string s, std::string t) {
    if (s.size() != t.size()) return false;
    std::sort(s.begin(), s.end());
    std::sort(t.begin(), t.end());
    return s == t;
}

// Sliding window — longest substring without repeating characters.
int longestSubstringNoRepeat(const std::string& s) {
    std::unordered_map<char, int> charIndex;
    int best = 0, start = 0;
    for (int i = 0; i < (int)s.size(); ++i) {
        char ch = s[i];
        if (charIndex.count(ch) && charIndex[ch] >= start)
            start = charIndex[ch] + 1;
        charIndex[ch] = i;
        best = std::max(best, i - start + 1);
    }
    return best;
}

// Expand-around-center for longest palindromic substring.
std::string longestPalindromicSubstring(const std::string& s) {
    std::string best;
    auto expand = [&](int l, int r) {
        while (l >= 0 && r < (int)s.size() && s[l] == s[r]) { --l; ++r; }
        std::string p = s.substr(l + 1, r - l - 1);
        if (p.size() > best.size()) best = p;
    };
    for (int i = 0; i < (int)s.size(); ++i) {
        expand(i, i);
        expand(i, i + 1);
    }
    return best;
}

// Reverse words in a sentence.
std::string reverseWords(std::string s) {
    std::istringstream iss(s);
    std::vector<std::string> words;
    std::string w;
    while (iss >> w) words.push_back(w);
    std::reverse(words.begin(), words.end());
    std::string result;
    for (int i = 0; i < (int)words.size(); ++i) {
        if (i) result += ' ';
        result += words[i];
    }
    return result;
}

// String to integer with sign and overflow handling.
int myAtoi(std::string s) {
    int i = 0, n = (int)s.size();
    while (i < n && s[i] == ' ') ++i;
    if (i == n) return 0;
    int sign = 1;
    if (s[i] == '-') { sign = -1; ++i; }
    else if (s[i] == '+') ++i;
    long num = 0;
    while (i < n && std::isdigit(s[i])) {
        num = num * 10 + (s[i++] - '0');
        if (num * sign >  INT_MAX) return  INT_MAX;
        if (num * sign <  INT_MIN) return  INT_MIN;
    }
    return (int)(sign * num);
}

bool validPalindrome(const std::string& s) {
    int lo = 0, hi = (int)s.size() - 1;
    while (lo < hi) {
        while (lo < hi && !std::isalnum(s[lo])) ++lo;
        while (lo < hi && !std::isalnum(s[hi])) --hi;
        if (std::tolower(s[lo]) != std::tolower(s[hi])) return false;
        ++lo; --hi;
    }
    return true;
}

// Count-and-say sequence.
std::string countAndSay(int n) {
    std::string result = "1";
    for (int iter = 1; iter < n; ++iter) {
        std::string nxt;
        int i = 0;
        while (i < (int)result.size()) {
            char ch = result[i];
            int count = 0;
            while (i < (int)result.size() && result[i] == ch) { ++i; ++count; }
            nxt += std::to_string(count) + ch;
        }
        result = nxt;
    }
    return result;
}

// Group anagrams together.
std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string> strs) {
    std::unordered_map<std::string, std::vector<std::string>> groups;
    for (auto& s : strs) {
        std::string key = s;
        std::sort(key.begin(), key.end());
        groups[key].push_back(s);
    }
    std::vector<std::vector<std::string>> result;
    for (auto& [_, v] : groups) result.push_back(v);
    return result;
}

// Longest common prefix among a list of strings.
std::string longestCommonPrefix(std::vector<std::string> strs) {
    if (strs.empty()) return "";
    std::string prefix = strs[0];
    for (int i = 1; i < (int)strs.size(); ++i)
        while (strs[i].find(prefix) != 0) {
            prefix.pop_back();
            if (prefix.empty()) return "";
        }
    return prefix;
}

// Compress chars in-place; return new length.
int stringCompression(std::vector<char>& chars) {
    int write = 0, read = 0, n = (int)chars.size();
    while (read < n) {
        char ch = chars[read];
        int count = 0;
        while (read < n && chars[read] == ch) { ++read; ++count; }
        chars[write++] = ch;
        if (count > 1)
            for (char c : std::to_string(count))
                chars[write++] = c;
    }
    return write;
}

// ─────────────────────────────────────────────────────────────────────────────
// LINKED LIST
// ─────────────────────────────────────────────────────────────────────────────

struct ListNode {
    int val;
    ListNode* next;
    explicit ListNode(int v = 0, ListNode* n = nullptr) : val(v), next(n) {}
};

// Factory helpers (used in tests).
ListNode* makeList(const std::vector<int>& vals) {
    ListNode dummy;
    ListNode* cur = &dummy;
    for (int v : vals) { cur->next = new ListNode(v); cur = cur->next; }
    return dummy.next;
}
std::vector<int> toVector(ListNode* head) {
    std::vector<int> r;
    while (head) { r.push_back(head->val); head = head->next; }
    return r;
}
void freeList(ListNode* head) {
    while (head) { ListNode* t = head->next; delete head; head = t; }
}

ListNode* reverseLinkedList(ListNode* head) {
    ListNode* prev = nullptr, *curr = head;
    while (curr) {
        ListNode* nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    return prev;
}

bool detectCycle(ListNode* head) {
    ListNode* slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

ListNode* mergeTwoSortedLists(ListNode* l1, ListNode* l2) {
    ListNode dummy;
    ListNode* curr = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) { curr->next = l1; l1 = l1->next; }
        else                    { curr->next = l2; l2 = l2->next; }
        curr = curr->next;
    }
    curr->next = l1 ? l1 : l2;
    return dummy.next;
}

ListNode* middleOfLinkedList(ListNode* head) {
    ListNode* slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(0, head);
    ListNode* fast = &dummy, *slow = &dummy;
    for (int i = 0; i <= n; ++i) fast = fast->next;
    while (fast) { fast = fast->next; slow = slow->next; }
    ListNode* del = slow->next;
    slow->next = slow->next->next;
    delete del;
    return dummy.next;
}

// ─────────────────────────────────────────────────────────────────────────────
// STACK
// ─────────────────────────────────────────────────────────────────────────────

// Convert space-separated infix expression to postfix (Shunting-yard, left-associative).
std::string infixToPostfix(const std::string& expr) {
    auto getPrec = [](const std::string& op) -> int {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        if (op == "^")              return 3;
        return 0;
    };
    auto isOperator = [](const std::string& t) {
        return t == "+" || t == "-" || t == "*" || t == "/" || t == "^";
    };
    auto isNumber = [](const std::string& t) -> bool {
        if (t.empty()) return false;
        int start = (t[0] == '-' && t.size() > 1) ? 1 : 0;
        for (int i = start; i < (int)t.size(); ++i)
            if (!std::isdigit((unsigned char)t[i])) return false;
        return true;
    };

    std::vector<std::string> output;
    std::stack<std::string> stk;
    std::istringstream iss(expr);
    std::string token;
    while (iss >> token) {
        if (isNumber(token)) {
            output.push_back(token);
        } else if (token == "(") {
            stk.push(token);
        } else if (token == ")") {
            while (!stk.empty() && stk.top() != "(") { output.push_back(stk.top()); stk.pop(); }
            if (!stk.empty()) stk.pop();
        } else if (isOperator(token)) {
            while (!stk.empty() && stk.top() != "(" && getPrec(stk.top()) >= getPrec(token)) {
                output.push_back(stk.top()); stk.pop();
            }
            stk.push(token);
        }
    }
    while (!stk.empty()) { output.push_back(stk.top()); stk.pop(); }
    std::string result;
    for (int i = 0; i < (int)output.size(); ++i) { if (i) result += ' '; result += output[i]; }
    return result;
}

// Next greater element for each position (-1 if none).
std::vector<int> nextGreaterElement(const std::vector<int>& nums) {
    int n = (int)nums.size();
    std::vector<int> result(n, -1);
    std::stack<int> stk;
    for (int i = 0; i < n; ++i) {
        while (!stk.empty() && nums[stk.top()] < nums[i]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }
    return result;
}

class MinStack {
    std::stack<std::pair<int,int>> st;  // {val, current_min}
public:
    void push(int val) {
        int curMin = st.empty() ? val : std::min(val, st.top().second);
        st.push({val, curMin});
    }
    void pop()      { st.pop(); }
    int top()       { return st.top().first; }
    int getMin()    { return st.top().second; }
};

// Largest rectangle in histogram.
int largestRectangleHistogram(std::vector<int> heights) {
    heights.push_back(0);
    std::stack<std::pair<int,int>> stk;  // {start_index, height}
    int maxArea = 0;
    for (int i = 0; i < (int)heights.size(); ++i) {
        int start = i;
        while (!stk.empty() && stk.top().second > heights[i]) {
            auto [idx, ht] = stk.top(); stk.pop();
            maxArea = std::max(maxArea, ht * (i - idx));
            start = idx;
        }
        stk.push({start, heights[i]});
    }
    return maxArea;
}

// Reverse first k elements of a deque, keeping the rest in original order.
std::deque<int> reverseFirstKQueue(std::deque<int> q, int k) {
    // Step 1: pop first k into a stack (reverses them)
    std::stack<int> stk;
    for (int i = 0; i < k; ++i) { stk.push(q.front()); q.pop_front(); }
    // Step 2: push reversed elements to the back of the remaining queue
    while (!stk.empty()) { q.push_back(stk.top()); stk.pop(); }
    // Step 3: rotate the (size-k) original tail elements to the back
    int tail = (int)q.size() - k;
    for (int i = 0; i < tail; ++i) { q.push_back(q.front()); q.pop_front(); }
    return q;
}

// ─────────────────────────────────────────────────────────────────────────────
// BINARY TREE
// ─────────────────────────────────────────────────────────────────────────────

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int v = 0) : val(v), left(nullptr), right(nullptr) {}
};

// Free an entire subtree.
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// BFS level-order traversal.
std::vector<std::vector<int>> levelOrderTraversal(TreeNode* root) {
    if (!root) return {};
    std::vector<std::vector<int>> result;
    std::queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int sz = (int)q.size();
        std::vector<int> level;
        for (int i = 0; i < sz; ++i) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}

// Check if two trees are mirrors of each other.
bool mirrorTrees(TreeNode* p, TreeNode* q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return mirrorTrees(p->left, q->right) && mirrorTrees(p->right, q->left);
}

// Lowest common ancestor in a BST.
TreeNode* lowestCommonAncestorBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val)      root = root->left;
        else if (p->val > root->val && q->val > root->val) root = root->right;
        else return root;
    }
    return nullptr;
}

// Serialize a binary tree to a comma-separated preorder string.
std::string serialize(TreeNode* root) {
    if (!root) return "null";
    return std::to_string(root->val) + "," + serialize(root->left) + "," + serialize(root->right);
}

// Deserialize from the format produced by serialize().
TreeNode* deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::function<TreeNode*()> build = [&]() -> TreeNode* {
        std::string tok;
        std::getline(iss, tok, ',');
        if (tok == "null") return nullptr;
        TreeNode* node = new TreeNode(std::stoi(tok));
        node->left  = build();
        node->right = build();
        return node;
    };
    return build();
}

// Validate that a binary tree is a valid BST.
bool validateBST(TreeNode* root,
                 long lo = (long)INT_MIN - 1,
                 long hi = (long)INT_MAX + 1) {
    if (!root) return true;
    if (root->val <= lo || root->val >= hi) return false;
    return validateBST(root->left, lo, root->val) &&
           validateBST(root->right, root->val, hi);
}

// ─────────────────────────────────────────────────────────────────────────────
// GRAPHS
// ─────────────────────────────────────────────────────────────────────────────

// Kahn's BFS-based topological sort. Returns {} if cycle detected.
std::vector<int> topologicalSort(int numVertices,
                                  const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> graph(numVertices);
    std::vector<int> inDegree(numVertices, 0);
    for (auto [u, v] : edges) { graph[u].push_back(v); ++inDegree[v]; }
    std::queue<int> q;
    for (int i = 0; i < numVertices; ++i)
        if (inDegree[i] == 0) q.push(i);
    std::vector<int> order;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        order.push_back(node);
        for (int nb : graph[node])
            if (--inDegree[nb] == 0) q.push(nb);
    }
    return (int)order.size() == numVertices ? order : std::vector<int>{};
}

// DFS three-colour cycle detection in a directed graph.
bool detectCycleDirected(int numVertices,
                          const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::vector<int>> graph(numVertices);
    for (auto [u, v] : edges) graph[u].push_back(v);
    enum Color { WHITE, GRAY, BLACK };
    std::vector<Color> color(numVertices, WHITE);
    std::function<bool(int)> dfs = [&](int node) -> bool {
        color[node] = GRAY;
        for (int nb : graph[node]) {
            if (color[nb] == GRAY) return true;
            if (color[nb] == WHITE && dfs(nb)) return true;
        }
        color[node] = BLACK;
        return false;
    };
    for (int i = 0; i < numVertices; ++i)
        if (color[i] == WHITE && dfs(i)) return true;
    return false;
}

// DFS flood-fill — number of islands. Modifies grid in place.
int numberOfIslands(std::vector<std::vector<char>> grid) {
    if (grid.empty()) return 0;
    int rows = (int)grid.size(), cols = (int)grid[0].size(), count = 0;
    std::function<void(int,int)> dfs = [&](int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || grid[r][c] != '1') return;
        grid[r][c] = '0';
        dfs(r+1,c); dfs(r-1,c); dfs(r,c+1); dfs(r,c-1);
    };
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (grid[r][c] == '1') { dfs(r, c); ++count; }
    return count;
}

// BFS shortest path distances in an unweighted graph (-1 = unreachable).
std::vector<int> shortestPathUnweighted(const std::vector<std::vector<int>>& graph, int src) {
    int n = (int)graph.size();
    std::vector<int> dist(n, -1);
    dist[src] = 0;
    std::queue<int> q;
    q.push(src);
    while (!q.empty()) {
        int node = q.front(); q.pop();
        for (int nb : graph[node])
            if (dist[nb] == -1) { dist[nb] = dist[node] + 1; q.push(nb); }
    }
    return dist;
}

// BFS two-colouring bipartite check.
bool bipartiteCheck(const std::vector<std::vector<int>>& graph) {
    int n = (int)graph.size();
    std::vector<int> color(n, -1);
    for (int start = 0; start < n; ++start) {
        if (color[start] != -1) continue;
        color[start] = 0;
        std::queue<int> q;
        q.push(start);
        while (!q.empty()) {
            int node = q.front(); q.pop();
            for (int nb : graph[node]) {
                if (color[nb] == -1) { color[nb] = 1 - color[node]; q.push(nb); }
                else if (color[nb] == color[node]) return false;
            }
        }
    }
    return true;
}

// DP + Bitmask Traveling Salesman Problem.
int travelingSalesman(const std::vector<std::vector<int>>& dist) {
    int n = (int)dist.size();
    int FULL = (1 << n) - 1;
    const int INF = INT_MAX / 2;
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));
    dp[1][0] = 0;
    for (int mask = 0; mask < (1 << n); ++mask)
        for (int u = 0; u < n; ++u) {
            if (!(mask >> u & 1) || dp[mask][u] == INF) continue;
            for (int v = 0; v < n; ++v) {
                if (mask >> v & 1) continue;
                int newMask = mask | (1 << v);
                dp[newMask][v] = std::min(dp[newMask][v], dp[mask][u] + dist[u][v]);
            }
        }
    int best = INF;
    for (int v = 1; v < n; ++v)
        if (dp[FULL][v] != INF)
            best = std::min(best, dp[FULL][v] + dist[v][0]);
    return best;
}

// ── Edge type used by ConnectedCitiesShortestPath ────────────────────────────
// Mirrors the inner class ConnectedCitiesShortestPath.Edge from the Java source.
struct Edge {
    int to;    // destination node
    int weight;
    Edge(int to, int weight) : to(to), weight(weight) {}
};

// Overload 1 — adjacency-list form (matches the Java testDijkstra signature).
// graph[u] holds all Edge objects leaving u.
// Returns integer distances (INT_MAX = unreachable), matching Java int[].
std::vector<int> connectedCitiesShortestPath(
        int n,
        const std::vector<std::vector<Edge>>& graph,
        int src) {
    const int INF = INT_MAX / 2;
    std::vector<int> dist(n, INF);
    dist[src] = 0;
    // min-heap: {dist, node}
    std::priority_queue<std::pair<int,int>,
                        std::vector<std::pair<int,int>>,
                        std::greater<>> pq;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (const Edge& e : graph[u])
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                pq.push({dist[e.to], e.to});
            }
    }
    // Restore true INT_MAX for unreachable nodes so callers can detect them.
    for (int& d : dist) if (d == INF) d = INT_MAX;
    return dist;
}

// Overload 2 — edge-list form (original helper, kept for existing tests).
// Returns double distances so that infinity is representable.
std::vector<double> connectedCitiesShortestPath(
        int n,
        const std::vector<std::tuple<int,int,int>>& edges,
        int src) {
    std::vector<std::vector<std::pair<int,int>>> graph(n);
    for (auto [u, v, w] : edges) {
        graph[u].push_back({w, v});
        graph[v].push_back({w, u});
    }
    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> dist(n, INF);
    dist[src] = 0;
    std::priority_queue<std::pair<double,int>,
                        std::vector<std::pair<double,int>>,
                        std::greater<>> pq;
    pq.push({0.0, src});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        for (auto [w, v] : graph[u])
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
    }
    return dist;
}

// ─────────────────────────────────────────────────────────────────────────────
// DYNAMIC PROGRAMMING
// ─────────────────────────────────────────────────────────────────────────────

int knapsack01(const std::vector<int>& weights,
               const std::vector<int>& values,
               int capacity) {
    int n = (int)weights.size();
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int w = 0; w <= capacity; ++w) {
            dp[i][w] = dp[i-1][w];
            if (weights[i-1] <= w)
                dp[i][w] = std::max(dp[i][w], dp[i-1][w - weights[i-1]] + values[i-1]);
        }
    return dp[n][capacity];
}

// O(n log n) patience-sorting LIS.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    std::vector<int> tails;
    for (int n : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), n);
        if (it == tails.end()) tails.push_back(n);
        else                   *it = n;
    }
    return (int)tails.size();
}

int coinChangeMinCoins(const std::vector<int>& coins, int amount) {
    const int INF = INT_MAX / 2;
    std::vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int coin : coins)
        for (int i = coin; i <= amount; ++i)
            if (dp[i - coin] != INF)
                dp[i] = std::min(dp[i], dp[i - coin] + 1);
    return dp[amount] == INF ? -1 : dp[amount];
}

bool wordBreak(const std::string& s, const std::vector<std::string>& wordDict) {
    std::unordered_set<std::string> words(wordDict.begin(), wordDict.end());
    int n = (int)s.size();
    std::vector<bool> dp(n + 1, false);
    dp[0] = true;
    for (int i = 1; i <= n; ++i)
        for (int j = 0; j < i; ++j)
            if (dp[j] && words.count(s.substr(j, i - j))) { dp[i] = true; break; }
    return dp[n];
}

int climbingStairs(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; ++i) { int tmp = a + b; a = b; b = tmp; }
    return b;
}

// ─────────────────────────────────────────────────────────────────────────────
// GREEDY
// ─────────────────────────────────────────────────────────────────────────────

// jobs: vector of {id, deadline, profit}. Returns {jobs_done, total_profit}.
std::pair<int,int> jobSequencing(std::vector<std::tuple<int,int,int>> jobs) {
    std::sort(jobs.begin(), jobs.end(),
              [](auto& a, auto& b){ return std::get<2>(a) > std::get<2>(b); });
    int maxDeadline = 0;
    for (auto& [id, d, p] : jobs) maxDeadline = std::max(maxDeadline, d);
    std::vector<int> slot(maxDeadline + 1, -1);
    int count = 0, profit = 0;
    for (auto& [id, deadline, p] : jobs)
        for (int t = std::min(deadline, maxDeadline); t > 0; --t)
            if (slot[t] == -1) { slot[t] = id; ++count; profit += p; break; }
    return {count, profit};
}

int minimumPlatforms(std::vector<int> arrivals, std::vector<int> departures) {
    std::sort(arrivals.begin(), arrivals.end());
    std::sort(departures.begin(), departures.end());
    int platforms = 0, maxPlatforms = 0;
    int i = 0, j = 0, n = (int)arrivals.size();
    while (i < n) {
        if (arrivals[i] <= departures[j]) { ++platforms; ++i; }
        else                               { --platforms; ++j; }
        maxPlatforms = std::max(maxPlatforms, platforms);
    }
    return maxPlatforms;
}

// Activity selection — returns indices of selected activities.
std::vector<int> activitySelection(std::vector<int> start, std::vector<int> finish) {
    int n = (int)start.size();
    std::vector<int> idx(n);
    std::iota(idx.begin(), idx.end(), 0);
    std::sort(idx.begin(), idx.end(), [&](int a, int b){ return finish[a] < finish[b]; });
    std::vector<int> selected = {idx[0]};
    int lastFinish = finish[idx[0]];
    for (int i = 1; i < n; ++i)
        if (start[idx[i]] >= lastFinish) {
            selected.push_back(idx[i]);
            lastFinish = finish[idx[i]];
        }
    return selected;
}

int minCostConnectRopes(std::vector<int> ropes) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> pq(ropes.begin(), ropes.end());
    int total = 0;
    while (pq.size() > 1) {
        int a = pq.top(); pq.pop();
        int b = pq.top(); pq.pop();
        total += a + b;
        pq.push(a + b);
    }
    return total;
}

// ─────────────────────────────────────────────────────────────────────────────
// HEAPS
// ─────────────────────────────────────────────────────────────────────────────

std::vector<int> kLargestElements(std::vector<int> nums, int k) {
    std::partial_sort(nums.begin(), nums.begin() + k, nums.end(), std::greater<int>());
    return std::vector<int>(nums.begin(), nums.begin() + k);
}

class MedianFinder {
    std::priority_queue<int> lo;                              // max-heap
    std::priority_queue<int, std::vector<int>, std::greater<int>> hi; // min-heap
public:
    void addNum(int num) {
        lo.push(num);
        hi.push(lo.top()); lo.pop();
        if (hi.size() > lo.size()) { lo.push(hi.top()); hi.pop(); }
    }
    double findMedian() {
        if (lo.size() > hi.size()) return lo.top();
        return (lo.top() + hi.top()) / 2.0;
    }
};

std::vector<int> mergeKSortedArrays(const std::vector<std::vector<int>>& arrays) {
    using T = std::tuple<int,int,int>;  // {value, array_idx, element_idx}
    std::priority_queue<T, std::vector<T>, std::greater<T>> pq;
    for (int i = 0; i < (int)arrays.size(); ++i)
        if (!arrays[i].empty())
            pq.push({arrays[i][0], i, 0});
    std::vector<int> result;
    while (!pq.empty()) {
        auto [val, i, j] = pq.top(); pq.pop();
        result.push_back(val);
        if (j + 1 < (int)arrays[i].size())
            pq.push({arrays[i][j+1], i, j+1});
    }
    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
// BIT MANIPULATION
// ─────────────────────────────────────────────────────────────────────────────

// dp[i] = number of 1-bits in i, for 0..n.
std::vector<int> countBits(int n) {
    std::vector<int> dp(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        dp[i] = dp[i >> 1] + (i & 1);
    return dp;
}

int missingNumber(const std::vector<int>& nums) {
    int n = (int)nums.size();
    return n * (n + 1) / 2 - std::accumulate(nums.begin(), nums.end(), 0);
}

uint32_t reverseBits(uint32_t n) {
    uint32_t result = 0;
    for (int i = 0; i < 32; ++i) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}

int xorAllSubsets(const std::vector<int>& nums) {
    if (nums.size() == 1) return nums[0];
    int result = 0;
    for (int n : nums) result ^= n;
    return result;
}

bool powerOfTwo(int n) {
    return n > 0 && (n & (n - 1)) == 0;
}

// ─────────────────────────────────────────────────────────────────────────────
// TRIE
// ─────────────────────────────────────────────────────────────────────────────

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEnd = false;
    ~TrieNode() { for (auto& [_, c] : children) delete c; }
};

class Trie {
protected:
    TrieNode* root;
public:
    Trie() : root(new TrieNode()) {}
    virtual ~Trie() { delete root; }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char ch : word)
            node = node->children.emplace(ch, new TrieNode()).first->second;
        node->isEnd = true;
    }

    bool search(const std::string& word) const {
        TrieNode* node = root;
        for (char ch : word) {
            if (!node->children.count(ch)) return false;
            node = node->children.at(ch);
        }
        return node->isEnd;
    }

    bool startsWith(const std::string& prefix) const {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (!node->children.count(ch)) return false;
            node = node->children.at(ch);
        }
        return true;
    }
};

class Autocomplete : public Trie {
    void dfs(TrieNode* node, std::string& path, std::vector<std::string>& results) const {
        if (node->isEnd) results.push_back(path);
        for (auto& [ch, child] : node->children) {
            path += ch;
            dfs(child, path, results);
            path.pop_back();
        }
    }
public:
    std::vector<std::string> suggestions(const std::string& prefix) const {
        TrieNode* node = root;
        for (char ch : prefix) {
            if (!node->children.count(ch)) return {};
            node = node->children.at(ch);
        }
        std::vector<std::string> results;
        std::string path = prefix;
        dfs(node, path, results);
        return results;
    }
};

class LongestPrefixMatch : public Trie {
public:
    std::string longestPrefix(const std::string& query) const {
        TrieNode* node = root;
        std::string prefix;
        for (char ch : query) {
            if (!node->children.count(ch)) break;
            node = node->children.at(ch);
            prefix += ch;
            if (node->isEnd) return prefix;
        }
        return prefix;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// HUFFMAN ENCODING
// ─────────────────────────────────────────────────────────────────────────────

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char c, int f, HuffmanNode* l = nullptr, HuffmanNode* r = nullptr)
        : ch(c), freq(f), left(l), right(r) {}
    bool isLeaf() const { return !left && !right; }
};

struct HuffmanCmp {
    bool operator()(HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; }
};

void buildCodes(HuffmanNode* node, const std::string& code,
                std::unordered_map<char, std::string>& codes) {
    if (!node) return;
    if (node->isLeaf()) { codes[node->ch] = code.empty() ? "0" : code; return; }
    buildCodes(node->left,  code + "0", codes);
    buildCodes(node->right, code + "1", codes);
}

void freeHuffman(HuffmanNode* node) {
    if (!node) return;
    freeHuffman(node->left);
    freeHuffman(node->right);
    delete node;
}

std::unordered_map<char, std::string> huffmanEncoding(const std::string& text) {
    std::unordered_map<char, int> freq;
    for (char c : text) ++freq[c];
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, HuffmanCmp> pq;
    for (auto& [c, f] : freq) pq.push(new HuffmanNode(c, f));
    while (pq.size() > 1) {
        HuffmanNode* l = pq.top(); pq.pop();
        HuffmanNode* r = pq.top(); pq.pop();
        pq.push(new HuffmanNode('\0', l->freq + r->freq, l, r));
    }
    std::unordered_map<char, std::string> codes;
    buildCodes(pq.top(), "", codes);
    freeHuffman(pq.top());
    return codes;
}
