/**
 * test_algorithms.cpp
 * C++ test suite for algorithms.h
 * Mirrors every test in test_algorithms.py.
 *
 * Build & run:
 *   g++ -std=c++17 -O2 -Wall -o test_algorithms test_algorithms.cpp && ./test_algorithms
 */

#include "algorithms.h"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
// Minimal test harness
// ─────────────────────────────────────────────────────────────────────────────

static int total = 0, passed = 0, failed = 0;
static std::string currentSuite;

void suite(const std::string& name) {
    currentSuite = name;
    std::cout << "\n[" << name << "]\n";
}

template<typename T>
void expect_eq(const std::string& label, const T& got, const T& want) {
    ++total;
    if (got == want) {
        ++passed;
        std::cout << "  PASS  " << label << "\n";
    } else {
        ++failed;
        std::cout << "  FAIL  " << label << "\n";
    }
}

void expect_true(const std::string& label, bool condition) {
    expect_eq(label, condition, true);
}
void expect_false(const std::string& label, bool condition) {
    expect_eq(label, condition, false);
}

void printSummary() {
    std::cout << "\n════════════════════════════════\n"
              << "Results: " << passed << "/" << total << " passed";
    if (failed) std::cout << "  (" << failed << " FAILED)";
    std::cout << "\n════════════════════════════════\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────

// Build a BST and locate a node by value.
TreeNode* findNode(TreeNode* root, int val) {
    while (root) {
        if (root->val == val) return root;
        root = (val < root->val) ? root->left : root->right;
    }
    return nullptr;
}

// Build a simple BST from a sorted vector for LCA tests.
TreeNode* insertBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left  = insertBST(root->left,  val);
    else                 root->right = insertBST(root->right, val);
    return root;
}

