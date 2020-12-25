
#include <iostream>
#include <string>
#include "BWT.h"
#include "utils.h"

using namespace std;

/***
 * 测试
**/
void test(BWT& transformer, string input)
{
    string index = transformer.encode(input);
    string origin = transformer.decode();

    cout << (input==origin? "[ ok ]" : "[err ]") << endl;
}

int main()
{
    int len = 10;
    BWT bwtInstance("acaacg");

    string index = bwtInstance.getIndex();

    cout << index << endl;

    // 对于 _occurrence 中的所有字符
    for(auto &charOcc : bwtInstance.getOccurrence(index))
    {
        cout << charOcc.first << ": ";
        for (size_t i = 0; i < index.size(); i++)
        {
            cout << charOcc.second[i] << ", ";
        }
        cout << endl;
    }

    cout << bwtInstance.match("ca") << endl;

    return 1;    
}