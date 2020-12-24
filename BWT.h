/*******************************************************************************
 *
 * @Author: Kris Huang, 
 * @Date: 2020-12-24
 * @Description: implementation of The Burrows-Wheeler Transform
 *
 *
 * @References:
 *
 * 1. https://alexbowe.com/fm-index/
 * 2. https://zhuanlan.zhihu.com/p/88263062
 * 3. https://blog.csdn.net/stormlovetao/article/details/7048481
 *
 ******************************************************************************/
 
#ifndef BWT_H__
#define BWT_H__

#include <map>
#include <string>
#include <vector>
#include <algorithm>

class BWT
{
private:
    // 克隆一个排序后的副本
    std::string sorted(std::string index);

public:
    BWT() {};
    ~BWT() {};

    // 对给定的 index 编码，输入原始字符串
    std::string encode(std::string origin);

    // 对给定的 index 解码，输入索引
    std::string decode(std::string index);

};

/**
 * 将原文编码成索引
 */
std::string BWT::encode(std::string origin)
{
    std::string row = "$" + origin;
    int length = row.size();

    // 编码过程中使用的排序后的轮转位移矩阵
    std::vector<std::string> sortedCyclicShiftedMatrix;
    for (int i = 0; i < length; i++)
    {
        // 添加一行
        sortedCyclicShiftedMatrix.push_back(row);
        // 左移一位
        row = row.substr(length - 1, 1) + row.substr(0, length - 1);
    }
    
    // 按字母表顺序为字符串排序，得到排序后的移位矩阵
    std::sort(sortedCyclicShiftedMatrix.begin(), sortedCyclicShiftedMatrix.end());

    std::string index;

    // 获取排序后的移位矩阵最后一列
    for (auto row : sortedCyclicShiftedMatrix)
    {
        index += row.substr(length - 1, 1);
    }

    return index;
}

/**
 * 从输入的 index 解码出原文
 */
std::string BWT::decode(std::string index)
{
    int length = index.size();

    // index 中的第几个 c
    std::map<char, int> rankofChar;
    // index 的第 i 个字符在之前出现过几次
    int* rankofCharAt = new int[length];

    for (int i = 0; i < length; i++)
    {
        // 获取字符
        char c = index[i];
        // 第一次遇见 c，这个 c 是第 0 个
        if(rankofChar.count(c) == 0) rankofChar[c] = 0;
        else rankofChar[c] += 1;

        rankofCharAt[i] = rankofChar[c];
    }

    std::string sortedindex = this->sorted(index);

    // sortedindex 中字典序小于字符 c 的所有字符个数
    std::map<char, int> NumofSmaller;

    for (int i = 0; i < length; i++)
    {
        // 获取字符
        char c = sortedindex[i];
        // 第一次遇见，记录一下此时的位置 i，也就是比它小的字符总数
        if(NumofSmaller.count(c) == 0) NumofSmaller[c] = i;
    }
    
    /**
     * 准备好 rankofCharAt 和 NumofSmaller 之后，就可以进行映射
     * 
     * 比如索引 index 中的第 i 个字符 c，就是 sortedindex 中下标为 rankofCharAt[i] + NumofSmaller[c] 的字符
     */

    // 开始解码索引
    int i = 0;
    std::string origin = "";
    while(index[i] != '$')
    {
        origin.push_back(index[i]);
        // 跳转到 sortedindex 中字符 index[i] 的下标
        i = rankofCharAt[i] + NumofSmaller[index[i]];
    }

    // 反转一下
    origin.assign(origin.rbegin(), origin.rend());

    return origin;
}

std::string BWT::sorted(std::string s)
{
    std::sort(s.begin(), s.end());
    return s;
}

#endif