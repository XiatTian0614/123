#include <iostream>
using namespace std;
const int maxstack = 100000;            //設定堆疊的最大容量

int Function(int m, int n) {
    int stack[maxstack];                //使用陣列堆疊
    int top = 0;                        //初始指標
    stack[top++] = m;                   // 將初始 m 值推入堆疊

    while (top > 0) {                   // 當堆疊不為空時持續執行
        m = stack[--top];               // 取出堆疊頂端的值作為目前的 m

        if (m == 0) {                   // 當m=0，執行n+=1
            n += 1;
        } else if (n == 0) {            // 當n=0
            stack[top++] = m - 1;       // 將m - 1推入堆疊中
            n = 1;                      // 將N設定為1
        } else {
            stack[top++] = m - 1;       // 先推入 m - 1（先進後出）
            stack[top++] = m;           // 後將 m 推入堆疊（先進後出）
            n -= 1;                     // 將 n 減 1，準備下一層
        }
    }

    return n;                            // 回傳 n
}

int main() {
    int m,n;
    cout << "輸入M與N: "<< endl;
    cin >> m >> n;
    cout << "輸出為: " << Function(m, n) << endl;
    return 0;
}