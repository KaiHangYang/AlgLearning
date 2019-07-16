#include <iostream>
#include <vector>

using namespace std;
// https://mp.weixin.qq.com/s?__biz=Mzg2NzA4MTkxNQ==&mid=2247485231&amp;idx=1&amp;sn=8dfdc04bd209fba3077269faabe7c36f&source=41#wechat_redirect
class BinaryHeap {
    // 上浮操作只在插入的时候用到。
    void upAdjust(vector<int> &arr) {
        int n = arr.size();
        int child = n - 1;
        int parent = (child - 1) / 2;
        int tmp = arr[child];

        while (child > 0 && tmp < arr[parent]) {
            arr[child] = arr[parent];
            child = parent;
            parent = (child - 1) / 2;
        }

        arr[child] = tmp;
    }

    void downAdjust(vector<int> &arr, int parent) {
        int n = arr.size();
        int child = 2 * parent + 1;
        int tmp = arr[parent];

        while (child < n) {
            if (child + 1 < n && arr[child] > arr[child+1]) {
                child++;
            }

            if (tmp <= arr[child]) break;

            arr[parent] = arr[child];
            parent = child;
            child = 2 * parent + 1;
        }
        arr[parent] = tmp;
    }
public:
    void build(vector<int> &arr) {
        // (arr.size() - 1 - 1) / 2为，最后一个叶子节点的父节点。
        for (int i = (arr.size() - 2) / 2; i >= 0; i--) {
            downAdjust(arr, i);
        }
    }
};