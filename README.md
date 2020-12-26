# FM-index
FM-index

## 测试运行

运行 `test.cpp` 即可

## BWT

Burrows-Wheeler Transform 的 C++ 实现，详见 `BWT.h` 文件，包含以下功能

+ `BWT(string origin)`：构造函数，将传入的原文 `origin` 编码成索引，构造一个 `BWT` 对象
+ `string encode(string origin)`：将传入的原文 `origin` 编码成索引并返回
+ `string decode()`：将该 `BWT` 对象上一次编码得到的索引解码并返回
+ `string decode(string index)`：将传入的索引 `index` 解码并返回
+ `vector<int> match(string pattern)`：在该 `BWT` 对象上一次编码的字符串中匹配 `pattern` 的位置，返回所有下标
+ `vector<int> match(string origin, string pattern)`：在传入的 `origin` 中匹配 `pattern` 的位置，返回所有下标

## utils.h

```C++
/** 随机生成长度为 length 的字符串，字符集为 {a-z} */
string strRand(const int length)

/** Move-To-Front transform 编码 */
string MTFEncode(const string &str)

/** Move-To-Front transform 解码 */
string MTFDecode(string code)

/** Run-Length Encode */
string RLEncode(string chars)

/** Run-Length Decode */
string RLDecode(string code)
```