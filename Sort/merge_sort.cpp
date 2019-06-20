#include <vector>
#include <algorithm>

using namespace std;
void merge(vector<int> &arr, int first, int mid, int last) {
    int i = first, j = mid + 1;
    int m = mid, n = last;
    int k = 0;
    vector<int> tmp(last - first + 1);
    while (i <= m && j <= n) {
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];
        }
        else {
            tmp[k++] = arr[j++];
        }
    }

    while (i <= m) {
        tmp[k++] = arr[i++];
    }
    
    while (j <= n) {
        tmp[k++] = arr[j++];
    }
    copy(tmp.begin(), tmp.end(), arr.begin() + first);
}

void mergesort(vector<int> &arr, int first, int last) {
    if (first < last) {
        int mid = (first + last) / 2;
        mergesort(arr, first, mid);
        mergesort(arr, mid+1, last);
        merge(arr, first, mid, last);
    }
}