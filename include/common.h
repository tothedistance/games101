#if !defined(__COMMON_H__)
#define __COMMON_H__
#include <glog/logging.h>
#include <gtest/gtest.h>
#include <Eigen/Core>
#include <Eigen/Dense>

#include <algorithm>
#include <array>
#include <chrono>
#include <climits>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <shared_mutex>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using namespace Eigen;

constexpr double PI = 3.14159265358979323846;

#define PREP_CONCAT(a, b) a##_##b
#define CONCAT(a, b) PREP_CONCAT(a, b)

#define DIR_NAME CMAKE_DIR_NAME
#define FILE_NAME CMAKE_FILE_NAME
#define DIR_FILE CONCAT(DIR_NAME, FILE_NAME)

/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* construct_listnode_list(string_view str) {
    vector<string> entries;
    bool delimited = false;
    set<char> sc{',', ' '};
    int begin = 1;
    for (int i = 1; i < str.length(); i++) {
        if (!delimited && sc.count(str[i])) {
            entries.emplace_back(str.begin() + begin, str.begin() + i);
            delimited = true;
            continue;
        } else if (delimited && sc.count(str[i])) {
            continue;
        } else if (delimited && !sc.count(str[i])) {
            begin = i;
            delimited = false;
            continue;
        }
    }
    if (begin != str.length() && !delimited) {
        entries.emplace_back(str.begin() + begin, str.end() - 1);
    }

    ListNode* dummy = new ListNode(0, nullptr);
    ListNode* head = dummy;
    for (auto& entry : entries) {
        ListNode* node = new ListNode(stoi(entry));
        head->next = node;
        head = node;
    }
    return dummy->next;
};

void print_listnode(ListNode* head) {
    while (head != nullptr) {
        cout << head->val << ' ';
        head = head->next;
    }
}

class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

Node* construct_node_list(string_view str) {
    int begin = 1;
    vector<pair<string, string>> entries;
    while ((begin = str.find('[', begin)) != -1) {
        int end = str.find(']', begin);
        string first, second;
        bool delimited = false;
        set<char> sc{',', ' '};
        for (int i = begin + 1; i < end; i++) {
            if (!delimited && sc.count(str[i])) {
                first = str.substr(begin + 1, i - begin - 1);
                delimited = true;
                continue;
            } else if (delimited && sc.count(str[i])) {
                continue;
            } else if (delimited && !sc.count(str[i])) {
                second = str.substr(i, end - i);
                break;
            }
        }
        entries.emplace_back(first, second);
        begin = end;
    }

    vector<Node*> ve;
    for (auto& p : entries) {
        ve.push_back(new Node(stoi(p.first)));
    }
    Node* prev = nullptr;
    for (int i = 0; i < entries.size(); i++) {
        auto e = ve[i];
        auto p = entries[i];
        if (prev != nullptr) {
            prev->next = e;
        }
        prev = e;
        if (p.second == "null") {
            e->random = nullptr;
        } else {
            int i = stoi(p.second);
            e->random = ve[i];
        }
    }
    return ve[0];
};

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right)
        : val(x), left(left), right(right) {}
};

TreeNode* construct_tree_node(string_view str) {
    vector<string> entries;
    bool delimited = false;
    set<char> sc{',', ' '};
    int begin = 1;
    for (int i = 1; i < str.length(); i++) {
        if (!delimited && sc.count(str[i])) {
            entries.emplace_back(str.begin() + begin, str.begin() + i);
            delimited = true;
            continue;
        } else if (delimited && sc.count(str[i])) {
            continue;
        } else if (delimited && !sc.count(str[i])) {
            begin = i;
            delimited = false;
            continue;
        }
    }
    if (begin != str.length() && !delimited) {
        entries.emplace_back(str.begin() + begin, str.end() - 1);
    }

    if (entries.empty()) {
        return nullptr;
    }

    queue<TreeNode*> q;
    q.push(new TreeNode(stoi(entries[0])));
    auto head = q.front();
    int count = 1;
    TreeNode* p;
    bool first_child = false;
    while (count < entries.size()) {
        if (!first_child) {
            p = q.front();
            q.pop();
        }
        if (p == nullptr) {
            continue;
        }

        TreeNode* node;
        if (entries[count] == "null") {
            node = nullptr;
        } else {
            node = new TreeNode(stoi(entries[count]));
        }
        q.push(node);
        if (!first_child) {
            p->left = node;
            first_child = true;
        } else {
            p->right = node;
            first_child = false;
        }
        count++;
    }
    return head;
}

vector<int> inorderTraversal(TreeNode* root) {
    vector<int> ans;
    if (root == nullptr) {
        return ans;
    }
    stack<pair<TreeNode*, int>> st;
    st.push({root, 0});
    while (!st.empty()) {
        auto& p = st.top();
        if (p.first == nullptr && p.second == 0) {
            p.second = 2;
            continue;
        }

        if (p.second == 0) {
            st.push({p.first->left, 0});
        } else if (p.second == 1) {
            ans.push_back(p.first->val);
            st.push({p.first->right, 0});
        } else {
            st.pop();
            if (st.empty()) {
                return ans;
            }

            auto& pp = st.top();
            if (pp.second == 0) {
                pp.second = 1;
            } else {
                pp.second = 2;
            }
        }
    }
    return ans;
}

template <typename T>
ostream& operator<<(ostream& out, vector<T>&& vec) {
    for (auto& ele : vec) {
        cout << ele << " ";
    }
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, vector<T>& vec) {
    for (auto& ele : vec) {
        cout << ele << " ";
    }
    return out;
}

#endif  // __COMMON_H__
