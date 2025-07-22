#include <iostream>

const int maxstack = 100000;

int Function(int m, int n) {
    int stack[maxstack];
    int top = 0;
    stack[top++] = m;

    while (top > 0) {
        m = stack[--top];

        if (m == 0) {
            n += 1;
        } else if (n == 0) {
            stack[top++] = m - 1;
            n = 1;
        } else {
            stack[top++] = m - 1;
            stack[top++] = m;
            n -= 1;
        }
    }

    return n;
}

int main() {
    int m, n;
    cin >> m >> n;
    cout << Function(m, n) << endl;
    return 0;
}
