
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
    string origin = transformer.decode(index);

    if (input == origin)
    {
        cout << "[ ok]" << endl;    
    }
    else
    {
        cout << "[err]" << endl;
    }
}

int main()
{
    int len = 10;
    BWT bwtInstance;

    while (len < 10000)
    {
        // 随机生成测试用例
        test(bwtInstance, strRand(len));
        len += len;
    }
}