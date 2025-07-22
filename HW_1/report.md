# 41141147

# 作業一的問題一

## 解題說明
本題要求以以下數學定義實作  Ackermann 函數：

    Function(m, n) =n + 1               當 m = 0 時
    Function(m - 1, 1)                  當 m > 0 且 n = 0 時
    Function(m - 1, Function(m, n - 1)) 當 m > 0 且 n > 0 時
    
### 解題策略
1.用遞迴依定義實作Ackermann函數

2.實作 `Function(int m, int n)` 函數並處理三種情況

3.注意不要輸入過大的 m 與 n，避免堆疊溢位（stack overflow）

## 程式實作

以下為遞迴版本的程式碼：
```cpp
#include <iostream>
using namespace std;


int Function(int m, int n) {
    if (m == 0) 
    {
        return n + 1;
    }
    if (n == 0) 
    {
        return Function(m - 1, 1);
    }
    return Function(m - 1, Function(m, n - 1));
}

int main() {
    int m = 2, n = 1;
    cout << "輸出為: " << Function(m, n) << endl;
    return 0;
}
```
以下為非遞迴版本的程式碼：
```cpp
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

```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 `O(A(m, n))`
2. 空間複雜度：空間複雜度為 `O(A(m, n))`

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (m, n) | 預期輸出 | 實際輸出 |
|----------|------------------|----------|----------|
| 測試一   | (0, 0)           | 1        | 1        |
| 測試二   | (1, 1)           | 3        | 3        |
| 測試三   | (1, 2)           | 4        | 4        |
| 測試四   | (2, 2)           | 7        | 7        |
| 測試五   | (3, 3)           | 61      | 61      |
| 測試六   | (3, -1)           | 異常      | 異常      |

### 編譯與執行指令

```shell
$ g++ -o ackermann main.cpp
$ ./ackermann
```

### 結論

1.程式能正確實作，並成功計算出函數結果。

2.當輸入 n 為負數時，會成功拋出異常，符合預期。

3.測試案例涵蓋多種可能驗證程式的正確性。

## 申論及開發報告

### 選擇使用遞迴的原因
1.Ackermann函數本身就是遞迴定義，所以直接使用遞迴實作

2.程式碼較簡單

3.遞迴版本在m超過4時容易造成記憶體堆疊溢位（Stack Overflow）

# 作業一的問題二

## 解題說明
本題要求以以下數學定義實作  Power Set 子集合的集合：

    S = {a, b, c}
    Power set = 
    {   {}, {a}, {b}, {c}, {a, b}, {a, c}, {b, c}, {a, b, c}    }
    
### 解題策略
1.Power Set 是集合中所有子集合的集合，數量為 `2^n`，其中 `n` 為集合中元素的數量。

2.利用遞迴的方式，對每個元素做「選」或「不選」的決策，依此產生所有可能的子集合。

3.將子集合的元素們加在一起，得到所有子集合的集合。

## 程式實作

以下為程式碼：
```cpp
#include <iostream>
#include <vector>
using namespace std;

vector<vector<char>> powerset(vector<char> S) {
    if (S.empty()) {
        return { {} };
    }
    char first = S[0];
    S.erase(S.begin());

    vector<vector<char>> other = powerset(S);

    vector<vector<char>> all;
    for (auto subset : other) {
        subset.insert(subset.begin(), first);
        all.push_back(subset);
    }

    other.insert(other.end(), all.begin(), all.end());
    return other;
}

int main() {
    vector<char> S = {'a', 'b', 'c'};
    vector<vector<char>> result = powerset(S);
    for (auto subset : result) {
        cout << "(";
        for (char c : subset) {
            cout << c;
        }
        cout << ")" << endl;
    }

    return 0;
}
```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 `O(n*2 ^ n)`
2. 空間複雜度：空間複雜度為 `O(n*2 ^ n)`

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (m, n) | 預期輸出 | 實際輸出 |
|----------|------------------|----------|----------|
| 測試一   | [1,2,3]           | ()(3)(2)(23)(1)(13)(12)(123)        | ()(3)(2)(23)(1)(13)(12)(123)        |
| 測試二   | [a,b,c]           | ()(c)(b)(bc)(a)(ac)(ab)(abc)       | ()(c)(b)(bc)(a)(ac)(ab)(abc)         |
| 測試三   | [a,b,c,d]          | ()(d)(c)(cd)(b)(bd)(bc)(bcd)(a)(ad)(ac)(acd)(ab)(abd)(abc)(abcd)        | ()(d)(c)(cd)(b)(bd)(bc)(bcd)(a)(ad)(ac)(acd)(ab)(abd)(abc)(abcd)        |

### 編譯與執行指令

```shell
$ g++ -o ackermann main.cpp
$ ./ackermann
```

### 結論

1.程式正確產生所有子集合。

2.輸出順序為「遞增遞迴」結果。

3.子集合元素順序保留原順序。

## 申論及開發報告

### 選擇使用遞迴的原因
1.遞迴實作直觀易懂，程式結構清楚。

2.當集合長度增加時，時間與空間消耗呈指數成長，效能下降明顯。

3.遞迴過深可能導致 stack overflow（如 n > 20）。
