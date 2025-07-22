#include <iostream>
using namespace std;


int Function(int m, int n) {
    if (m == 0) //如果m=0的情況
    {
        return n + 1;
    }
    if (n == 0) //當n=0的情況，重新呼叫Function(m - 1, 1)
    {
        return Function(m - 1, 1);
    }
    return Function(m - 1, Function(m, n - 1));//當都沒有符合的情況，重新呼叫Function(m - 1, Function(m, n - 1))
}
int main() {
    int m,n;
    cout << "輸入M與N: "<< endl;
    cin >> m >> n;
    cout << "輸出為: " << Function(m, n) << endl;
    return 0;
}