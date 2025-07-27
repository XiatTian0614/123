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
