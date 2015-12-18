#ifndef _JSONPARSER_H_INCLUDED_
#define _JSONPARSER_H_INCLUDED_

#include "Yond_string.h"
#include "Yond_json.h"

class JsonParser
{
	enum
	{
		eUNKNOWN 		= 0x01,
		ePROTOCOL 		= 0x02,
	};

public:
	JsonParser();
	virtual ~JsonParser();

	int ParseJson( const char * pInput );

	DWORD GetProtocol();

private:

	DWORD   dwProtocol;

	DWORD   dwStatus;
};


/*
 *   JsonBuffer
 */

class JsonMap
{
public:
    JsonMap();
    virtual ~JsonMap();

    int set_json(cJSON * in_json);
    int set_json(const char * in_buff);

protected:
    cJSON * m_jsRoot;

public:
    int ReadString(const char * in_name, char * out_data, int in_size);
    int ReadDouble(const char * in_name, double & out_data);
    int ReadInteger(const char * in_name, int & out_data);
    int ReadLongLong(const char * in_name, INT64 & out_data);

public:
    int ReadJson(const char * in_name, JsonMap & out_data);
    int ReadArrayJson( int in_index, JsonMap & out_data );
    int GetArraySize();
 };



/*
 *   JsonBuffer
 */

class JsonBuffer
{
public:
    JsonBuffer(const char * in_buff = NULL);
    JsonBuffer(const char * in_buff, int in_size);
    JsonBuffer(JsonBuffer & in_buff);
    virtual ~JsonBuffer();

    // Json Length;
    void clear_buff();
    void empty();
    int length();

    // Json package
    void package_array();
    void package_one();

    void set_json(cJSON * in_json);
    void set_json(const char * in_buff);
    const char * get_json();

protected:
    cJSON * m_jsRoot;
    Yond_string m_yJson;

public:
    int ReadString(const char * in_name, char * out_data, int in_size);
    int ReadDouble(const char * in_name, double & out_data);
    int ReadInteger(const char * in_name, int & out_data);
    int ReadLongLong(const char * in_name, INT64 & out_data);
    int ReadJson(const char * in_name, JsonBuffer & out_data);

    int WriteString(const char * in_name, char * out_data);
    int WriteDouble(const char * in_name, double out_data);
    int WriteInteger(const char * in_name, int out_data);
    int WriteLongLong(const char * in_name, INT64 out_data);
    int WriteJson( JsonBuffer & out_data );
    int WriteJson(const char * in_name, JsonBuffer & out_data);
};


/*
 *   JsonBuffer
 */
class JsonHeader
{
public:
    JsonHeader();
    ~JsonHeader();

public:
    int Parser( const char * json_buff, int json_size   );

    WORD GetUserkey();
    WORD GetUserid();

private:
	int m_iUserid;
	int m_iUserkey;
};


#endif // _JSON_STRING_H_INCLUDED_
