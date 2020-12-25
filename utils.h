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

using std::default_random_engine;
using std::random_device;
using std::string;

/**
 * @Author: Kris Huang, 
 * @Date: 2020-12-24
 * @Description: 随机生成长度为 length 的字符串，字符集为 {a-z}
 * 
 * @References: https://blog.csdn.net/lfod1997/article/details/107749492
 */
string strRand(int length)
{                  // length: 产生字符串的长度
    char tmp;      // tmp: 暂存一个随机数
    string buffer; // buffer: 保存返回值

    // 下面这两行比较重要:
    random_device rd;                   // 产生一个 std::random_device 对象 rd
    default_random_engine random(rd()); // 用 rd 初始化一个随机数发生器 random

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
void MTFEncode(const string &str, int *result)
{
    string alphalist = "abcdefghijklmnopqrstuvwxyz";
    int pos = 0;
    for (int i = 0; i < str.size(); i++)
    {
        // 查找当前字符 str[i] 的下标
        pos = alphalist.find(str[i]);

        // 把 pos 位置上的字母移到第一位
        alphalist.erase(pos, 1);
        alphalist.insert(0, 1, str[i]);

        // 记录当前的位置 pos，也就是字符 str[i] 对应的编码
        result[i] = pos;
    }
}

string MTFDecode(int *code, int size)
{
    string alphalist = "abcdefghijklmnopqrstuvwxyz";
    string result = "";
    
    for (int i = 0; i < size; i++)
    {
        // 在 alphalist 中查找下标为 code[i] 的字符，加到结果中
        result += alphalist[code[i]];

        // 把 code[i] 位置上的字母移到第一位
        alphalist.erase(code[i], 1);
        alphalist.insert(0, 1, result[i]);
    }

    return result;
}

#endif