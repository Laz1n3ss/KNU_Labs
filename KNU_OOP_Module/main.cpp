#include <iostream>
#include <cstdio>
#include <unordered_map>

using namespace std;

int main()
{
    freopen ("input.txt", "r", stdin);

    unordered_map<char, int> charCount;

    char c;
    while(cin >> c) {
        if(c >= 'A' && c <= 'Z' || c == '-')
            charCount[c]++;
    }

    for(pair<char, int> el: charCount)
        cout << el.first << ": " << el.second << "\n";
    return 0;
}
