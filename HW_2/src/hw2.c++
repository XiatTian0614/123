#include <iostream>
using namespace std;

const int MAX_TERMS = 100; //�]�w�W����100��

class Term {
public:
    float coef; // �Y��
    int exp;    // ����
};

class Polynomial {
private:
    Term termArray[MAX_TERMS];  // �}�C�x�s���
    int terms;  // ��e����

public:
    Polynomial() {
        terms = 0;
    }

    void NewTerm(float c, int e) { // �s�W�@���}�C�x�s���
        if (terms >= MAX_TERMS) {
            cout << "���~�G�W�L���ؤW���I" << endl;
            return;
        }
        termArray[terms].coef = c;  // �]�w�Y��
        termArray[terms].exp = e;   // �]�w����
        terms++;
    }

    // ��J�h����
    void input() {
        cout << "��J����: ";
        cin >> terms;
        if (terms>=0) {
            for (int i = 0; i < terms; ++i) {
                cout << "��J�� " << i + 1 << " (coef exp): ";
                cin >> termArray[i].coef >> termArray[i].exp;
            }
        } else {
            cout << "���~�G��J���Ƥp��0�I" << endl;
            exit(1);
        }
    }

    // ��X�h����
    void output() const {
        for (int i = 0; i < terms; ++i) {
            int coef = termArray[i].coef;
            int exp = termArray[i].exp;
            if (i > 0) {    // �p�G���O�Ĥ@��
                if (coef > 0) cout << " + ";    //�Ĥ@������� +�A����� -
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

        // �[�J�ѤU�� a(x) ����
        for (; aPos < terms; aPos++)
            c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);

        // �[�J�ѤU�� b(x) ����
        for (; bPos < b.terms; bPos++)
            c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);

        return c;
    }
};
int main() {
    Polynomial p1, p2, sum;

    cout << "��J�Ĥ@�Ӧh����:" << endl;
    p1.input();

    cout << "��J�ĤG�Ӧh����:" << endl;
    p2.input();

    sum = p1.Add(p2);

    cout << "\n�Ĥ@�Ӧh����: ";
    p1.output();

    cout << "�ĤG�Ӧh����: ";
    p2.output();

    cout << "�ۥ[���G: ";
    sum.output();

    return 0;
}