bool sameTree(TreeNode* a, TreeNode* b) {
    if (!a && !b) return true;
    if (!a || !b || a->val != b->val) return false;
    return sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

// ═════════════════════════════════════════════════════════════════════════════
// ARRAYS
// ═════════════════════════════════════════════════════════════════════════════

void test_twoSum() {
    suite("TwoSum");
    expect_eq("basic",            twoSum({2,7,11,15}, 9),  std::vector<int>({0,1}));
    expect_eq("non_adjacent",     twoSum({3,2,4}, 6),      std::vector<int>({1,2}));
    expect_eq("duplicate_values", twoSum({3,3}, 6),        std::vector<int>({0,1}));
    expect_eq("negatives",        twoSum({-1,-2,-3,-4},-6),std::vector<int>({1,3}));
    expect_eq("no_solution",      twoSum({1,2,3}, 100),    std::vector<int>({}));
}

void test_maxSubarray() {
    suite("MaxSubarray");
    expect_eq("mixed",        maxSubarray({-2,1,-3,4,-1,2,1,-5,4}), 6);
    expect_eq("all_positive", maxSubarray({1,2,3,4}),                10);
    expect_eq("all_negative", maxSubarray({-3,-1,-2}),              -1);
    expect_eq("single",       maxSubarray({42}),                     42);
    expect_eq("large_drop",   maxSubarray({5,-100,5}),               5);
}

void test_mergeIntervals() {
    suite("MergeIntervals");
    expect_eq("overlapping",  mergeIntervals({{1,3},{2,6},{8,10},{15,18}}),
              std::vector<std::vector<int>>({{1,6},{8,10},{15,18}}));
    expect_eq("touching",     mergeIntervals({{1,4},{4,5}}),
              std::vector<std::vector<int>>({{1,5}}));
    expect_eq("no_overlap",   mergeIntervals({{1,2},{3,4}}),
              std::vector<std::vector<int>>({{1,2},{3,4}}));
    expect_eq("all_overlap",  mergeIntervals({{1,10},{2,5},{3,7}}),
              std::vector<std::vector<int>>({{1,10}}));
    expect_eq("single",       mergeIntervals({{1,5}}),
              std::vector<std::vector<int>>({{1,5}}));
}

void test_minRotatedArray() {
    suite("MinRotatedArray");
    expect_eq("rotated",     minRotatedArray({3,4,5,1,2}), 1);
    expect_eq("rotated_once",minRotatedArray({2,1}),        1);
    expect_eq("not_rotated", minRotatedArray({1,2,3,4,5}), 1);
    expect_eq("single",      minRotatedArray({7}),          7);
    expect_eq("two_elements",minRotatedArray({5,1}),        1);
}

void test_trappingRainwater() {
    suite("TrappingRainwater");
    expect_eq("classic",   trappingRainwater({0,1,0,2,1,0,1,3,2,1,2,1}), 6);
    expect_eq("symmetric", trappingRainwater({4,2,0,3,2,5}),              9);
    expect_eq("no_water",  trappingRainwater({1,2,3,4}),                  0);
    expect_eq("flat",      trappingRainwater({3,3,3}),                    0);
    expect_eq("valley",    trappingRainwater({3,0,3}),                    3);
}

void test_threeSum() {
    suite("ThreeSum");
    auto r1 = threeSum({-1,0,1,2,-1,-4});
    expect_true("contains_-1_-1_2", std::find(r1.begin(),r1.end(),std::vector<int>({-1,-1,2})) != r1.end());
    expect_true("contains_-1_0_1",  std::find(r1.begin(),r1.end(),std::vector<int>({-1,0,1}))  != r1.end());
    expect_eq("all_zeros",   threeSum({0,0,0}),   std::vector<std::vector<int>>({{0,0,0}}));
    expect_eq("no_solution", threeSum({1,2,3}),   std::vector<std::vector<int>>({}));
    expect_eq("dedup",       threeSum({-2,0,0,2,2}), std::vector<std::vector<int>>({{-2,0,2}}));
}

void test_productExceptSelf() {
    suite("ProductExceptSelf");
    expect_eq("basic",    productExceptSelf({1,2,3,4}),    std::vector<int>({24,12,8,6}));
    expect_eq("one_zero", productExceptSelf({1,0,3,4}),    std::vector<int>({0,12,0,0}));
    expect_eq("two_zeros",productExceptSelf({0,0,3}),      std::vector<int>({0,0,0}));
    expect_eq("neg",      productExceptSelf({-1,1,0,-3,3}),std::vector<int>({0,0,9,0,0}));
}

void test_containerWithMostWater() {
    suite("ContainerWithMostWater");
    expect_eq("classic",    containerWithMostWater({1,8,6,2,5,4,8,3,7}), 49);
    expect_eq("two",        containerWithMostWater({1,1}),                 1);
    expect_eq("descending", containerWithMostWater({5,4,3,2,1}),           6);
    expect_eq("ascending",  containerWithMostWater({1,2,3,4,5}),           6);
}

void test_removeDuplicatesSorted() {
    suite("RemoveDuplicatesSorted");
    {
        std::vector<int> v = {1,1,2};
        int k = removeDuplicatesSorted(v);
        expect_eq("basic_len", k, 2);
        expect_eq("basic_vals", std::vector<int>(v.begin(), v.begin()+k), std::vector<int>({1,2}));
    }
    {
        std::vector<int> v = {1,2,3};
        expect_eq("no_dups", removeDuplicatesSorted(v), 3);
    }
    {
        std::vector<int> v = {7,7,7,7};
        int k = removeDuplicatesSorted(v);
        expect_eq("all_same_len", k, 1);
        expect_eq("all_same_val", v[0], 7);
    }
    {
        std::vector<int> v;
        expect_eq("empty", removeDuplicatesSorted(v), 0);
    }
}

void test_rotateArray() {
    suite("RotateArray");
    {
        std::vector<int> v = {1,2,3,4,5,6,7};
        rotateArray(v, 3);
        expect_eq("basic", v, std::vector<int>({5,6,7,1,2,3,4}));
    }
    {
        std::vector<int> v = {1,2,3};
        rotateArray(v, 4);
        expect_eq("k_gt_n", v, std::vector<int>({3,1,2}));
    }
    {
        std::vector<int> v = {1,2,3};
        rotateArray(v, 0);
        expect_eq("k_zero", v, std::vector<int>({1,2,3}));
    }
    {
        std::vector<int> v = {99};
        rotateArray(v, 5);
        expect_eq("single", v, std::vector<int>({99}));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// STRINGS
// ═════════════════════════════════════════════════════════════════════════════

void test_strings() {
    suite("AnagramCheck");
    expect_true ("anagram",           anagramCheck("anagram","nagaram"));
    expect_false("not_anagram",       anagramCheck("rat","car"));
    expect_false("different_lengths", anagramCheck("ab","abc"));
    expect_true ("empty",             anagramCheck("",""));

    suite("LongestSubstringNoRepeat");
    expect_eq("classic",    longestSubstringNoRepeat("abcabcbb"), 3);
    expect_eq("all_same",   longestSubstringNoRepeat("bbbbb"),    1);
    expect_eq("mixed",      longestSubstringNoRepeat("pwwkew"),   3);
    expect_eq("empty",      longestSubstringNoRepeat(""),         0);
    expect_eq("all_unique", longestSubstringNoRepeat("abcdef"),   6);

    suite("LongestPalindromicSubstring");
    {
        auto r = longestPalindromicSubstring("babad");
        expect_true("odd", r == "bab" || r == "aba");
    }
    expect_eq("even",         longestPalindromicSubstring("cbbd"),    std::string("bb"));
    expect_eq("entire",       longestPalindromicSubstring("racecar"), std::string("racecar"));
    expect_eq("single",       longestPalindromicSubstring("a"),       std::string("a"));

    suite("ReverseWords");
    expect_eq("basic",       reverseWords("the sky is blue"),    std::string("blue is sky the"));
    expect_eq("extra_spaces",reverseWords("  hello world  "),   std::string("world hello"));
    expect_eq("single_word", reverseWords("python"),             std::string("python"));

    suite("Atoi");
    expect_eq("positive",          myAtoi("42"),            42);
    expect_eq("negative",          myAtoi("   -42"),       -42);
    expect_eq("trailing_nondigit", myAtoi("4193 with words"), 4193);
    expect_eq("non_numeric",       myAtoi("words and 987"),    0);
    expect_eq("overflow_pos",      myAtoi("99999999999"),  INT_MAX);
    expect_eq("overflow_neg",      myAtoi("-99999999999"), INT_MIN);
    expect_eq("empty",             myAtoi(""),               0);
    expect_eq("plus_sign",         myAtoi("+100"),          100);

    suite("ValidPalindrome");
    expect_true ("classic",     validPalindrome("A man, a plan, a canal: Panama"));
    expect_false("not_palindrome", validPalindrome("race a car"));
    expect_true ("empty",       validPalindrome(""));
    expect_true ("single",      validPalindrome("a"));

    suite("CountAndSay");
    expect_eq("n1", countAndSay(1), std::string("1"));
    expect_eq("n2", countAndSay(2), std::string("11"));
    expect_eq("n3", countAndSay(3), std::string("21"));
    expect_eq("n4", countAndSay(4), std::string("1211"));
    expect_eq("n5", countAndSay(5), std::string("111221"));

    suite("GroupAnagrams");
    {
        auto groups = groupAnagrams({"eat","tea","tan","ate","nat","bat"});
        expect_eq("count", (int)groups.size(), 3);
    }
    {
        auto groups = groupAnagrams({"abc","def"});
        expect_eq("all_unique", (int)groups.size(), 2);
    }

    suite("LongestCommonPrefix");
    expect_eq("basic",      longestCommonPrefix({"flower","flow","flight"}), std::string("fl"));
    expect_eq("no_prefix",  longestCommonPrefix({"dog","racecar","car"}),    std::string(""));
    expect_eq("identical",  longestCommonPrefix({"abc","abc","abc"}),        std::string("abc"));
    expect_eq("single_word",longestCommonPrefix({"alone"}),                  std::string("alone"));
    expect_eq("empty_list", longestCommonPrefix({}),                         std::string(""));

    suite("StringCompression");
    {
        std::vector<char> c = {'a','a','b','b','c','c','c'};
        int len = stringCompression(c);
        expect_eq("length", len, 6);
        expect_eq("content", std::vector<char>(c.begin(), c.begin()+len),
                  std::vector<char>({'a','2','b','2','c','3'}));
    }
    {
        std::vector<char> c = {'a','b','c'};
        expect_eq("no_compress", stringCompression(c), 3);
    }
    {
        std::vector<char> c = {'a'};
        int len = stringCompression(c);
        expect_eq("single_len", len, 1);
        expect_eq("single_val", c[0], 'a');
    }
    {
        std::vector<char> c(12, 'a');
        int len = stringCompression(c);
        std::string s(c.begin(), c.begin() + len);
        expect_eq("long_run", s, std::string("a12"));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// LINKED LIST
// ═════════════════════════════════════════════════════════════════════════════

void test_linkedList() {
    suite("ReverseLinkedList");
    {
        auto* h = makeList({1,2,3,4,5});
        auto* r = reverseLinkedList(h);
        expect_eq("basic", toVector(r), std::vector<int>({5,4,3,2,1}));
        freeList(r);
    }
    {
        auto* h = makeList({1});
        auto* r = reverseLinkedList(h);
        expect_eq("single", toVector(r), std::vector<int>({1}));
        freeList(r);
    }
    expect_eq("empty", (ListNode*)nullptr, reverseLinkedList(nullptr));
    {
        auto* h = makeList({1,2});
        auto* r = reverseLinkedList(h);
        expect_eq("two", toVector(r), std::vector<int>({2,1}));
        freeList(r);
    }

    suite("DetectCycle");
    {
        auto* h = makeList({1,2,3});
        expect_false("no_cycle", detectCycle(h));
        freeList(h);
    }
    expect_false("empty", detectCycle(nullptr));
    {
        ListNode* n1 = new ListNode(1);
        ListNode* n2 = new ListNode(2);
        ListNode* n3 = new ListNode(3);
        n1->next = n2; n2->next = n3; n3->next = n2;
        expect_true("with_cycle", detectCycle(n1));
        n3->next = nullptr;  // break cycle before freeing
        freeList(n1);
    }
    {
        ListNode* n = new ListNode(1); n->next = n;
        expect_true("self_loop", detectCycle(n));
        n->next = nullptr; delete n;
    }

    suite("MergeTwoSortedLists");
    {
        auto* r = mergeTwoSortedLists(makeList({1,2,4}), makeList({1,3,4}));
        expect_eq("basic", toVector(r), std::vector<int>({1,1,2,3,4,4}));
        freeList(r);
    }
    {
        auto* r = mergeTwoSortedLists(nullptr, makeList({1,2}));
        expect_eq("l1_null", toVector(r), std::vector<int>({1,2}));
        freeList(r);
    }
    {
        auto* r = mergeTwoSortedLists(makeList({1,2}), nullptr);
        expect_eq("l2_null", toVector(r), std::vector<int>({1,2}));
        freeList(r);
    }
    expect_eq("both_null", (ListNode*)nullptr, mergeTwoSortedLists(nullptr, nullptr));
    {
        auto* r = mergeTwoSortedLists(makeList({1,2}), makeList({3,4}));
        expect_eq("disjoint", toVector(r), std::vector<int>({1,2,3,4}));
        freeList(r);
    }

    suite("MiddleOfLinkedList");
    {
        auto* h = makeList({1,2,3,4,5});
        expect_eq("odd_length", middleOfLinkedList(h)->val, 3);
        freeList(h);
    }
    {
        auto* h = makeList({1,2,3,4});
        expect_eq("even_length", middleOfLinkedList(h)->val, 3);
        freeList(h);
    }
    {
        auto* h = makeList({42});
        expect_eq("single", middleOfLinkedList(h)->val, 42);
        freeList(h);
    }

    suite("RemoveNthFromEnd");
    {
        auto* r = removeNthFromEnd(makeList({1,2,3,4,5}), 2);
        expect_eq("middle", toVector(r), std::vector<int>({1,2,3,5}));
        freeList(r);
    }
    {
        auto* r = removeNthFromEnd(makeList({1,2,3}), 3);
        expect_eq("remove_head", toVector(r), std::vector<int>({2,3}));
        freeList(r);
    }
    {
        auto* r = removeNthFromEnd(makeList({1,2,3}), 1);
        expect_eq("remove_tail", toVector(r), std::vector<int>({1,2}));
        freeList(r);
    }
    {
        auto* r = removeNthFromEnd(makeList({1}), 1);
        expect_eq("single_node", (ListNode*)nullptr, r);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// STACK
// ═════════════════════════════════════════════════════════════════════════════

void test_stack() {
    suite("InfixToPostfix");
    expect_eq("addition",    infixToPostfix("3 + 4"),            std::string("3 4 +"));
    expect_eq("precedence",  infixToPostfix("3 + 4 * 2"),        std::string("3 4 2 * +"));
    expect_eq("parens",      infixToPostfix("( 3 + 4 ) * 2"),    std::string("3 4 + 2 *"));
    expect_eq("complex",     infixToPostfix("5 + ( 1 + 2 ) * 4 - 3"), std::string("5 1 2 + 4 * + 3 -"));

    suite("NextGreaterElement");
    expect_eq("basic",      nextGreaterElement({4,1,2}),     std::vector<int>({-1,2,-1}));
    expect_eq("descending", nextGreaterElement({5,4,3,2,1}), std::vector<int>({-1,-1,-1,-1,-1}));
    expect_eq("ascending",  nextGreaterElement({1,2,3,4}),   std::vector<int>({2,3,4,-1}));
    expect_eq("single",     nextGreaterElement({7}),         std::vector<int>({-1}));

    suite("MinStack");
    {
        MinStack ms;
        ms.push(-2); ms.push(0); ms.push(-3);
        expect_eq("get_min_1", ms.getMin(), -3);
        ms.pop();
        expect_eq("top",       ms.top(),    0);
        expect_eq("get_min_2", ms.getMin(), -2);
    }
    {
        MinStack ms;
        ms.push(5); ms.push(10);
        expect_eq("min_after_larger", ms.getMin(), 5);
    }
    {
        MinStack ms;
        ms.push(42);
        expect_eq("single_top", ms.top(),    42);
        expect_eq("single_min", ms.getMin(), 42);
    }

    suite("LargestRectangleHistogram");
    expect_eq("classic",   largestRectangleHistogram({2,1,5,6,2,3}), 10);
    expect_eq("all_same",  largestRectangleHistogram({3,3,3,3}),      12);
    expect_eq("single",    largestRectangleHistogram({5}),             5);
    expect_eq("ascending", largestRectangleHistogram({1,2,3,4,5}),    9);
    expect_eq("valley",    largestRectangleHistogram({5,1,5}),         5);

    suite("ReverseFirstKQueue");
    {
        std::deque<int> q = {1,2,3,4,5};
        auto r = reverseFirstKQueue(q, 3);
        expect_eq("basic", std::vector<int>(r.begin(), r.end()), std::vector<int>({3,2,1,4,5}));
    }
    {
        std::deque<int> q = {1,2,3};
        auto r = reverseFirstKQueue(q, 3);
        expect_eq("k_eq_len", std::vector<int>(r.begin(), r.end()), std::vector<int>({3,2,1}));
    }
    {
        std::deque<int> q = {1,2,3};
        auto r = reverseFirstKQueue(q, 1);
        expect_eq("k_one", std::vector<int>(r.begin(), r.end()), std::vector<int>({1,2,3}));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// BINARY TREE
// ═════════════════════════════════════════════════════════════════════════════

void test_binaryTree() {
    suite("LevelOrderTraversal");
    {
        //       3
        //      / \
        //     9  20
        //        / \
        //       15   7
        TreeNode* root = new TreeNode(3);
        root->left = new TreeNode(9);
        root->right = new TreeNode(20);
        root->right->left  = new TreeNode(15);
        root->right->right = new TreeNode(7);
        expect_eq("basic", levelOrderTraversal(root),
                  std::vector<std::vector<int>>({{3},{9,20},{15,7}}));
        freeTree(root);
    }
    {
        TreeNode* root = new TreeNode(1);
        expect_eq("single", levelOrderTraversal(root),
                  std::vector<std::vector<int>>({{1}}));
        freeTree(root);
    }
    expect_eq("empty", levelOrderTraversal(nullptr),
              std::vector<std::vector<int>>({}));

    suite("MirrorTrees");
    {
        TreeNode* t1 = new TreeNode(1);
        t1->left = new TreeNode(2); t1->right = new TreeNode(3);
        TreeNode* t2 = new TreeNode(1);
        t2->left = new TreeNode(3); t2->right = new TreeNode(2);
        expect_true("mirror",     mirrorTrees(t1, t2));
        freeTree(t1); freeTree(t2);
    }
    {
        TreeNode* t1 = new TreeNode(1);
        t1->left = new TreeNode(2); t1->right = new TreeNode(3);
        TreeNode* t2 = new TreeNode(1);
        t2->left = new TreeNode(2); t2->right = new TreeNode(3);
        expect_false("not_mirror", mirrorTrees(t1, t2));
        freeTree(t1); freeTree(t2);
    }
    expect_true("both_null",  mirrorTrees(nullptr, nullptr));
    expect_false("one_null",  mirrorTrees(new TreeNode(1), nullptr));

    suite("LowestCommonAncestorBST");
    {
        TreeNode* root = nullptr;
        for (int v : {6,2,8,0,4,7,9,3,5}) root = insertBST(root, v);
        TreeNode* p = findNode(root, 2);
        TreeNode* q = findNode(root, 8);
        expect_eq("lca_root",   lowestCommonAncestorBST(root, p, q)->val, 6);
        p = findNode(root, 2);
        q = findNode(root, 4);
        expect_eq("lca_subtree",lowestCommonAncestorBST(root, p, q)->val, 2);
        freeTree(root);
    }

    suite("SerializeDeserialize");
    {
        TreeNode* root = new TreeNode(1);
        root->left = new TreeNode(2); root->right = new TreeNode(3);
        root->right->left = new TreeNode(4); root->right->right = new TreeNode(5);
        std::string s = serialize(root);
        TreeNode* restored = deserialize(s);
        expect_true("roundtrip", sameTree(root, restored));
        freeTree(root); freeTree(restored);
    }
    expect_true("null_roundtrip", deserialize(serialize(nullptr)) == nullptr);
    {
        TreeNode* root = new TreeNode(42);
        TreeNode* restored = deserialize(serialize(root));
        expect_eq("single_val",  restored->val,   42);
        expect_true("single_no_left",  restored->left  == nullptr);
        expect_true("single_no_right", restored->right == nullptr);
        freeTree(root); freeTree(restored);
    }

    suite("ValidateBST");
    {
        TreeNode* root = nullptr;
        for (int v : {5,3,7,1,4,6,8}) root = insertBST(root, v);
        expect_true("valid", validateBST(root));
        freeTree(root);
    }
    {
        // Invalid: right child has smaller value
        TreeNode* root = new TreeNode(5);
        root->left = new TreeNode(1);
        root->right = new TreeNode(4);
        root->right->left = new TreeNode(3);
        root->right->right = new TreeNode(6);
        expect_false("invalid", validateBST(root));
        freeTree(root);
    }
    expect_true("single_node", validateBST(new TreeNode(1)));
    {
        TreeNode* root = new TreeNode(2);
        root->left = new TreeNode(2);
        expect_false("duplicate", validateBST(root));
        freeTree(root);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// GRAPHS
// ═════════════════════════════════════════════════════════════════════════════

void test_graphs() {
    suite("TopologicalSort");
    {
        auto o = topologicalSort(4, {{0,1},{1,2},{2,3}});
        expect_eq("basic_dag", o, std::vector<int>({0,1,2,3}));
    }
    expect_eq("cycle_empty", topologicalSort(3, {{0,1},{1,2},{2,0}}),
              std::vector<int>({}));
    {
        auto o = topologicalSort(3, {});
        expect_eq("no_edges_size", (int)o.size(), 3);
    }

    suite("DetectCycleDirected");
    expect_true ("has_cycle",  detectCycleDirected(3, {{0,1},{1,2},{2,0}}));
    expect_false("no_cycle",   detectCycleDirected(3, {{0,1},{1,2}}));
    expect_true ("self_loop",  detectCycleDirected(2, {{0,0}}));
    expect_false("no_edges",   detectCycleDirected(5, {}));

    suite("NumberOfIslands");
    expect_eq("three", numberOfIslands({{'1','1','0'},{'0','1','0'},{'1','0','1'}}), 3);
    expect_eq("one",   numberOfIslands({{'1','1','1'},{'1','1','0'},{'0','1','1'}}), 1);
    expect_eq("none",  numberOfIslands({{'0','0'},{'0','0'}}),                      0);
    expect_eq("all",   numberOfIslands({{'1','1'},{'1','1'}}),                      1);
    expect_eq("empty", numberOfIslands({}),                                         0);

    suite("ShortestPathUnweighted");
    {
        std::vector<std::vector<int>> g = {{1,2},{0,3},{0,3},{1,2}};
        expect_eq("basic", shortestPathUnweighted(g, 0), std::vector<int>({0,1,1,2}));
    }
    {
        std::vector<std::vector<int>> g = {{1},{0},{},{}};
        auto d = shortestPathUnweighted(g, 0);
        expect_eq("disconnected_0",  d[0],  0);
        expect_eq("disconnected_1",  d[1],  1);
        expect_eq("disconnected_2",  d[2], -1);
        expect_eq("disconnected_3",  d[3], -1);
    }

    suite("BipartiteCheck");
    expect_true ("bipartite",  bipartiteCheck({{1,3},{0,2},{1,3},{0,2}}));
    expect_false("odd_cycle",  bipartiteCheck({{1,2},{0,2},{0,1}}));
    expect_true ("empty_graph",bipartiteCheck({{},{},{}}));
    expect_true ("single_node",bipartiteCheck({{}}));

    suite("TravelingSalesman");
    // ── testTSP (mirrors Java JUnit test exactly) ──────────────────────────
    {
        std::vector<std::vector<int>> dist = {
            {0, 10, 15, 20},
            {10,  0, 35, 25},
            {15, 35,  0, 30},
            {20, 25, 30,  0},
        };
        // Known optimal tour cost = 80
        expect_eq("testTSP_known_optimal", travelingSalesman(dist), 80);
    }
    // ── additional cases ───────────────────────────────────────────────────
    {
        // Two cities: only one round trip possible — A->B->A
        std::vector<std::vector<int>> dist = {{0, 5}, {5, 0}};
        expect_eq("two_cities", travelingSalesman(dist), 10);
    }
    {
        // Three cities, uniform cost: any tour = 3 * cost
        std::vector<std::vector<int>> dist = {{0,1,1},{1,0,1},{1,1,0}};
        expect_eq("three_cities_uniform", travelingSalesman(dist), 3);
    }
    {
        // Asymmetric costs — greedy would pick wrong path;
        // optimal is 0->2->3->1->0 = 1+2+3+4 = 10
        std::vector<std::vector<int>> dist = {
            { 0, 10,  1, 99},
            {10,  0, 99,  4},
            { 1, 99,  0,  2},
            {99,  4,  2,  0},
        };
        expect_eq("asymmetric_greedy_trap", travelingSalesman(dist), 17);
    }
    {
        // All edges equal weight w — optimal tour visits each city once = n*w
        int n = 5, w = 7;
        std::vector<std::vector<int>> dist(n, std::vector<int>(n, w));
        for (int i = 0; i < n; ++i) dist[i][i] = 0;
        expect_eq("uniform_5_cities", travelingSalesman(dist), n * w);
    }

    suite("ConnectedCitiesShortestPath");

    // ── testDijkstra — direct port of the Java JUnit test ─────────────────
    // Builds the same undirected weighted graph via adjacency list of Edge objects
    // and verifies assertArrayEquals({0,2,6,1,7}, dijkstra(n, graph, 0)).
    {
        int n = 5;
        std::vector<std::vector<Edge>> graph(n);
        graph[0].push_back(Edge(1, 2));
        graph[1].push_back(Edge(0, 2));
        graph[1].push_back(Edge(2, 4));
        graph[2].push_back(Edge(1, 4));
        graph[0].push_back(Edge(3, 1));
        graph[3].push_back(Edge(0, 1));
        graph[3].push_back(Edge(4, 7));
        graph[4].push_back(Edge(3, 7));
        graph[2].push_back(Edge(4, 1));
        graph[4].push_back(Edge(2, 1));

        auto dist = connectedCitiesShortestPath(n, graph, 0);
        // assertArrayEquals(new int[]{0, 2, 6, 1, 7}, dist)
        expect_eq("testDijkstra_src",    dist[0], 0);
        expect_eq("testDijkstra_node1",  dist[1], 2);
        expect_eq("testDijkstra_node2",  dist[2], 6);
        expect_eq("testDijkstra_node3",  dist[3], 1);
        expect_eq("testDijkstra_node4",  dist[4], 7);
    }

    // ── additional adjacency-list cases ───────────────────────────────────
    {
        // Source node has distance 0 to itself.
        std::vector<std::vector<Edge>> graph(1);
        auto dist = connectedCitiesShortestPath(1, graph, 0);
        expect_eq("single_node_src", dist[0], 0);
    }
    {
        // Node with no edges to it is unreachable (INT_MAX).
        std::vector<std::vector<Edge>> graph(3);
        graph[0].push_back(Edge(1, 5));
        graph[1].push_back(Edge(0, 5));
        // node 2 is an island
        auto dist = connectedCitiesShortestPath(3, graph, 0);
        expect_eq("island_node_unreachable", dist[2], INT_MAX);
    }
    {
        // Shortest path prefers lighter edge over fewer hops.
        // 0 --(10)--> 1
        // 0 --(1)-->  2 --(1)--> 1   => shortest to 1 is 2
        std::vector<std::vector<Edge>> graph(3);
        graph[0].push_back(Edge(1, 10));
        graph[1].push_back(Edge(0, 10));
        graph[0].push_back(Edge(2, 1));
        graph[2].push_back(Edge(0, 1));
        graph[2].push_back(Edge(1, 1));
        graph[1].push_back(Edge(2, 1));
        auto dist = connectedCitiesShortestPath(3, graph, 0);
        expect_eq("lighter_path_node1", dist[1], 2);
        expect_eq("lighter_path_node2", dist[2], 1);
    }
    {
        // All nodes equidistant from source in a star topology.
        int n = 5;
        std::vector<std::vector<Edge>> graph(n);
        for (int i = 1; i < n; ++i) {
            graph[0].push_back(Edge(i, 3));
            graph[i].push_back(Edge(0, 3));
        }
        auto dist = connectedCitiesShortestPath(n, graph, 0);
        expect_eq("star_src", dist[0], 0);
        for (int i = 1; i < n; ++i)
            expect_eq("star_node_" + std::to_string(i), dist[i], 3);
    }

    // ── edge-list overload (original tests kept) ───────────────────────────
    {
        std::vector<std::tuple<int,int,int>> edges = {{0,1,4},{0,2,1},{2,1,2},{1,3,1}};
        auto d = connectedCitiesShortestPath(4, edges, 0);
        expect_eq("edgelist_src",   (int)d[0], 0);
        expect_eq("edgelist_node1", (int)d[1], 3);
        expect_eq("edgelist_node2", (int)d[2], 1);
        expect_eq("edgelist_node3", (int)d[3], 4);
    }
    {
        auto d = connectedCitiesShortestPath(3, std::vector<std::tuple<int,int,int>>{{0,1,2}}, 0);
        expect_eq("edgelist_unreachable", std::isinf(d[2]), true);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// DYNAMIC PROGRAMMING
// ═════════════════════════════════════════════════════════════════════════════

void test_dp() {
    suite("Knapsack01");
    expect_eq("classic",           knapsack01({1,3,4,5},{1,4,5,7},7), 9);
    expect_eq("zero_capacity",     knapsack01({1,2},{3,4},0),          0);
    expect_eq("single_fits",       knapsack01({2},{5},3),              5);
    expect_eq("single_too_heavy",  knapsack01({5},{10},3),             0);
    expect_eq("all_fit",           knapsack01({1,1,1},{1,2,3},10),     6);

    suite("LongestIncreasingSubsequence");
    expect_eq("classic",    longestIncreasingSubsequence({10,9,2,5,3,7,101,18}), 4);
    expect_eq("sorted",     longestIncreasingSubsequence({1,2,3,4,5}),           5);
    expect_eq("descending", longestIncreasingSubsequence({5,4,3,2,1}),           1);
    expect_eq("single",     longestIncreasingSubsequence({7}),                   1);
    expect_eq("duplicates", longestIncreasingSubsequence({1,3,3,5}),             3);

    suite("CoinChangeMinCoins");
    expect_eq("classic",      coinChangeMinCoins({1,5,11},15),  3);
    expect_eq("exact_coin",   coinChangeMinCoins({1,5,10},10),  1);
    expect_eq("impossible",   coinChangeMinCoins({2},3),        -1);
    expect_eq("zero_amount",  coinChangeMinCoins({1,2,5},0),    0);
    expect_eq("greedy_fails", coinChangeMinCoins({1,3,4,5},6),  2);

    suite("WordBreak");
    expect_true ("basic_true",   wordBreak("leetcode",{"leet","code"}));
    expect_false("basic_false",  wordBreak("catsandog",{"cats","dog","sand","and","cat"}));
    expect_true ("repeated",     wordBreak("aaaaaa",{"a","aa","aaa"}));
    expect_true ("empty_string", wordBreak("",{"anything"}));

    suite("ClimbingStairs");
    expect_eq("n1",  climbingStairs(1),  1);
    expect_eq("n2",  climbingStairs(2),  2);
    expect_eq("n3",  climbingStairs(3),  3);
    expect_eq("n10", climbingStairs(10), 89);
}

// ═════════════════════════════════════════════════════════════════════════════
// GREEDY
// ═════════════════════════════════════════════════════════════════════════════

void test_greedy() {
    suite("JobSequencing");
    {
        std::vector<std::tuple<int,int,int>> jobs = {{1,2,100},{2,1,50},{3,2,10},{4,1,20},{5,3,30}};
        auto [count, profit] = jobSequencing(jobs);
        expect_eq("profit", profit, 180);
        expect_eq("count",  count,  3);
    }
    {
        auto [c, p] = jobSequencing(std::vector<std::tuple<int,int,int>>{{{1,1,50}}});
        expect_eq("single_count",  c, 1);
        expect_eq("single_profit", p, 50);
    }
    {
        auto [c, p] = jobSequencing(std::vector<std::tuple<int,int,int>>{{{1,1,10},{2,1,20},{3,1,30}}});
        expect_eq("same_deadline_count",  c, 1);
        expect_eq("same_deadline_profit", p, 30);
    }

    suite("MinimumPlatforms");
    expect_eq("classic",     minimumPlatforms({900,940,950,1100,1500,1800},{910,1200,1120,1130,1900,2000}), 3);
    expect_eq("no_overlap",  minimumPlatforms({100,200},{150,250}), 1);
    expect_eq("full_overlap",minimumPlatforms({100,100,100},{200,200,200}), 3);

    suite("ActivitySelection");
    {
        std::vector<int> s = {1,3,0,5,8,5};
        std::vector<int> f = {2,4,6,7,9,9};
        auto selected = activitySelection(s, f);
        expect_true("min_3", (int)selected.size() >= 3);
        for (int i = 0; i + 1 < (int)selected.size(); ++i)
            expect_true("no_overlap", s[selected[i+1]] >= f[selected[i]]);
    }
    {
        auto r = activitySelection({1},{2});
        expect_eq("single", r, std::vector<int>({0}));
    }
    {
        auto r = activitySelection({0,0,0},{10,10,10});
        expect_eq("all_overlap", (int)r.size(), 1);
    }

    suite("MinCostConnectRopes");
    expect_eq("classic",    minCostConnectRopes({4,3,2,6}), 29);
    expect_eq("two_ropes",  minCostConnectRopes({1,2}),      3);
    expect_eq("single",     minCostConnectRopes({5}),        0);
    expect_eq("equal_ropes",minCostConnectRopes({1,1,1,1}),  8);
}

// ═════════════════════════════════════════════════════════════════════════════
// HEAPS
// ═════════════════════════════════════════════════════════════════════════════

void test_heaps() {
    suite("KLargestElements");
    expect_eq("basic",    kLargestElements({3,1,4,1,5,9,2,6},3), std::vector<int>({9,6,5}));
    expect_eq("k_one",    kLargestElements({5,3,8,1},1),         std::vector<int>({8}));
    expect_eq("negatives",kLargestElements({-1,-5,-3},2),        std::vector<int>({-1,-3}));

    suite("MedianFinder");
    {
        MedianFinder mf;
        for (int n : {1,2,3}) mf.addNum(n);
        expect_eq("odd", mf.findMedian(), 2.0);
    }
    {
        MedianFinder mf;
        mf.addNum(1); mf.addNum(2);
        expect_eq("even", mf.findMedian(), 1.5);
    }
    {
        MedianFinder mf;
        mf.addNum(5);
        expect_eq("single", mf.findMedian(), 5.0);
    }
    {
        MedianFinder mf;
        for (int n : {5,4,3,2,1}) mf.addNum(n);
        expect_eq("reverse", mf.findMedian(), 3.0);
    }
    {
        MedianFinder mf;
        for (int n : {2,2,2}) mf.addNum(n);
        expect_eq("duplicates", mf.findMedian(), 2.0);
    }

    suite("MergeKSortedArrays");
    {
        std::vector<int> expected(9);
        std::iota(expected.begin(), expected.end(), 1);
        expect_eq("basic", mergeKSortedArrays({{1,4,7},{2,5,8},{3,6,9}}), expected);
    }
    expect_eq("single_array",   mergeKSortedArrays({{1,2,3}}),          std::vector<int>({1,2,3}));
    expect_eq("empty_arrays",   mergeKSortedArrays({{},{1},{}}),         std::vector<int>({1}));
    expect_eq("all_empty",      mergeKSortedArrays({{},{},{}}),           std::vector<int>({}));
    expect_eq("diff_lengths",   mergeKSortedArrays({{1},{1,2,3},{2,4}}), std::vector<int>({1,1,2,2,3,4}));
}

// ═════════════════════════════════════════════════════════════════════════════
// BIT MANIPULATION
// ═════════════════════════════════════════════════════════════════════════════

void test_bitManipulation() {
    suite("CountBits");
    expect_eq("basic", countBits(5), std::vector<int>({0,1,1,2,1,2}));
    expect_eq("zero",  countBits(0), std::vector<int>({0}));
    {
        auto r = countBits(8);
        for (int exp = 0; exp < 4; ++exp)
            expect_eq("power2_" + std::to_string(exp), r[1<<exp], 1);
    }

    suite("MissingNumber");
    expect_eq("basic",        missingNumber({3,0,1}), 2);
    expect_eq("missing_last", missingNumber({0,1,2}), 3);
    expect_eq("missing_first",missingNumber({1,2,3}), 0);
    expect_eq("single_0",     missingNumber({0}),     1);
    expect_eq("single_1",     missingNumber({1}),     0);

    suite("ReverseBits");
    expect_eq("known",   reverseBits(43261596u), 964176192u);
    expect_eq("zero",    reverseBits(0u),        0u);
    expect_eq("all_ones",reverseBits(0xFFFFFFFFu),0xFFFFFFFFu);

    suite("XorAllSubsets");
    expect_eq("single",   xorAllSubsets({5}),       5);
    expect_eq("two",      xorAllSubsets({1,2}),      1^2);
    expect_eq("multiple", xorAllSubsets({1,2,3,4}), 1^2^3^4);

    suite("PowerOfTwo");
    for (int e = 0; e < 10; ++e)
        expect_true("pow2_" + std::to_string(e), powerOfTwo(1<<e));
    for (int n : {0,3,5,6,7,9,10,12})
        expect_false("not_pow2_" + std::to_string(n), powerOfTwo(n));
    expect_false("negative", powerOfTwo(-2));
}

// ═════════════════════════════════════════════════════════════════════════════
// TRIE
// ═════════════════════════════════════════════════════════════════════════════

void test_trie() {
    suite("Trie");
    {
        Trie t;
        for (auto& w : {"apple","app","apt","bat"}) t.insert(w);
        expect_true ("search_apple",   t.search("apple"));
        expect_true ("search_app",     t.search("app"));
        expect_false("search_ap",      t.search("ap"));
        expect_false("search_missing", t.search("application"));
        expect_true ("starts_ap",      t.startsWith("ap"));
        expect_true ("starts_ba",      t.startsWith("ba"));
        expect_false("starts_cat",     t.startsWith("cat"));
    }

    suite("Autocomplete");
    {
        Autocomplete ac;
        for (auto& w : {"apple","app","apt","application","bat"}) ac.insert(w);
        auto r = ac.suggestions("app");
        std::sort(r.begin(), r.end());
        expect_eq("suggestions_app", r, std::vector<std::string>({"app","apple","application"}));
        expect_eq("no_match",  (int)ac.suggestions("xyz").size(), 0);
        {
            auto r2 = ac.suggestions("bat");
            expect_eq("full_word", r2, std::vector<std::string>({"bat"}));
        }
        {
            auto r3 = ac.suggestions("");
            std::sort(r3.begin(), r3.end());
            expect_eq("empty_prefix", r3,
                      std::vector<std::string>({"app","apple","application","apt","bat"}));
        }
    }

    suite("LongestPrefixMatch");
    {
        LongestPrefixMatch lpm;
        for (auto& w : {"app","apple","application"}) lpm.insert(w);
        expect_eq("exact",       lpm.longestPrefix("app"),         std::string("app"));
        expect_eq("longer_query",lpm.longestPrefix("applet"),      std::string("app"));
        expect_eq("no_match",    lpm.longestPrefix("banana"),      std::string(""));
        expect_eq("full_word",   lpm.longestPrefix("application"), std::string("app"));
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// HUFFMAN ENCODING
// ═════════════════════════════════════════════════════════════════════════════

void test_huffman() {
    suite("HuffmanEncoding");
    {
        auto codes = huffmanEncoding("abracadabra");
        for (char ch : std::string("abcdr"))
            expect_true(std::string("char_present_") + ch, codes.count(ch) > 0);
    }
    {
        auto codes = huffmanEncoding("hello world");
        for (auto& [ch, code] : codes) {
            bool allBinary = true;
            for (char c : code) if (c != '0' && c != '1') allBinary = false;
            expect_true(std::string("binary_code_") + ch, allBinary);
        }
    }
    {
        // Prefix-free property
        auto codes = huffmanEncoding("mississippi");
        bool prefixFree = true;
        std::vector<std::string> codeList;
        for (auto& [_, c] : codes) codeList.push_back(c);
        for (int i = 0; i < (int)codeList.size() && prefixFree; ++i)
            for (int j = 0; j < (int)codeList.size() && prefixFree; ++j)
                if (i != j && codeList[j].find(codeList[i]) == 0)
                    prefixFree = false;
        expect_true("prefix_free", prefixFree);
    }
    {
        auto codes = huffmanEncoding("aaaa");
        expect_eq("single_char", codes['a'], std::string("0"));
    }
    {
        std::string text = "abracadabra";
        auto codes = huffmanEncoding(text);
        int bits = 0;
        for (char ch : text) bits += (int)codes[ch].size();
        expect_true("fewer_bits_than_naive", bits < (int)text.size() * 8);
    }
}

// ═════════════════════════════════════════════════════════════════════════════
// MAIN
// ═════════════════════════════════════════════════════════════════════════════

int main() {
    test_twoSum();
    test_maxSubarray();
    test_mergeIntervals();
    test_minRotatedArray();
    test_trappingRainwater();
    test_threeSum();
    test_productExceptSelf();
    test_containerWithMostWater();
    test_removeDuplicatesSorted();
    test_rotateArray();

    test_strings();

    test_linkedList();
    test_stack();
    test_binaryTree();
    test_graphs();
    test_dp();
    test_greedy();
    test_heaps();
    test_bitManipulation();
    test_trie();
    test_huffman();

    printSummary();
    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
