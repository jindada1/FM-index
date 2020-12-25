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
#include "sa.h"

class BWT
{
private:
    // 编码生成的索引
    std::string index = "";

    // 索引中到第 i 行为止（包括第 i 行），字符 c 出现了多少次
    std::map<char, int*> occurrence;
    std::map<char, int*> getOccurrence(std::string _index);

    // 排好序的索引中字典序小于字符 c 的所有字符个数
    std::map<char, int> numofSmaller;
    std::map<char, int> getNumofSmaller(std::string _sortedindex);

    // 索引中的第 i 个字符对应了原字符串的第 SA[i] 个字符，用来将匹配结果转换至原字符串 
    std::vector<int> SA;

    // 克隆一个排序后的副本
    std::string __cloneSorted(std::string index);
    // 传入所有解码所需的数据后，返回解码结果
    std::string __decode(std::string _index, std::map<char, int*>& _occurrence, std::map<char, int>& _numofSmaller);

public:
    BWT() {};
    BWT(std::string origin) { this->encode(origin); };
    ~BWT() {};

    // 对给定的 index 编码，输入原始字符串
    std::string encode(std::string origin);
    
    // 返回上一次编码所得的索引
    std::string getIndex() { return this->index; };

    // 不传参数就是解码上一次编码所得的索引
    std::string decode();

    // 对给定的 index 解码，输入索引
    std::string decode(std::string index);

    // 根据上一次编码所得的索引，匹配子串 pattern 出现的位置
    std::string match(std::string pattern);

    // 根据输入的索引，匹配子串 pattern 出现的位置
    std::string match(std::string _index, std::string pattern);

};

/** 将原文编码成索引 */
std::string BWT::encode(std::string origin)
{
    std::string row = origin + "$";
    int length = row.size();

    this->SA = suffix_array(row);
    index = "";
    std::string sortedindex = "";
    for(auto w : this->SA)
        index.push_back(w ? row[w-1] : '$'), sortedindex.push_back(row[w]);

    // 准备好辅助信息，便于后续操作
    this->occurrence = this->getOccurrence(index);
    this->numofSmaller = this->getNumofSmaller(sortedindex);

    return index;
}

/** 从输入的 index 解码出原文 */
std::string BWT::decode(std::string _index)
{
    std::map<char, int*> _occurrence = this->getOccurrence(_index);

    std::string _sortedindex = this->__cloneSorted(_index);
    std::map<char, int> _numofSmaller = this->getNumofSmaller(_sortedindex);

    return this->__decode(_index, _occurrence, _numofSmaller);
}

/** 解码上一次编码得到的索引 */
std::string BWT::decode()
{
    // 如果之前没有编码，即没有索引，就返回空字符串
    if (this->index == "") return "";

    // 否则解码上一次编码的结果
    return this->__decode(this->index, this->occurrence, this->numofSmaller);
}

/** 根据上一次编码所得的索引，匹配子串 pattern 出现的位置 */
std::string BWT::match(std::string pattern)
{
    // 如果之前没有编码，即没有索引，就返回空
    if (this->index == "") return nullptr;
    // 获取索引长度
    int len = this->index.size();
    // 准备好 Occ
    std::map<char, int*> _occurrence = this->getOccurrence(this->index);

    // 从 pattern 的最后一位字符开始匹配
    int cur = pattern.size() - 1;
    /** (left, right] 是匹配范围，初始化为最大范围，随和匹配的进行逐步缩小 */
    int left = 0;
    int right = len - 1;

    while (left <= right && cur >= 0)
    {
        char c = pattern[cur];
        // 如果 left 位置上也是 c
        left = this->numofSmaller[c] + _occurrence[c][left] - (this->index[left] == c);
        right = this->numofSmaller[c] + _occurrence[c][right] - 1;
        cur -= 1;
    }
    
    return std::to_string(left) + std::to_string(right);
}

/** 根据输入的索引，匹配子串 pattern 出现的位置 */
std::string BWT::match(std::string _index, std::string pattern)
{
    return "";
}

/** 索引中到第 i 行为止（包括第 i 行），字符 c 出现了多少次 */
std::map<char, int*> BWT::getOccurrence(std::string _index)
{
    int length = _index.size();
    // _index 中第 i 行之前，字符 c 出现了多少次
    std::map<char, int*> _occurrence;
    
    for (int i = 0; i < length; i++)
    {
        // 获取字符 c
        char c = _index[i];

        // 第一次遇见 c，创建关于 c 的数组，并初始化为全零
        if(_occurrence.count(c) == 0) _occurrence[c] = new int[length]{};
        
        // 对于 _occurrence 中的所有字符
        for(auto &charOcc : _occurrence)
        {
            // 先 copy 一份之前统计好的次数，只需讨论一个特殊情况即 i = 0 时
            charOcc.second[i] = i==0 ? 0 : charOcc.second[i - 1];
            // 再把现在遇到的这个字符次数 + 1
            if(charOcc.first == c) charOcc.second[i] += 1;
        }
    }

    return _occurrence;
}

/** 排好序的索引中字典序小于字符 c 的字符数 */
std::map<char, int> BWT::getNumofSmaller(std::string _sortedindex)
{
    // sortedindex 中字典序小于字符 c 的所有字符个数
    std::map<char, int> _numofSmaller;
    for (int i = 0; i < _sortedindex.size(); i++)
    {
        // 获取字符
        char c = _sortedindex[i];
        // 第一次遇见，记录一下此时的位置 i，也就是比它小的字符总数
        if(_numofSmaller.count(c) == 0) _numofSmaller[c] = i;
    }

    return _numofSmaller;
}

/** 传入所有解码所需的数据后，返回解码结果 */
std::string BWT::__decode(std::string _index, std::map<char, int*>& _occurrence, std::map<char, int>& _numofSmaller)
{
    /**
     * 准备好 _occurrence 和 _numofSmaller 之后，就可以进行映射（本质就是一个定位）
     * 
     * 比如索引 index 中的第 i 个字符 c，就是 sortedindex 中下标为 _occurrence[c][i] + _numofSmaller[c] - 1的字符
     */

    // 开始解码索引
    int i = 0;
    std::string origin = "";
    while(_index[i] != '$')
    {
        origin.push_back(_index[i]);
        // 跳转到 sortedindex 中字符 index[i] 的下标
        i = _occurrence[_index[i]][i] + _numofSmaller[_index[i]] - 1;
    }

    // 反转一下
    origin.assign(origin.rbegin(), origin.rend());

    return origin;
}

/** 根据输入的字符串，返回一个克隆的排好序的字符串 */
std::string BWT::__cloneSorted(std::string s)
{
    std::sort(s.begin(), s.end());
    return s;
}

#endif
