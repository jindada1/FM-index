
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

    // BWT bwtInstance;

    // while (len < 10000)
    // {
    //     // 随机生成测试用例
    //     test(bwtInstance, strRand(len));
    //     len += len;
    // }

    BWT bwtInstance2("googol");

    cout << bwtInstance2.getIndex() << endl;

    for (auto pos:bwtInstance2.match("gol")) cout << pos << ", ";

    return 1;
}