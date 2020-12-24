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
    // 编码生成的索引
    std::string index = "";
    // 索引中第 i 个字符在之前出现过几次
    int* rankofCharAt;
    // 排好序的索引中字典序小于字符 c 的所有字符个数
    std::map<char, int> numofSmaller;
    // 克隆一个排序后的副本
    std::string __cloneSorted(std::string index);

    int* getRankofCharAt(std::string _index);

    std::map<char, int> getNumofSmaller(std::string _sortedindex);
    
    std::string __decode(std::string _index, int* _rankofCharAt, std::map<char, int>& _numofSmaller);

public:
    BWT() {};
    BWT(std::string origin) { this->encode(origin); };
    ~BWT() {};

    // 对给定的 index 编码，输入原始字符串
    std::string encode(std::string origin);

    // 不传参数就是解码上一次编码所得的索引
    std::string decode();

    // 对给定的 index 解码，输入索引
    std::string decode(std::string index);

};

/** 将原文编码成索引 */
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

    // 索引
    index = "";
    // 排序好的索引
    std::string sortedindex = "";

    // 获取排序后的移位矩阵最后一列
    for (auto row : sortedCyclicShiftedMatrix)
    {
        sortedindex += row.substr(0, 1);
        index += row.substr(length - 1, 1);
    }

    // 准备好辅助信息，便于后续操作
    this->rankofCharAt = this->getRankofCharAt(index);
    this->numofSmaller = this->getNumofSmaller(sortedindex);

    return index;
}

/** 从输入的 index 解码出原文 */
std::string BWT::decode(std::string _index)
{
    int* _rankofCharAt = this->getRankofCharAt(_index);

    std::string _sortedindex = this->__cloneSorted(_index);
    std::map<char, int> _numofSmaller = this->getNumofSmaller(_sortedindex);

    return this->__decode(_index, _rankofCharAt, _numofSmaller);
}

/** 解码上一次编码得到的索引 */
std::string BWT::decode()
{
    // 如果之前没有编码，即没有索引，就返回空字符串
    if (this->index == "") return "";

    // 否则解码上一次编码的结果
    return this->__decode(this->index, this->rankofCharAt, this->numofSmaller);
}

/** 索引中第 i 个字符在之前出现过几次 */
int* BWT::getRankofCharAt(std::string _index)
{
    int length = _index.size();
    // _index 中的第几个 c
    std::map<char, int> _rankofChar;
    // _index 的第 i 个字符在之前出现过几次
    int* _rankofCharAt = new int[length];

    for (int i = 0; i < length; i++)
    {
        // 获取字符
        char c = _index[i];
        // 第一次遇见 c，这个 c 是第 0 个
        if(_rankofChar.count(c) == 0) _rankofChar[c] = 0;
        else _rankofChar[c] += 1;

        _rankofCharAt[i] = _rankofChar[c];
    }

    return _rankofCharAt;
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
std::string BWT::__decode(std::string _index, int* _rankofCharAt, std::map<char, int>& _numofSmaller)
{
    /**
     * 准备好 _rankofCharAt 和 _numofSmaller 之后，就可以进行映射（本质就是一个定位）
     * 
     * 比如索引 index 中的第 i 个字符 c，就是 sortedindex 中下标为 _rankofCharAt[i] + _numofSmaller[c] 的字符
     */

    // 开始解码索引
    int i = 0;
    std::string origin = "";
    while(_index[i] != '$')
    {
        origin.push_back(_index[i]);
        // 跳转到 sortedindex 中字符 index[i] 的下标
        i = _rankofCharAt[i] + _numofSmaller[_index[i]];
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