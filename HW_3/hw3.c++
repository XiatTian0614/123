#include <iostream>
#include <cmath>
using namespace std;

// �e�V�ŧi Polynomial ���O�A�]�� Term �ݭn���D��
class Polynomial;

// Term ���O - ��ܦh����������@��
class Term {
    friend class Polynomial; // ���\ Polynomial �s���p������
private:
    float coef; // �Y��
    int exp;    // ����
    Term* link; // ���V�U�@�Ӷ������w
    
    // �c�y��ơA��l�ƫY�ơB���ƩM�챵
    Term(float c, int e, Term* next = nullptr) : coef(c), exp(e), link(next) {}
    
public:
    // ����Y��
    float getCoef() const { return coef; }
    // �������
    int getExp() const { return exp; }
    // ����U�@�Ӹ`�I���w
    Term* getLink() const { return link; }
    // �]�m�U�@�Ӹ`�I���w
    void setLink(Term* next) { link = next; }
    // �]�m�Y��
    void setCoef(float c) { coef = c; }
};


class Polynomial {
private:
    Term* head; // �Y�`�I���w
    static Term* avail; 
    
    // ����s�`�I�]�q�i�ΦC��ηs�ء^
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
    
    // �^���`�I��i�ΦC��
    void RetNode(Term* x) {
        x->setLink(avail);
        avail = x;
    }
    
    // �M�Ŧh�����A����Ҧ��`�I
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
    // �q�{�c�y���
    Polynomial() : head(nullptr) {}
    
    // �����c�y���
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
    
    // ��ȹB��ŭ���
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
        
        // �M��X�A�����J��m
        while (current != head && current->getExp() > e) {
            prev = current;
            current = current->getLink();
        }
        
        // �p�G���Ƥw�s�b�A�X�֦P����
        if (current != head && current->getExp() == e) {
            current->setCoef(current->getCoef() + c);
            // �p�G�X�֫�Y�Ƭ�0�A�R���Ӷ�
            if (current->getCoef() == 0) {
                prev->setLink(current->getLink());
                RetNode(current);
            }
        } else {
            // ���J�s��
            prev->setLink(GetNode(c, e, current));
        }
    }
    
    // �h�����[�k�B��ŭ���
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        
        Term* aCurrent = (head == nullptr) ? nullptr : head->getLink();
        Term* bCurrent = (b.head == nullptr) ? nullptr : b.head->getLink();
        
        // �X�֨�Ӧh��������
        while ((aCurrent != nullptr && aCurrent != head) || 
            (bCurrent != nullptr && bCurrent != b.head)) {
            float coef = 0;
            int exp = 0;
            
            // �B�z a ��e�����Ƥj�� b ��e�����ƪ����p
            if (aCurrent != nullptr && aCurrent != head && 
                (bCurrent == nullptr || bCurrent == b.head || aCurrent->getExp() > bCurrent->getExp())) {
                coef = aCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
            } 
            // �B�z b ��e�����Ƥj�� a ��e�����ƪ����p
            else if (bCurrent != nullptr && bCurrent != b.head && 
                    (aCurrent == nullptr || aCurrent == head || bCurrent->getExp() > aCurrent->getExp())) {
                coef = bCurrent->getCoef();
                exp = bCurrent->getExp();
                bCurrent = bCurrent->getLink();
            } 
            // �B�z���ƬۦP�����p�]�ۥ[�Y�ơ^
            else {
                coef = aCurrent->getCoef() + bCurrent->getCoef();
                exp = aCurrent->getExp();
                aCurrent = aCurrent->getLink();
                bCurrent = bCurrent->getLink();
            }
            
            // �u���Y�Ƥ���0�~���J���G
            if (coef != 0) {
                result.Insert(coef, exp);
            }
        }
        
        return result;
    }
    
    // �h������k�B��ŭ���
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        
        Term* aCurrent = (head == nullptr) ? nullptr : head->getLink();
        Term* bCurrent = (b.head == nullptr) ? nullptr : b.head->getLink();
        
        // �����[�k�A���B�z��k�޿�
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
                coef = -bCurrent->getCoef(); // �`�N�Y�ƨ��t
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
    
    // �h�������k�B��ŭ���
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        
        if (head == nullptr || b.head == nullptr) return result;
        
        Term* aCurrent = head->getLink();
        // �� a ���C�@��
        while (aCurrent != head) {
            Term* bCurrent = b.head->getLink();
            // �P b ���C�@���ۭ�
            while (bCurrent != b.head) {
                float coef = aCurrent->getCoef() * bCurrent->getCoef();
                int exp = aCurrent->getExp() + bCurrent->getExp(); // ���Ƭۥ[
                result.Insert(coef, exp);
                bCurrent = bCurrent->getLink();
            }
            aCurrent = aCurrent->getLink();
        }
        
        return result;
    }
    
    // �p��h�����b x �B����
    float Evaluate(float x) const {
        float result = 0;
        
        if (head == nullptr) return result;
        
        Term* current = head->getLink();
        while (current != head) {
            // �p��C�@�����Ȩò֥[
            result += current->getCoef() * pow(x, current->getExp());
            current = current->getLink();
        }
        
        return result;
    }
    
    // �ͤ���ƫŧi�A�Ω��J��X
    friend istream& operator>>(istream& is, Polynomial& x);
    friend ostream& operator<<(ostream& os, const Polynomial& x);
};

