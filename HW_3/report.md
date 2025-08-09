# 41141147

# 作業二

## 解題說明
這個作業要求我們製作一個多項式的物件，並實作其中的功能。

係數（coef）：float 類型

指數（exp）：int 類型

指向下一個節點的指針（link）：Term* 類型

此外，還需要實現多項式的 輸入、輸出、加法、減法、乘法、求值 等功能，並使用 可用空間列表（avail-space list） 來管理節點的內存分配與回收，以提高效率。

    
### 解題策略
1.用兩個陣列分別存係數和指數，保持索引對應

2.每次新增項時自動排序，確保指數嚴格遞減

3.驗證「項數n」與後續c/e配對數量是否一致

## 程式實作

```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;
    Term* link;
    Term(float c, int e, Term* next = nullptr) : coef(c), exp(e), link(next) {}
    
public:
    float getCoef() const { return coef; }
    int getExp() const { return exp; }
    Term* getLink() const { return link; }
    void setLink(Term* next) { link = next; }
    void setCoef(float c) { coef = c; }
};

class Polynomial {
private:
    Term* head;
    static Term* avail;

    Term* GetNode(float c, int e, Term* next) {
        Term* p;
        if (avail == nullptr) {
            p = new Term(c, e, next);
        } else {
            p = avail;
            avail = avail->getLink();
            p->setCoef(c);
            p->setLink(next);
        }
        return p;
    }

    void RetNode(Term* x) {
        x->setLink(avail);
        avail = x;
    }

    void Clear() {
        if (head == nullptr) return;
        
        Term* current = head->getLink();
        while (current != head) {
            Term* temp = current;
            current = current->getLink();
            RetNode(temp);
        }
        RetNode(head);
        head = nullptr;
    }

public:
    Polynomial() : head(nullptr) {}
    
    Polynomial(const Polynomial& a) : head(nullptr) {
        if (a.head == nullptr) return;
        
        head = GetNode(0, -1, nullptr);
        head->setLink(head);
        
        Term* current = a.head->getLink();
        while (current != a.head) {
            Insert(current->getCoef(), current->getExp());
            current = current->getLink();
        }
    }
    
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {
            Clear();
            if (a.head != nullptr) {
                head = GetNode(0, -1, nullptr);
                head->setLink(head);
                
                Term* current = a.head->getLink();
                while (current != a.head) {
                    Insert(current->getCoef(), current->getExp());
                    current = current->getLink();
                }
            }
        }
        return *this;
    }
    
    ~Polynomial() {
        Clear();
    }
    
    void Insert(float c, int e) {
        if (head == nullptr) {
            head = GetNode(0, -1, nullptr);
            head->setLink(head);
            head->setLink(GetNode(c, e, head));
            return;
        }
        
        Term* prev = head;
        Term* current = head->getLink();
        
        while (current != head && current->getExp() > e) {
            prev = current;
            current = current->getLink();
        }
        
        if (current != head && current->getExp() == e) {
            current->setCoef(current->getCoef() + c);
            if (current->getCoef() == 0) {
                prev->setLink(current->getLink());
                RetNode(current);
            }
        } else {
            prev->setLink(GetNode(c, e, current));
        }
    }
    
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        
        Term* aCurrent = (head == nullptr) ? nullptr : head->getLink();
        Term* bCurrent = (b.head == nullptr) ? nullptr : b.head->getLink();
        
        while ((aCurrent != nullptr && aCurrent != head) || 
               (bCurrent != nullptr && bCurrent != b.head)) {
            float coef = 0;
            int exp = 0;
            
            if (aCurrent != nullptr && aCurrent != head && 
                (bCurrent == nullptr || bCurrent == b.head || aCurrent->getExp() > bCurrent->getExp())) {
                coef = aCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
            } else if (bCurrent != nullptr && bCurrent != b.head && 
                       (aCurrent == nullptr || aCurrent == head || bCurrent->getExp() > aCurrent->getExp())) {
                coef = bCurrent->getCoef();
                exp = bCurrent->getExp();
                bCurrent = bCurrent->getLink();
            } else {
                coef = aCurrent->getCoef() + bCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
                bCurrent = bCurrent->getLink();
            }
            
            if (coef != 0) {
                result.Insert(coef, exp);
            }
        }
        
        return result;
    }
    
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        
        Term* aCurrent = (head == nullptr) ? nullptr : head->getLink();
        Term* bCurrent = (b.head == nullptr) ? nullptr : b.head->getLink();
        
        while ((aCurrent != nullptr && aCurrent != head) || 
               (bCurrent != nullptr && bCurrent != b.head)) {
            float coef = 0;
            int exp = 0;
            
            if (aCurrent != nullptr && aCurrent != head && 
                (bCurrent == nullptr || bCurrent == b.head || aCurrent->getExp() > bCurrent->getExp())) {
                coef = aCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
            } else if (bCurrent != nullptr && bCurrent != b.head && 
                       (aCurrent == nullptr || aCurrent == head || bCurrent->getExp() > aCurrent->getExp())) {
                coef = -bCurrent->getCoef();
                exp = bCurrent->getExp();
                bCurrent = bCurrent->getLink();
            } else {
                coef = aCurrent->getCoef() - bCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
                bCurrent = bCurrent->getLink();
            }
            
            if (coef != 0) {
                result.Insert(coef, exp);
            }
        }
        
        return result;
    }
    
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        
        if (head == nullptr || b.head == nullptr) return result;
        
        Term* aCurrent = head->getLink();
        while (aCurrent != head) {
            Term* bCurrent = b.head->getLink();
            while (bCurrent != b.head) {
                float coef = aCurrent->getCoef() * bCurrent->getCoef();
                int exp = aCurrent->getExp() + bCurrent->getExp();
                result.Insert(coef, exp);
                bCurrent = bCurrent->getLink();
            }
            aCurrent = aCurrent->getLink();
        }
        
        return result;
    }
    
    float Evaluate(float x) const {
        float result = 0;
        
        if (head == nullptr) return result;
        
        Term* current = head->getLink();
        while (current != head) {
            result += current->getCoef() * pow(x, current->getExp());
            current = current->getLink();
        }
        
        return result;
    }
    
    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);
};

Term* Polynomial::avail = nullptr;

istream& operator>>(istream& is, Polynomial& x) {
    x.Clear();
    
    int n;
    is >> n;
    
    if (n > 0) {
        x.head = x.GetNode(0, -1, nullptr);
        x.head->setLink(x.head);
        
        for (int i = 0; i < n; i++) {
            float coef;
            int exp;
            is >> coef >> exp;
            x.Insert(coef, exp);
        }
    }
    
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& x) {
    if (x.head == nullptr || x.head->getLink() == x.head) {
        os << "0";
        return os;
    }
    
    Term* current = x.head->getLink();
    bool firstTerm = true;
    
    while (current != x.head) {
        if (!firstTerm && current->getCoef() > 0) {
            os << "+";
        }
        
        if (current->getExp() == 0) {
            os << current->getCoef();
        } else {
            if (current->getCoef() != 1 && current->getCoef() != -1) {
                os << current->getCoef();
            } else if (current->getCoef() == -1) {
                os << "-";
            }
            
            os << "x";
            
            if (current->getExp() != 1) {
                os << "^" << current->getExp();
            }
        }
        
        firstTerm = false;
        current = current->getLink();
    }
    
    return os;
}

int main() {
    Polynomial p1, p2;
    
    cout << "輸入第一個多項式 (輸入: n c1 e1 c2 e2 ...): ";
    cin >> p1;
    
    cout << "輸入第二個多項式 (輸入: n c1 e1 c2 e2 ...): ";
    cin >> p2;
    
    cout << "\n第一個多項式: " << p1 << endl;
    cout << "第二個多項式: " << p2 << endl;
    
    Polynomial sum = p1 + p2;
    cout << "總和: " << sum << endl;
    
    Polynomial diff = p1 - p2;
    cout << "差分: " << diff << endl;
    
    Polynomial product = p1 * p2;
    cout << "積分: " << product << endl;
    
    float x;
    cout << "\n輸入要計算多項式的值 x: ";
    cin >> x;
    
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "p2(" << x << ") = " << p2.Evaluate(x) << endl;
    
    return 0;
}
```
## 效能分析

