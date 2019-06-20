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
            swap(arr[i++], arr[j--])；
        }
        else {
            i++;
        }
    }
    // j is first element <= temp, right all > temp
    swap(arr[j], arr[left]);
    return j;
}

void quick_sort(vector<int> &arr, int left, int right) {
    if (left > right) {
        return;
    }
    int j = partition(arr, left, right);
    quick_sort(arr, left, j - 1);
    quick_sort(arr, j + 1, right);
}