// ��l���R�A���� avail
Term* Polynomial::avail = nullptr;

// ��J�B��ŭ���
istream& operator>>(istream& is, Polynomial& x) {
    x.Clear(); // �M�Ų{���h����
    
    int n;
    is >> n; // Ū������
    
    if (n > 0) {
        x.head = x.GetNode(0, -1, nullptr);
        x.head->setLink(x.head);
        
        // Ū���C�@�����Y�ƩM����
        for (int i = 0; i < n; i++) {
            float coef;
            int exp;
            is >> coef >> exp;
            x.Insert(coef, exp);
        }
    }
    
    return is;
}

// ��X�B��ŭ���
ostream& operator<<(ostream& os, const Polynomial& x) {
    // �B�z�Ŧh����
    if (x.head == nullptr || x.head->getLink() == x.head) {
        os << "0";
        return os;
    }
    
    Term* current = x.head->getLink();
    bool firstTerm = true;
    
    // ��X�C�@��
    while (current != x.head) {
        // �B�z���Y�ƫe���[��
        if (!firstTerm && current->getCoef() > 0) {
            os << "+";
        }
        
        // �B�z�`�ƶ�
        if (current->getExp() == 0) {
            os << current->getCoef();
        } else {
            // �B�z�Y�Ƭ�1��-1���S���p
            if (current->getCoef() != 1 && current->getCoef() != -1) {
                os << current->getCoef();
            } else if (current->getCoef() == -1) {
                os << "-";
            }
            
            os << "x";
            
            // �B�z���Ƭ�1���S���p
            if (current->getExp() != 1) {
                os << "^" << current->getExp();
            }
        }
        
        firstTerm = false;
        current = current->getLink();
    }
    
    return os;
}

// �D���
int main() {
    Polynomial p1, p2;
    
    cout << "��J�Ĥ@�Ӧh���� (��J: n c1 e1 c2 e2 ...): ";
    cin >> p1;
    
    cout << "��J�ĤG�Ӧh���� (��J: n c1 e1 c2 e2 ...): ";
    cin >> p2;
    
    cout << "\n�Ĥ@�Ӧh����: " << p1 << endl;
    cout << "�ĤG�Ӧh����: " << p2 << endl;
    
    Polynomial sum = p1 + p2;
    cout << "�`�M: " << sum << endl;
    
    Polynomial diff = p1 - p2;
    cout << "�t��: " << diff << endl;
    Polynomial product = p1 * p2;
    cout << "�n��: " << product << endl;
    float x;
    cout << "\n��J�n�p��h�������� x: ";
    cin >> x;
    
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "p2(" << x << ") = " << p2.Evaluate(x) << endl;
    
    return 0;
}