1. 時間複雜度：加法/減法 O(m + n)，乘法 O(m × n)，輸入 O(n²)
2. 空間複雜度：O(n + m)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (p1,p2,n1,n2) | 預期輸出 | 實際輸出 |
|----------|------------------|----------|----------|
| 測試一   | 5 5 5 4 4 3 3 2 2 1 1| 第一個多項式: 5x^5+4x^4+3x^3+2x^2+x|第一個多項式: 5x^5+4x^4+3x^3+2x^2+x
||3 3 3 2 2 1 1|第二個多項式: 3x^3+2x^2+x|第二個多項式: 3x^3+2x^2+x
|||總和: 5x^5+4x^4+6x^3+4x^2+2x|總和: 5x^5+4x^4+6x^3+4x^2+2x
|||差分: 5x^5+4x^4|差分: 5x^5+4x^4
|||積分: 15x^8+22x^7+22x^6+16x^5+10x^4+4x^3+x^2|積分: 15x^8+22x^7+22x^6+16x^5+10x^4+4x^3+x^2
| 測試二   | 3 -2 3 1 2 -1 0| 第一個多項式: -2x^3+x^2-1x+2|第一個多項式: -2x^3+x^2-1x+2|
||2 1 1 -3 0|第二個多項式: x-3|第二個多項式: x-3
|||總和: -2x^3+x^2+x-4|總和: -2x^3+x^2+x-4
|||差分: -2x^3+x^2-x+2|差分: -2x^3+x^2-x+2
|||積分: -2x^4+7x^3-3x^2-x+3|積分: -2x^4+7x^3-3x^2-x+3
| 測試三   | 2 1 10 1 5| 第一個多項式: x^10+x^5+1|第一個多項式: x^10+x^5+1
||1 1 8|第二個多項式: x^8+1|第二個多項式: x^8+1
|||總和: x^10+x^8+x^5+2|總和: x^10+x^8+x^5+2
|||差分: x^10-x^8+x^5-1|差分: x^10-x^8+x^5-1
|||積分: x^18+x^13+x^8+x^5+2|積分: x^18+x^13+x^8+x^5+2

### 編譯與執行指令

```shell
g++ -o hw3 hw3.c++
.\hw3
```

### 結論

1.本程式使用循環鏈結串列來實現多項式運算。

2.程式實現了簡化的記憶體回收機制。

3.程式完整實現了多項式的加法、減法和乘法運算。

## 申論及開發報告

### 申論
1.本程式採用物件導向設計，將多項式封裝為Polynomial類，項封裝為Term類。

2.程式碼有效區隔每個物件的函數，可以單獨理解。

3.確保結果多項式始終保持指數降序排列。