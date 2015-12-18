#include "Handler_Module.h"
#include "GameUser.h"

int acl_create_showcards_1( char * pInput, WORD wSize )
{
	int _battleid = g_GameUser.m_nBattleid;
    int _seatid   = g_GameUser.m_nSeatid;
	int _userkey  = g_GameUser.m_nSeatKey;
	
	char buff[1024]  = {0};
	char format[256] = 	"{ \"Protocol\":\"%d\", "
						"  \"battleid\":\"%d\", "
                        "  \"seatid\":\"%d\"} ";
						
	sprintf( buff, format, MAKEDWORD( Games_Protocol,  ShowCards_REQ ) ,
				_battleid, _seatid);

	int len = strlen(buff);
	if ( len < wSize ) {
		memcpy(pInput, buff, len);
	}
	
	// printf("%s \n", buff);
}

/********** 字符串转数字 **********/
#define MAX_BUFF 32
int Battle_Poker_Get_Byte_Array( char *_data, BYTE * buff, int uiSize, char div)
{
	char szBuff[MAX_BUFF] = {0};
	char *obj = szBuff;
    char *src = _data;
    BYTE *bys = buff;
    int  sub  = 0;

    while ( *src != '\0' )
    {
        if ( *src == div )
        {
            if ( sub < uiSize )
            {
                if ( (obj - szBuff) < MAX_BUFF && (obj - szBuff) != 0 )
                {
                    *bys = (BYTE) atoi(szBuff);
                    ++bys;
                    ++sub;
                }
            }

            memset( szBuff, 0x0, sizeof(szBuff) );
            obj = szBuff;
        }
        else if ( (obj - szBuff) < MAX_BUFF )
        {
            *obj = *src;
            ++obj;
        }
        ++src;
    }
    if ( sub < uiSize )
    {
        if ( (obj - szBuff) < MAX_BUFF && (obj - szBuff) != 0)
        {
            *bys = (BYTE) atoi(szBuff);
            ++sub;
        }
    }
    return sub;
}

static char array_color[4]  = { '+', '-', '*', '/' };
static BYTE array_index[13] = { '3','4','5','6','7','8','9','S','J','Q','K','A','2' };

/********** 下标数字转牌数字 **********/
int Battle_Poker_Transform ( BYTE * pbyIndex, BYTE byIndexSize )
{
	BYTE col = 0, val = 0;
	BYTE pv = 0;
		
	printf("\n");
	printf( "[Client] MSG Begin >> (Initcards_Poker) \n" );
	printf( "[Client] { ");
	
    for ( int i=0; i<byIndexSize; i++) {
        pv = pbyIndex[i];
        if ( pv < MAX_POKER ) {
			col = pv / 13;
            val = pv % 13;
			if ( pv==52) {
				printf("%c", '@' );	
			}
			else if ( pv==52) {
				printf("%c", '$' );		
			}
			else {		
				if ( col>=0  && col<4 ) {
					printf("%c", array_color[col] );
				}
				if ( val>=0  && val<13 ) {
					printf("%c", array_index[val]);
				}
			}
			printf("  ");
		}
    }
	printf( " }; \n");
	printf( "[Client] MSG End \n");
    return 0;
}
/********** end 下标数字转牌数字 **********/

void getAllPoker( char * pInput, WORD wSize )
{	
    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return;
    }
	
	JsonMap js_data;
	js_map.ReadJson( "data", js_data );

	char _szPoker[256] = {0};
	int iSize = js_data.GetArraySize();
	
	if ( iSize > 0 )
	{
		int _battleid = 0;
		JsonMap js_array;
		js_data.ReadArrayJson( 0, js_array );
		js_array.ReadString ("poker" ,   _szPoker,  sizeof(_szPoker) );
		
		char szMsg[1024] = {0};
		BYTE * bMove = g_GameUser.m_byPokers;
		BYTE   bSize = sizeof( g_GameUser.m_byPokers );
		
		int nSize = Battle_Poker_Get_Byte_Array( _szPoker, bMove, bSize, ',' );
		
		Battle_Poker_Transform( bMove, nSize );
	}
	
}

void MSG_Handler_InitCards_BRD( int cnSock, char * pInput, WORD wSize )
{
	getAllPoker( pInput, wSize );
	
	printf("\n");
	printf( "[Client] MSG Begin >> (MSG_Handler_InitCards_BRD) \n" );
	printf( "[Client] %s \n",  (char*)pInput );
	printf( "[Client] MSG End \n");
	
	char szBuff[1024] = {0};
	int len = acl_create_showcards_1(szBuff, 1024);
	send(cnSock, szBuff, len, 0);
	
	return;
}