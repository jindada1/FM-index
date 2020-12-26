
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

    string code = MTFEncode(text);

    cout << text << endl;

    cout << code << endl;

    cout << MTFDecode(code) << endl;
}

void test_RLE()
{
    string a = "aaaaaaaabbbb";
    
    string code = RLEncode(a);
    cout << code << endl;

    cout << RLDecode(code) << endl;
}

int main()
{
    // test_MTF();
    // test_RLE();

    BWT bwtInstance;

    const int length = 10;
    string text = "bananabananabananabananabananabanana";
    cout << "origin text: " << text << endl;

    // BWT 变换
    string bwtIndex = bwtInstance.encode(text);
    cout << "BWT encode: " << bwtIndex << endl;

    // cout << RLEncode(bwtIndex) << endl;

    // MTF 编码
    string mtfCode = MTFEncode(bwtIndex);
    cout << "MTF encode: " << mtfCode << endl;

    // RLE 压缩
    string rleCode = RLEncode(mtfCode);
    cout << "RLE compress: " << rleCode << endl;

    // RLE 解压
    string DecompressedCode = RLDecode(rleCode);
    cout << "RLE decompress" << DecompressedCode << endl;

    // MTF 解码
    string decodedIndex = MTFDecode(DecompressedCode);
    cout << "MTF decode: " << decodedIndex << endl;

    // BWT 解码
    string originText = bwtInstance.decode(decodedIndex);
    cout << "BWT decode: " << originText << endl;

    return 1;
}
