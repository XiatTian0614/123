#include <iostream>
#include <cstring>

using namespace std;

void printSubset(char* subset, int length) {    // 輸出子集合的函數
    cout << "(";
    for (int i = 0; i < length; ++i) {
        cout << subset[i];                      // 輸出子集合中的每個元素
    }
    cout << ")" << endl;                        // 輸出子集合結束
}

void generatePowerSet(char* S, int n, char* subset, int subsetSize, int index) {    // 遞迴產生 power set 的函數
    if (index == n) {   // 遞迴終止條件：當 index ==n ，列印目前子集合
        printSubset(subset, subsetSize);
        return;
    }

    // 遞迴情況 1：不包含當前元素 S[index]
    generatePowerSet(S, n, subset, subsetSize, index + 1);
    // 遞迴情況 2：包含當前元素 S[index]
    subset[subsetSize] = S[index];      // 加入當前元素到子集合中
    generatePowerSet(S, n, subset, subsetSize + 1, index + 1);
}

int main() {
    char S[] = {'a', 'b', 'c'};
    int n = sizeof(S) / sizeof(S[0]);
    char subset[10]; 
    generatePowerSet(S, n, subset, 0, 0);

    return 0;
}
