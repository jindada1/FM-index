
#include <iostream>
#include <string>
#include "BWT.h"
#include "utils.h"

using namespace std;

/***
 * 测试
**/
void test_BWT_encode_decode()
{
    int len = 10;
    BWT bwtInstance;

    while (len < 10000)
    {
        // 随机生成测试用例
        string input = strRand(len);
        // 编码
        string index = bwtInstance.encode(input);
        // 再解码
        string decode = bwtInstance.decode();

        cout << (input == decode ? "[ ok ]" : "[err ]") << endl;
        len += len;
    }
}

void test_BWT_match()
{
    BWT bwtInstance2("gowwwwww");

    cout << bwtInstance2.getIndex() << endl;

    for (auto pos:bwtInstance2.match("googol", "gol")) cout << pos << ", ";
}

void test_MTF()
{
    string text = "bananaaa";

    cout << text << endl;

    int *result = new int[text.size()];

    MTFEncode(text, result);

    for (size_t i = 0; i < text.size(); i++)
    {
        cout << result[i] << ", ";
    }
    cout << endl;
    
    cout << MTFDecode(result, text.size()) << endl;
}

int main()
{
    test_MTF();
    return 1;
}