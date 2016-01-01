#include <Utility.h>
#include <iostream>
#include "JsonParser.h"

using std::cout;
using std::endl;

JsonParser::JsonParser()
{
	memset( this, 0, sizeof(this) );
}


JsonParser::~JsonParser()
{

}

int JsonParser::ParseJson( const char * pInput )
{
	dwStatus = eUNKNOWN;

	cJSON * root = NULL;

	root = cJSON_Parse( pInput );
	if ( root == NULL ) {
		return -1;
	}

	int nLen = 0;

	// protocol id
	cJSON * protocol = cJSON_GetObjectItem( root, "Protocol" );
	if ( protocol ) {
		dwStatus |= ePROTOCOL;
		dwProtocol = 0;
		dwProtocol = protocol->valueint;
		if ( dwProtocol == 0 )
		{
			char szProtocol[12];
			nLen = strlen( protocol->valuestring );
			if ( nLen < sizeof(szProtocol) )
			{
				memcpy( szProtocol, protocol->valuestring, nLen + 1 );
				szProtocol[ nLen ] = '\0';
				dwProtocol = atol(szProtocol);
			}
		}
	}

	if ( root ) {
		cJSON_Delete( root );
	}

	return 0;
}

DWORD JsonParser::GetProtocol() {
	return dwProtocol;
}



/*
 *   JsonMap
 */
JsonMap::JsonMap()
{
    m_jsRoot = NULL;
}

JsonMap::~JsonMap()
{

}

int JsonMap::set_json(cJSON * in_json) {
     m_jsRoot = in_json;
     if (m_jsRoot) {
        return 0;
    }
    return -1;
}

int JsonMap::set_json(const char * in_buff) {
    m_jsRoot = cJSON_Parse( in_buff );
    if (m_jsRoot) {
        return 0;
    }
    return -1;
}

int JsonMap::ReadString(const char * in_name, char * out_data, int in_size)
{
    if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
        int json_len = 0;
	    if (json_data->type==cJSON_Number){
            char temp[32] = {0};
            sprintf( temp, "%d", json_data->valueint );
            json_len = strlen( temp );
            if ( json_len > in_size ) {
                return 0;
            }
            memcpy( out_data, temp, json_len + 1);
            return json_len;
	    }
	    else if (json_data->type==cJSON_String){
            json_len = strlen( json_data->valuestring );
            if ( json_len > in_size ) {
                return 0;
            }
            memcpy( out_data, json_data->valuestring, json_len + 1);
            return json_len;
	    }
	}
    return 0;
}

int JsonMap::ReadDouble(const char * in_name, double & out_data)
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
	    if (json_data->type==cJSON_Number){
            out_data = json_data->valuedouble;
            return sizeof(double);
	    }
	    if (json_data->type==cJSON_String){
            out_data = strtod( json_data->valuestring, NULL );
            return sizeof(double);
	    }
	}
    return 0;
}

int JsonMap::ReadInteger(const char * in_name, int & out_data)
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
	    if (json_data->type==cJSON_Number){
            out_data = json_data->valueint;
            return sizeof(double);
	    }
	    if (json_data->type==cJSON_String){
            out_data = atol( json_data->valuestring );
            return sizeof(double);
	    }
	}
    return 0;
}

int JsonMap::ReadLongLong(const char * in_name, INT64 & out_data)
{
  if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
	    if (json_data->type==cJSON_Number){
            out_data = json_data->valueint;
            return sizeof(double);
	    }
	    if (json_data->type==cJSON_String){
            out_data = atol(json_data->valuestring );
            return sizeof(double);
	    }
	}
    return 0;
}


int JsonMap::ReadJson(const char * in_name, JsonMap & out_data)
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
        out_data.set_json(json_data);
        return 0;
	}
    return -1;
}


int JsonMap::ReadArrayJson( int in_index, JsonMap & out_data )
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetArrayItem( m_jsRoot, in_index );
	if ( json_data ) {
        out_data.set_json(json_data);
        return 0;
	}
    return -1;
}

int JsonMap::GetArraySize()
{
    if ( m_jsRoot != NULL ) {
		return cJSON_GetArraySize(m_jsRoot);
	}
    return 0;
}

/*
 *   JsonBuffer
 */

JsonBuffer::JsonBuffer(const char * in_buff)
{
    m_jsRoot = cJSON_Parse( in_buff );
    m_yJson = 0;
}

JsonBuffer::JsonBuffer(const char * in_buff, int in_size)
{
    m_jsRoot = cJSON_Parse( in_buff );
    m_yJson = 0;
}

JsonBuffer::JsonBuffer(JsonBuffer & in_buff)
{
    m_yJson = in_buff.m_yJson;
}

JsonBuffer::~JsonBuffer()
{
    if ( m_jsRoot ) {
		cJSON_Delete( m_jsRoot );
	}
}

void JsonBuffer::set_json( const char * in_buff ) {
    m_jsRoot = cJSON_Parse( in_buff );
}

void JsonBuffer::set_json( cJSON * in_json ) {
    m_jsRoot = in_json;
}


void JsonBuffer::clear_buff() {
    m_yJson = NULL;
}

void JsonBuffer::empty() {
    m_yJson = NULL;
}

int JsonBuffer::length() {
    return m_yJson.length();
}

