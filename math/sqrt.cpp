#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

// binary search
#define M_EPISlON 0.00001
float sqrt_0(float a) {
    if (a < 0) {
        return -1;
    }
    float start = 0, end = a, mid = (start + end) * 0.5;
    while (fabsf(end - start) > M_EPISlON) {
        mid = (start + end) * 0.5;
        if (mid * mid > a) {
            end = mid;
        }
        else {
            start = mid;
        }
    }
    return mid;
}

// newtown
float sqrt_1(float a) {
    if (a < 0) {
        return -1;
    }
    float x = a, y = 0;
    // f(x) = x^2 - n
    // 求 f(x) = 0的根
    // 根据曲线，先得到在x0处的切线的方程和y=0的交点，此交点的x值就是下一个x，
    // 因为曲线x^2是一个凸函数所以可以用牛顿迭代法来求。
    while (fabsf(x - y) > M_EPISlON) {
        y = x;
        x = 0.5 * (x + a / x);
    }
    return x;
}

int main() {
    float i;
    i = 10; cout << sqrt(i) << " " << sqrt_0(i) << " " << sqrt_1(i) << endl;
    i = 0; cout << sqrt(i) << " " << sqrt_0(i) << " " << sqrt_1(i) << endl;
    i = 583; cout << sqrt(i) << " " << sqrt_0(i) << " " << sqrt_1(i) << endl;
    i = -2; cout << sqrt(i) << " " << sqrt_0(i) << " " << sqrt_1(i) << endl;
    return 0;
}