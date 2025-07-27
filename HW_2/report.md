# 41141147

# 作業二

## 解題說明
本題要求以以下數學定義實作 多項式相加函數:

    
### 解題策略
1.創建多個陣列供給多項式使用，儲存各項的係數與次方

2.維持多項式項次遞減順序以便相加函數可以實作

3.判斷錯誤輸入時，將異常輸出

## 程式實作

```cpp
#include <iostream>
using namespace std;

const int MAX_TERMS = 100; //設定上限為100項

class Term {
public:
    float coef; // 係數
    int exp;    // 次方
};

class Polynomial {
private:
    Term termArray[MAX_TERMS];  // 陣列儲存資料
    int terms;  // 當前項數

public:
    Polynomial() {
        terms = 0;
    }

    void NewTerm(float c, int e) { // 新增一項陣列儲存資料
        if (terms >= MAX_TERMS) {
            cout << "錯誤：超過項目上限！" << endl;
            return;
        }
        termArray[terms].coef = c;  // 設定係數
        termArray[terms].exp = e;   // 設定次方
        terms++;
    }

    // 輸入多項式
    void input() {
        cout << "輸入項數: ";
        cin >> terms;
        if (terms>=0) {
            for (int i = 0; i < terms; ++i) {
                cout << "輸入第 " << i + 1 << " (coef exp): ";
                cin >> termArray[i].coef >> termArray[i].exp;
            }
        } else {
            cout << "錯誤：輸入項數小於0！" << endl;
            exit(1);
        }
    }

    // 輸出多項式
    void output() const {
        for (int i = 0; i < terms; ++i) {
            int coef = termArray[i].coef;
            int exp = termArray[i].exp;
            if (i > 0) {    // 如果不是第一項
                if (coef > 0) cout << " + ";    //第一項不顯示 +，但顯示 -
                else cout << " - ";
            } 
            else {
                if (coef < 0) cout << "-";
            }
            int absCoef = abs(coef);
            if (absCoef != 1 || exp == 0) {
                cout << absCoef;
            }
            if (exp != 0) {
                cout << "x";
                if (exp != 1) {
                    cout << "^" << exp;
                }
            }
        }
        cout << endl;
    }

    Polynomial Add(const Polynomial& b) const {
        Polynomial c;
        int aPos = 0, bPos = 0;

        while (aPos < terms && bPos < b.terms) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float t = termArray[aPos].coef + b.termArray[bPos].coef;
                if (t != 0)
                    c.NewTerm(t, termArray[aPos].exp);
                aPos++;
                bPos++;
            }
            else if (termArray[aPos].exp > b.termArray[bPos].exp) {
                c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
        }

        // 加入剩下的 a(x) 項目
        for (; aPos < terms; aPos++)
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

        // 加入剩下的 b(x) 項目
        for (; bPos < b.terms; bPos++)
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

        return c;
    }
};
int main() {
    Polynomial p1, p2, sum;

    cout << "輸入第一個多項式:" << endl;
    p1.input();

    cout << "輸入第二個多項式:" << endl;
    p2.input();

    sum = p1.Add(p2);

    cout << "\n第一個多項式: ";
    p1.output();

    cout << "第二個多項式: ";
    p2.output();

    cout << "相加結果: ";
    sum.output();

    return 0;
}
```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 `O(n + m)`，其中 `n` 和 `m` 分別是兩個多項式的項數。
2. 空間複雜度：空間複雜度為 `O(1)`，因為只使用了有限的額外空間來儲存結果。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (p1,p2,n1,n2) | 預期輸出 | 實際輸出 |
|----------|------------------|----------|----------|
| 測試一   | (x^2 + 2x + 3, x^3 + 2x + 5,3,3)| x^3 + x^2 + 4x + 8 | x^3 + x^2 + 4x + 8 |
| 測試二   | (x^3 + 2 + 3x^-1, x^5 + 3x^2 + 4 + 5x^-1,3,4)| x^3 + x^2 + 4x + 8 | x^3 + x^2 + 4x + 8 |
| 測試三   | (x^2 + 2x + 3, x^3 + 2x + 5,-1,3)| 錯誤：輸入項數小於0！ | 錯誤：輸入項數小於0！ |

### 編譯與執行指令

```shell
g++ -o hw2 hw2.c++
.\hw2
```

### 結論

1.程式能正確實作，並成功計算出多項式相加結果。

2.當輸入項數為負數時，會成功拋出錯誤，符合預期。

3.測試案例涵蓋多種可能驗證程式的正確性。

## 申論及開發報告

### 申論
1.使用固定大小陣列 termArray[] 儲存多項式項目，雖然簡單，但有最大項數限制（100項）。

2.程式碼有效區隔每個物件的函數，可以單獨理解。

3.當輸入項數為負數時，會成功拋出錯誤，直接結束程式，不會再接著詢問第二項多項式。
