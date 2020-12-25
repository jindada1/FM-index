#ifndef sa_h
#define sa_h
#define max_char 999
#include<vector>
#include<string>
#include<numeric>
using std::vector;
using std::string;
inline void radix_sort(vector<int> &s, vector<int> &a, vector<int> &b, vector<int> &c)
{
	fill(c.begin(), c.end(), 0);
	for(int i = a.size(); i--; ++c[s[a[i]]]);
	partial_sum(c.begin(), c.end(), c.begin());
	for(int i = a.size(); i--; b[--c[s[a[i]]]] = a[i]);
}
inline vector<int> suffix_array(string s)
{
	int n = s.size();
	vector<int> a(n*2, 0), b(n*2, 0), c(n+1 > max_char ? n+1 : max_char, 0);
	vector<int> tmp(n, 0), sa(n, 0);
	copy(s.begin(), s.end(), a.begin());
	for(int len = 1; len / 2 < n; len *= 2)
	{
		copy(a.begin() + len/2, a.end(), b.begin());
		iota(sa.begin(), sa.end(), 0);
		radix_sort(b, sa, tmp, c);
		radix_sort(a, tmp, sa, c);
		tmp[sa[0]] = 1;
		for(int i = 1; i < n; ++i)
			tmp[sa[i]] = tmp[sa[i-1]] + (a[sa[i]] != a[sa[i-1]] || b[sa[i]] != b[sa[i-1]]);
		copy(tmp.begin(), tmp.end(), a.begin());
		if(tmp[sa[n-1]] == n)
			break;
	}
	return sa;
}
#endif
