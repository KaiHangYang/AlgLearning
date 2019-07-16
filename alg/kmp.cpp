#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

// 主要是对pattern计算一个前后缀匹配next序列，然后匹配过程中根据这个匹配序列在匹配错的基础上进行回退。
void get_next(string patten, vector<int>& next) {
    string str1 = "", str2 = "", str3 = "";
    next = vector<int>(patten.size(), 0);
    for (int i = 0; i < patten.size(); ++i) {
        str1 = patten.substr(0, i+1); // the already matched string.
        for (int j = 1; j < str1.size(); ++j) {
            // j is the length of the suffix and prefix
            // suffix
            str2 = str1.substr(str1.size() - j, j);
            // prefix
            str3 = str1.substr(0, str2.size());

            if (str2 == str3) {
                // matched
                next[i] = j;
            }
        }
    }
}

int main() {
    vector<int> next;
    get_next("AABAABAAC", next);
    for (int i = 0; i < next.size(); ++i) {
        cout << next[i] << endl;
    }
    return 0;
}