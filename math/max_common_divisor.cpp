#include <iostream>
#include <vector>

using namespace std;

int getMaxCommonDivisor(int m, int n) {
    int max_val, min_val;
    if (m > n) {
        max_val = m;
        min_val = n;
    }
    else {
        max_val = n;
        min_val = m;
    }
    int r;
    do {
        r = max_val % min_val;
        max_val = min_val;
        min_val = r;
    } while (r != 0);

    return max_val;
}

int main() {
    cout << getMaxCommonDivisor(10, 20) << endl;
    return 0;
}