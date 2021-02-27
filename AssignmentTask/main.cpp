#include <iostream>
#include <cstdlib>

#include "hungary.cpp"
#include "mack.cpp"

#include "method.h"

using namespace std;

int main()
{
    int type;
    MethodBase* method = nullptr;
    do {
        cout << "Enter method (1 - Hungary, 2 - Mack)\n";
        cin >> type;

        if(type == 1)
            method = new HungaryMethod();
        else if(type == 2)
            method = new MackMethod();
    } while(type != 1 && type != 2);

    method->Start();
    system("pause");
    return 0;
}
