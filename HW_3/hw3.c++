#include <iostream>
#include <cmath>
using namespace std;

// 前向宣告 Polynomial 類別，因為 Term 需要知道它
class Polynomial;

// Term 類別 - 表示多項式中的單一項
class Term {
    friend class Polynomial; // 允許 Polynomial 存取私有成員
private:
    float coef; // 係數
    int exp;    // 指數
    Term* link; // 指向下一個項的指針
    
    // 構造函數，初始化係數、指數和鏈接
    Term(float c, int e, Term* next = nullptr) : coef(c), exp(e), link(next) {}
    
public:
    // 獲取係數
    float getCoef() const { return coef; }
    // 獲取指數
    int getExp() const { return exp; }
    // 獲取下一個節點指針
    Term* getLink() const { return link; }
    // 設置下一個節點指針
    void setLink(Term* next) { link = next; }
    // 設置係數
    void setCoef(float c) { coef = c; }
};


class Polynomial {
private:
    Term* head; // 頭節點指針
    static Term* avail; 
    
    // 獲取新節點（從可用列表或新建）
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
    
    // 回收節點到可用列表
    void RetNode(Term* x) {
        x->setLink(avail);
        avail = x;
    }
    
    // 清空多項式，釋放所有節點
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
    // 默認構造函數
    Polynomial() : head(nullptr) {}
    
    // 拷貝構造函數
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
    
    // 賦值運算符重載
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
        
        // 尋找合適的插入位置
        while (current != head && current->getExp() > e) {
            prev = current;
            current = current->getLink();
        }
        
        // 如果指數已存在，合併同類項
        if (current != head && current->getExp() == e) {
            current->setCoef(current->getCoef() + c);
            // 如果合併後係數為0，刪除該項
            if (current->getCoef() == 0) {
                prev->setLink(current->getLink());
                RetNode(current);
            }
        } else {
            // 插入新項
            prev->setLink(GetNode(c, e, current));
        }
    }
    
    // 多項式加法運算符重載
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        
        Term* aCurrent = (head == nullptr) ? nullptr : head->getLink();
        Term* bCurrent = (b.head == nullptr) ? nullptr : b.head->getLink();
        
        // 合併兩個多項式的項
        while ((aCurrent != nullptr && aCurrent != head) || 
            (bCurrent != nullptr && bCurrent != b.head)) {
            float coef = 0;
            int exp = 0;
            
            // 處理 a 當前項指數大於 b 當前項指數的情況
            if (aCurrent != nullptr && aCurrent != head && 
                (bCurrent == nullptr || bCurrent == b.head || aCurrent->getExp() > bCurrent->getExp())) {
                coef = aCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
            } 
            // 處理 b 當前項指數大於 a 當前項指數的情況
            else if (bCurrent != nullptr && bCurrent != b.head && 
                    (aCurrent == nullptr || aCurrent == head || bCurrent->getExp() > aCurrent->getExp())) {
                coef = bCurrent->getCoef();
                exp = bCurrent->getExp();
                bCurrent = bCurrent->getLink();
            } 
            // 處理指數相同的情況（相加係數）
            else {
                coef = aCurrent->getCoef() + bCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
                bCurrent = bCurrent->getLink();
            }
            
            // 只有係數不為0才插入結果
            if (coef != 0) {
                result.Insert(coef, exp);
            }
        }
        
        return result;
    }
    
    // 多項式減法運算符重載
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        
        Term* aCurrent = (head == nullptr) ? nullptr : head->getLink();
        Term* bCurrent = (b.head == nullptr) ? nullptr : b.head->getLink();
        
        // 類似加法，但處理減法邏輯
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
                coef = -bCurrent->getCoef(); // 注意係數取負
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
    
    // 多項式乘法運算符重載
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        
        if (head == nullptr || b.head == nullptr) return result;
        
        Term* aCurrent = head->getLink();
        // 對 a 的每一項
        while (aCurrent != head) {
            Term* bCurrent = b.head->getLink();
            // 與 b 的每一項相乘
            while (bCurrent != b.head) {
                float coef = aCurrent->getCoef() * bCurrent->getCoef();
                int exp = aCurrent->getExp() + bCurrent->getExp(); // 指數相加
                result.Insert(coef, exp);
                bCurrent = bCurrent->getLink();
            }
            aCurrent = aCurrent->getLink();
        }
        
        return result;
    }
    
    // 計算多項式在 x 處的值
    float Evaluate(float x) const {
        float result = 0;
        
        if (head == nullptr) return result;
        
        Term* current = head->getLink();
        while (current != head) {
            // 計算每一項的值並累加
            result += current->getCoef() * pow(x, current->getExp());
            current = current->getLink();
        }
        
        return result;
    }
    
    // 友元函數宣告，用於輸入輸出
    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);
};

// 初始化靜態成員 avail
Term* Polynomial::avail = nullptr;

// 輸入運算符重載
istream& operator>>(istream& is, Polynomial& x) {
    x.Clear(); // 清空現有多項式
    
    int n;
    is >> n; // 讀取項數
    
    if (n > 0) {
        x.head = x.GetNode(0, -1, nullptr);
        x.head->setLink(x.head);
        
        // 讀取每一項的係數和指數
        for (int i = 0; i < n; i++) {
            float coef;
            int exp;
            is >> coef >> exp;
            x.Insert(coef, exp);
        }
    }
    
    return is;
}

// 輸出運算符重載
ostream& operator<<(ostream& os, const Polynomial& x) {
    // 處理空多項式
    if (x.head == nullptr || x.head->getLink() == x.head) {
        os << "0";
        return os;
    }
    
    Term* current = x.head->getLink();
    bool firstTerm = true;
    
    // 輸出每一項
    while (current != x.head) {
        // 處理正係數前的加號
        if (!firstTerm && current->getCoef() > 0) {
            os << "+";
        }
        
        // 處理常數項
        if (current->getExp() == 0) {
            os << current->getCoef();
        } else {
            // 處理係數為1或-1的特殊情況
            if (current->getCoef() != 1 && current->getCoef() != -1) {
                os << current->getCoef();
            } else if (current->getCoef() == -1) {
                os << "-";
            }
            
            os << "x";
            
            // 處理指數為1的特殊情況
            if (current->getExp() != 1) {
                os << "^" << current->getExp();
            }
        }
        
        firstTerm = false;
        current = current->getLink();
    }
    
    return os;
}

// 主函數
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