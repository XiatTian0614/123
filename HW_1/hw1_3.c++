#include <iostream>
#include <cstring>

using namespace std;


void printSubset(char* subset, int length) {
    cout << "(";
    for (int i = 0; i < length; ++i) {
        cout << subset[i];
    }
    cout << ")" << endl;
}


void generatePowerSet(char* S, int n, char* subset, int subsetSize, int index) {
    if (index == n) {
        printSubset(subset, subsetSize);
        return;
    }


    generatePowerSet(S, n, subset, subsetSize, index + 1);


    subset[subsetSize] = S[index];
    generatePowerSet(S, n, subset, subsetSize + 1, index + 1);
}

int main() {
    char S[] = {'a', 'b', 'c'};
    int n = sizeof(S) / sizeof(S[0]);
    char subset[10]; 
    generatePowerSet(S, n, subset, 0, 0);

    return 0;
}