void JsonBuffer::package_one()
{
    Yond_string pk = "{";
    pk += m_yJson;
    pk += "}";
    m_yJson = pk;
}

void JsonBuffer::package_array()
{
    Yond_string pk = "[";
    pk += m_yJson;
    pk += "]";
    m_yJson = pk;
}

const char * JsonBuffer::get_json() {
    return m_yJson.c_str();
}

int JsonBuffer::ReadString(const char * in_name, char * out_data, int in_size)
{
    if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
        int json_len = 0;
	    if (json_data->type==cJSON_Number){
            char temp[32] = {0};
            sprintf( temp, "%s", json_data->valueint );
            json_len = strlen( temp );
            if ( json_len > in_size ) {
                return 0;
            }
            memcpy( out_data, temp, json_len + 1);
            return json_len;
	    }
	    if (json_data->type==cJSON_String){
            json_len = strlen( json_data->valuestring );
            if ( json_len > in_size ) {
                return 0;
            }
            memcpy( out_data, json_data->valuestring, json_len + 1);
            return json_len;
	    }
	}
    return 0;
}

int JsonBuffer::ReadDouble(const char * in_name, double & out_data)
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
	    if (json_data->type==cJSON_Number){
            out_data = json_data->valuedouble;
            return sizeof(double);
	    }
	    if (json_data->type==cJSON_String){
            out_data = strtod( json_data->valuestring, NULL );
            return sizeof(double);
	    }
	}
    return 0;
}

int JsonBuffer::ReadInteger(const char * in_name, int & out_data)
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
	    if (json_data->type==cJSON_Number){
            out_data = json_data->valueint;
            return sizeof(double);
	    }
	    if (json_data->type==cJSON_String){
            out_data = atol( json_data->valuestring );
            return sizeof(double);
	    }
	}
    return 0;
}

int JsonBuffer::ReadLongLong(const char * in_name, INT64 & out_data)
{
  if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
	    if (json_data->type==cJSON_Number){
            out_data = json_data->valueint;
            return sizeof(double);
	    }
	    if (json_data->type==cJSON_String){
            out_data = atol(json_data->valuestring );
            return sizeof(double);
	    }
	}
    return 0;
}

int JsonBuffer::ReadJson(const char * in_name, JsonBuffer & out_data)
{
   if ( m_jsRoot == NULL ) {
		return -1;
	}

	// json_data id
	cJSON * json_data = cJSON_GetObjectItem( m_jsRoot, in_name );
	if ( json_data ) {
        out_data.set_json(json_data);
        return 0;
	}
    return -1;
}

int JsonBuffer::WriteString(const char * in_name, char * out_data)
{
    if ( m_yJson.length() != 0 ) {
        m_yJson += ",";
    }

    char temp[80] = {0};
    sprintf( temp,"\"%s\":\"%s\"" ,in_name, out_data);
    m_yJson += temp;
    return m_yJson.length();
}

int JsonBuffer::WriteDouble(const char * in_name, double out_data)
{
    if ( m_yJson.length() != 0 ) {
        m_yJson += ",";
    }

    char temp[80] = {0};
    sprintf( temp,"\"%s\":\"%f\"" ,in_name, out_data);
    m_yJson += temp;
    return m_yJson.length();
}

int JsonBuffer::WriteInteger(const char * in_name, int out_data)
{
    if ( m_yJson.length() != 0 ) {
        m_yJson += ",";
    }

    char temp[80] = {0};
    sprintf( temp,"\"%s\":\"%d\"" ,in_name, out_data);
    m_yJson += temp;
    return m_yJson.length();
}

int JsonBuffer::WriteLongLong(const char * in_name, INT64 out_data)
{
    if ( m_yJson.length() != 0 ) {
        m_yJson += ",";
    }

    char temp[80] = {0};
    sprintf( temp,"\"%s\":\"%llu\"" ,in_name, out_data );
    m_yJson += temp;
    return m_yJson.length();
}

int JsonBuffer::WriteJson( JsonBuffer & out_data )
{
    if ( m_yJson.length() != 0 ) {
        m_yJson += ",";
    }

    m_yJson += out_data.get_json();
    return m_yJson.length();
}

int JsonBuffer::WriteJson(const char * in_name, JsonBuffer & out_data)
{
    if ( m_yJson.length() != 0 ) {
        m_yJson += ",";
    }

    char temp[80] = {0};
    sprintf( temp,"\"%s\":" ,in_name);
    m_yJson += temp;
    m_yJson += out_data.get_json();
    return m_yJson.length();
}


/*
    { "msghead":{ "userkey":"5220", "userkey":"5220"} }
*/
JsonHeader::JsonHeader()
{

}

JsonHeader::~JsonHeader()
{

}

int JsonHeader::Parser( const char * json_buff, int json_size   )
{
    JsonBuffer buff( json_buff, json_size );

    JsonBuffer msgbuff;
    if ( buff.ReadJson( "msghead", msgbuff) != 0 ) {
        return -1;
    }

    msgbuff.ReadInteger("userid",  m_iUserid);
    msgbuff.ReadInteger("userkey", m_iUserkey);
}

WORD JsonHeader::GetUserkey() {
    return m_iUserkey;
}

WORD JsonHeader::GetUserid() {
    return m_iUserid;
}
