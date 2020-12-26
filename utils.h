/*******************************************************************************
 *
 * @Author: Kris Huang, 
 * @Date: 2020-12-24
 * @Description: tool functions
 *
 ******************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <random>
#include <ctype.h>

using std::string;

/**
 * @Author: Kris Huang, 
 * @Date: 2020-12-24
 * @Description: 随机生成长度为 length 的字符串，字符集为 {a-z}
 * 
 * @References: https://blog.csdn.net/lfod1997/article/details/107749492
 */
string strRand(const int length)
{                  // length: 产生字符串的长度
    char tmp;      // tmp: 暂存一个随机数
    string buffer; // buffer: 保存返回值

    // 下面这两行比较重要:
    std::random_device rd;                   // 产生一个 std::random_device 对象 rd
    std::default_random_engine random(rd()); // 用 rd 初始化一个随机数发生器 random

    for (int i = 0; i < length; i++)
    {
        tmp = random() % 26; // 随机一个小于 26 的整数，对应 a-z 共 26 个字符
        tmp += 'a';
        buffer += tmp;
    }
    return buffer;
}

/**
 * @Author: Kris Huang, 
 * @Date: 2020-12-25
 * @Description: implementation of Move-To-Front transform
 * 
 * @References: https://www.cnblogs.com/xudong-bupt/p/3763916.html
 */

const string alphalist = "abcdefghijklmnopqrstuvwxyz$";

string MTFEncode(const string &str)
{
    string _list(alphalist);
    string result = "";
    int pos = 0;
    for (int i = 0; i < str.size(); i++)
    {
        // 查找当前字符 str[i] 的下标
        pos = _list.find(str[i]);

        // 把 pos 位置上的字母移到第一位
        _list.erase(pos, 1);
        _list.insert(0, 1, str[i]);

        // 根据当前的位置 pos，转换成 char 记录下来，也就是字符 str[i] 对应的编码
        result += alphalist[pos];
    }

    return result;
}

string MTFDecode(string code)
{
    string _list(alphalist);
    string result = "";
    int pos;

    for (int i = 0; i < code.size(); i++)
    {
        pos = alphalist.find(code[i]);
        // 在 _list 中查找下标为 pos 的字符，加到结果中
        result += _list[pos];

        // 把 pos 位置上的字母移到第一位
        _list.erase(pos, 1);
        _list.insert(0, 1, result[i]);
    }

    return result;
}

/**
 * @Author: Kris Huang, 
 * @Date: 2020-12-26
 * @Description: implementation of Run-Length Encode and Decode.
 *   - RLEncode: input string, return compressed string. [aaaaaaaabbbb -> 8a4b]
 *   - RLDecode: input compressed string, return string. [8a4b -> aaaaaaaabbbb]
 */
string RLEncode(string chars)
{
    if (chars.size() == 0) return "";
    // 保存编码后的结果
    string result = "";
    // 从第一个字母开始
    char last = chars[0];
    int count = 0;

    for(auto c: chars)
    {
        // 当前的字母和上一个字母一样，个数加一
        if (last == c) count += 1;
        // 当前的字母 != 上一个字母，上一个字母统计结束，开始统计下一个字母
        else
        {
            // 记录上一个字母的数量
            result += std::to_string(count);
            // 记录上一个字母
            result += last;
            // 准备记录下一个字母
            last = c;
            count = 1;
        }
    }
    result += std::to_string(count);
    result += last;

    return result;
}

string RLDecode(string code)
{
    if (code.size() == 0) return "";
    // 保存解码后的结果
    string result = "";

    int count = 0;
    // 从第一个字母开始
    for(auto c: code)
    {
        // 如果是数字，开始记录出现的次数，注意将 char 转成 int
        if (isdigit(c)) count = 10 * count + (c - '0');
        // 解析到字母时，将 count 个 c 加到结果后面
        else
        {
            string sub(count, c);
            result += sub;
            count = 0;
        }
    }

    return result;
}

#endif