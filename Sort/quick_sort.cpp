#include <vector>
#include <utility>
using namespace std;

int partition(vector<int> &arr, int left, int right) {
    int i = left + 1;
    int j = right;
    int temp = arr[left];

    while (i <= j) {
        while (arr[i] < temp) {
            i++;
        }
        while (arr[j] > temp) {
            j--;
        }
        if (i < j) {
            swap(arr[i++], arr[j--]);
        }
        else {
            i++;
        }
    }
    // j is first element <= temp, right all > temp
    swap(arr[j], arr[left]);
    return j;
}


int partition_2(vector<int> &arr, int left, int right) {
    int x = left - 1, y = left;
    // x + 1 always point to the one maybe > right
    while (y < right) {
        if (arr[y] < arr[right]) {
            // y pos < pivot
            int tmp = arr[y];
            arr[y] = arr[++x];
            arr[x] = tmp;
        }
        ++y;
    }
    int tmp = arr[right];
    arr[right] = arr[x+1];
    arr[x+1] = tmp;
}
void quick_sort(vector<int> &arr, int left, int right) {
    if (left > right) {
        return;
    }
    int j = partition(arr, left, right);
    quick_sort(arr, left, j - 1);
    quick_sort(arr, j + 1, right);
}