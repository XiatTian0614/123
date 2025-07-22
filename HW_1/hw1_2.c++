#include <iostream>
#include <cmath>
using namespace std;
int Function(int m, int n) {
    if (m == 0) 
    {
        return n + 1;
    }
    if  (m == 1) 
    {
        return n + 2;
    }
    if  (m == 2) 
    {
        return 2 * n + 3;
    }
    if  (m == 3) 
    {
        return pow(2, n + 3) - 3;
    }
    else 
    {
        return -1;
    }
}

int main() {
    int m,n;
    cout << "輸入 m 與 n: "<< endl;
    cin >> m >> n;
    cout << "輸出為:" << Function( m, n) << endl;
    return 0;
}
