#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    void get(vector<int> &weight, vector<int> &value, int target) {
        int n = weight.size();
        vector<int> result(n + 1, 0);
        // 数组第二个元素表示的是，在背包里总重量为w的最优解。
        vector<vector<int>> dp(n + 1, vector<int>(target + 1, 0));
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= target; ++j) {
                if (j < weight[i]) {
                    dp[i][j] = dp[i - 1][j]; // 因为背包当前只装了j的东西，所以i一定不能在里面
                }
                else {
                    // i 在里面或者不在里面的最大值
                    // i 若在里面的话，则说明上一个状态是背包总量为j - weight[i]
                    if (dp[i - 1][j] < dp[i - 1][j - weight[i]] + value[i]) {
                        dp[i][j] = dp[i - 1][j - weight[i]] + value[i];
                        result[i] = 1;
                    }
                    else {
                        dp[i][j] = dp[i - 1][j];
                    }
                }
            }
        }
        // 如何找到所有包含在内的元素呢
    }
};

int main() {
    
    return 0;
}