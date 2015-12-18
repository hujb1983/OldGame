#ifndef _YOND_STRING_H_INCLUDED_
#define _YOND_STRING_H_INCLUDED_

#include <Utility.h>
#include <iostream>
using namespace std;

class Yond_string
{
public:
	// 默认构造函数
	Yond_string();
	// C字符串初始化
	Yond_string( const char * str );
	// 拷贝构造函数
	Yond_string( const Yond_string & str );

	// 析构函数
	~Yond_string();

	// 输入输出
	friend istream & operator >> ( istream & in,  Yond_string & str );
	friend ostream & operator << ( ostream & out, Yond_string & str );
	// +
	friend Yond_string operator + ( const Yond_string & s1, const Yond_string & s2 );
	// 比较
	friend bool operator == ( const Yond_string & s1, const Yond_string & s2 );
	friend bool operator != ( const Yond_string & s1, const Yond_string & s2 );
	friend bool operator <  ( const Yond_string & s1, const Yond_string & s2 );
	friend bool operator <= ( const Yond_string & s1, const Yond_string & s2 );
	friend bool operator >  ( const Yond_string & s1, const Yond_string & s2 );
	friend bool operator >= ( const Yond_string & s1, const Yond_string & s2 );

	// 下标操作符
	char & operator [] ( const int index );
	const char & operator [] ( const int index ) const;
	// =
	Yond_string & operator = ( const Yond_string & str );
	// +=
	Yond_string & operator += ( const Yond_string & str );

	// 是否为空
	bool empty();
	// 获取长度
	int length();
	//
	const char * c_str();

	// 子串
	Yond_string substr(  int pos, int n );
	// 添加
	Yond_string & append( const Yond_string & str );
	// 插入
	Yond_string & insert( int pos, const Yond_string & str );
	// 替换
	Yond_string & assign( const Yond_string & str, int pos, int len );
	// 删除
	Yond_string & erase( int pos, int len );

	// 忽略大小写比较2个字符串大小
	int CompareNoCase( const char * s1 );

	// 取得分组队列
    int GetByteArray(BYTE * buff, int uiSize, char div=',');
    int GetIntArray(int * buff, int uiSize, char div=',');

    // 设置分组队列
    int SetByteArray(BYTE * buff, int uiSize, char div=',');

private:
	char * m_pData;
	int m_nLength;
};


#endif // _Yond_string_H_INCLUDED_
