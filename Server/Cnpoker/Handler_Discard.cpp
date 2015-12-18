#include "Handler_Module.h"
#include "CnpokerServer.h"

/* 分析牌，输入Poker值，得到 byThanType、byThanValue、byThanCount; */
int Battle_Poker_Parser ( BYTE * byPK, BYTE bySize, BYTE & byThanValue, BYTE & byThanCount);

/* 比较牌大小 */
int Battle_Poker_Compare ( GameBattle * pBattle, BYTE * byPK, BYTE bySize, int & byPokerType );

/* 转化成ByteArray */
int Battle_Poker_Get_Byte_Array ( char *_data, BYTE * buff, int uiSize, char div);

/* 转化成版型面值转化 */
int Battle_Poker_Transform ( BYTE * byIndex, BYTE byIndexSize, BYTE * byPK, BYTE byPKSize );


/* {protocol : 1, userkey : 2, status : 3} */
int Processed_Discards ( ServerSession * pServerSession, const char * pInput )
{
    JsonMap js_map;
    if ( js_map.set_json( (char *) pInput ) == -1 ) {
        return FALSE;
    }

    int _errmsg = 0;
    int _iBattleid, _iSeatid, _iStatus, _iCount;
    char _szPoker[256] = {0};

    js_map.ReadInteger("battleid", _iBattleid );
    js_map.ReadInteger("seatid",   _iSeatid  );
    js_map.ReadInteger("count" ,   _iCount );
    js_map.ReadString ("poker" ,   _szPoker,  sizeof(_szPoker) );

    if ( !(strlen( _szPoker ) > 0) ) {
        return FALSE;
    }

    // @{{{ 检查参数
    if ( (_iSeatid<0) || (_iSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _iBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    // 判断是否正在玩牌
    if ( pBattle->getBattleStatus() != eGB_PLAYING ) {
        return FALSE;
    }

    // 判断出牌坐位
    if ( pBattle->getPlaying() != _iSeatid ) {
        return FALSE;
    }

    int _byPokerType = 0;
    if ( _iCount>0 ) {

        /* step1. 打牌作判断 */
        BYTE byPokers[MAX_POKER+1] = {0};
        int nSize = Battle_Poker_Get_Byte_Array( _szPoker, byPokers, sizeof(byPokers), ' ');

        /* step2. 第一个打牌不作判断 */
        if ( pBattle->getDiscardTimes()>0 )     // 第一个出牌不做比较
        {
            /* step3. 判断是否属于你拥有的牌 */
            if ( !pBattle->hadDiscards( _iSeatid, byPokers, nSize )  ) {
                return FALSE;
            }

            /* step4. 判断比较是否大于前面的玩家 */
            if ( Battle_Poker_Compare( pBattle, byPokers, nSize, _byPokerType) == FALSE ) {
                return FALSE;
            }
        }

        /* step5. 设置参数 */
        pBattle->getDiscardTimes()++;
        pBattle->SetDiscards( _iSeatid, byPokers, nSize );
        pBattle->SetDiscardString( _iSeatid, _szPoker );
        pBattle->SetLastSeat( _iSeatid );  // 出牌的坐位;
    }
    else {
         /* step6. 如果没出牌，判断是否第一个，第一个不能不出牌的，亲! */
        if ( pBattle->getDiscardTimes()==0 ) {
            _errmsg = -1;
        }
    }

    /* step6. 打印参数到所有客户端 */
    {
        // @{{{ 组合所有的牌，发送给所有玩家;
        char szMsg[1024] = {0};
        char format[256] = 	"{ \"protocol\":\"%d\","
                                "%s,"
                                "\"data\":[{"
                                "\"errmsg\":\"%d\","
                                "\"battleid\":\"%d\","
                                "\"seatid\":\"%d\","
                                "\"pktype\":\"%d\","
                                "\"count\":\"%d\","
                                "\"poker\":\"%s\"}] }";
        char szPlayerkey[256] = {0};
        pBattle->GetAllPlayerKey( szPlayerkey, sizeof(szPlayerkey) );

        sprintf( szMsg, format, MAKEDWORD(Games_Protocol, Discards_BRD ),
                szPlayerkey,
                _errmsg,
                pBattle->getIndex(),
                _iSeatid,
                _byPokerType,
                _iCount,
                _szPoker);
        // }}}@ 组合所有的牌

        // @{{{ 发送到其他玩家；
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );

        if ( pBattle->canEnd()==FALSE ) {
            BYTE _byNextSeatid;
            _byNextSeatid = pBattle->nextSeat( _iSeatid );  // 授权-并判断
            pBattle->SetPlaying( _byNextSeatid );
        }
    }
}

void MSG_Handler_Discards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {

    // 解析牌后返送
    Processed_Discards( pServerSession, (char*) pMsg );
}


/**********  比较 **********/
int Battle_Poker_Compare( GameBattle * pBattle, BYTE * pbyIndex, BYTE byIndexSize, int & byPokerType )
{
    BYTE byThanType, byThanValue, byThanCount;
    BYTE _byType, _byValue, _byCount, _bySize;

    // 先转化为一个数字
    BYTE pbyPK[MAX_PK_TYPE] = {0};
    BYTE byPKSize = MAX_PK_TYPE;
    Battle_Poker_Transform( pbyIndex, byIndexSize, pbyPK, byPKSize );
    byThanType  = Battle_Poker_Parser(pbyPK, byIndexSize, byThanValue, byThanCount);
    byPokerType = byThanType; // 把类型传出去;

    // step1: 出的牌有问题，直接返回错误;
    if ( byThanType==PH_0 ) {
        goto Compare_Wrong;
    }

    _byType  = pBattle->getLastType();
    _byValue = pBattle->getLastValue();
    _byCount = pBattle->getLastCount();
    _bySize  = pBattle->getLastSize();

    if ( pBattle->getDiscardTimes()==0 ) {
        goto Compare_Correct;   // 第一个出牌直接走起
    }

    // 对两首牌的类型进行比较;
    if (_byType == byThanType) {
        if ( _byCount == byThanCount ) {
            if ( _byValue < byThanValue ) {
                goto Compare_Correct;
            }
        }
    }
    else if (_byType != byThanType) {
        if (byThanType == PH_MAX) {
            goto Compare_Correct;
        }
        else {
            if (byThanType == PH_4) {
                goto Compare_Correct;
            }
        }
    }
    goto Compare_Wrong;

Compare_Correct:
    pBattle->SetLastType ( byThanType  );
    pBattle->SetLastValue( byThanValue );
    pBattle->SetLastCount( byThanCount );
    pBattle->SetLastSize ( byIndexSize );
    return byThanType;

Compare_Wrong:
    return 0;
}


/**********  begin of 出牌的估值算法 **********/
int Battle_Poker_Parser( BYTE * byPK, BYTE bySize, BYTE & byThanValue, BYTE & byThanCount)
{
	if ( bySize <= 0 || byPK == NULL ) {
		return (PH_0);
	}

	byThanCount = 0;
	byThanValue = byPK[0]; // 取第一个牌为比较值

	// (高击中 + 低复杂度) 判断区
	if ( bySize == 1 ) {
        byThanCount = 1;
		return (PH_1); // 单牌
	}
	else if ( bySize == 2)
    {
        byThanCount = 2;
		if ( byPK[0] == byPK[1] ) {
			return (PH_2); // 对子
		}
		if ( (byPK[0] == 14) && (byPK[1] == 15) ) {
			return (PH_MAX); // 王炸
		}
		return -1;
	}

	short _Max_Value = 0; 	// PK最大面值
	short _Min_Value = 15; 	// PK最小面值
	short _Max_Count = 0; 	// PK计量最多
	short _Min_Count = 0; 	// PK计量最少
	short _Min_Index = 0; 	// PK计量需要一个下标位来判断

	const short CS_SLOT = 15; // PK的下标最大常量数
	short _PK_Slot[CS_SLOT] = {0}; // PK的SLOT容器

	short _idx = 0;
	short _val = 0;
	for ( short i=0; i<(short)bySize; ++i)
	{
		_idx = byPK[i];
		if ( _idx >= 15 ) {
			return (PH_0);
		}

		_PK_Slot[_idx]++;
		_val = _PK_Slot[_idx];

		_Max_Value = (_idx >  _Max_Value) ? _idx : _Max_Value; 	// 高位置获取
		_Min_Value = (_idx <= _Min_Value) ? _idx : _Min_Value; 	// 低位置获取
		_Max_Count = (_val >  _Max_Count) ? _val : _Max_Count; 	// 高计量获取
	}

	if ( (_Max_Value == _Min_Value) ) { // PK 大小面值都相等时
		if ( bySize == 3 ) {
            byThanCount = 3;
			return (PH_3); // 3个
		}
		if ( bySize == 4 ) {
		    byThanCount = 4;
			return (PH_4); // 炸弹
		}
		return (PH_0);
	}

	if ( _Max_Count > 4) {
		return (PH_0);
	}

	// 得出最小面值的牌
	for (short i=0; i<(short)bySize; ++i)
	{
		_idx = byPK[i];
		_val = _PK_Slot[_idx];
		_Min_Count = (_val <= _Min_Count) ? _val : _Min_Count; // 低计量获取
	}

	byThanValue = _Max_Value; // 到这里取最大值为比较值
	if ( _Max_Count == _Min_Count ) { // PK 数量都相等时
		byThanCount = bySize / _Max_Count;
		short _Div_0 = bySize / (_Max_Value - _Min_Value + 1);
		short _Div_1 = bySize % (_Max_Value - _Min_Value + 1);
		short _Div_2 = bySize / _Max_Count;
		short _Div_3 = bySize % _Max_Count;
		if ( (_Max_Count == _Div_0) && (_Div_1 == 0) ) {
			if ( _Max_Count == 1 ) {
				if ( (_Div_2 >= 5) && (_Div_3 == 0) )
					return (PH_4); // 顺子
				else
					return (PH_0); // 顺不起来
			}
			if ( _Max_Count == 2 ) {
				if ( (_Div_2 >= 3) && (_Div_3 == 0) )
					return 0; // 连对
				else
					return (PH_0); // 连不起来
			}
			if ( _Max_Count == 3 ) {
				if ( (_Div_2 >= 2) && (_Div_3 == 0) )
					return 0; // 飞机
				else
					return (PH_0); // 飞不起来
			}
		}
		return (PH_0);
	}

	_idx = 0;
	for (short i=0; i<(short)bySize; ++i)
	{
		_idx = byPK[i];
		if ( _PK_Slot[_idx] == _Max_Count ) {
			break;
		}
	}

	byThanValue = _idx; // 到这里取最多牌面值为比较值:
	if ( _Max_Count == 3 ) {
		byThanCount = 3;
		if ( (_Min_Count == 1) && (bySize == 4) ) {
			return (PH_31); // 三带一
		}
		if ( (_Min_Count == 2) && (bySize == 5) ) {
			return (PH_32); // 三带二
		}

		// 是否三个连起来了
		short _Three_Max_Value 	= 0; 	// 3同位最大面值
		short _Three_Min_Value 	= 15; 	// 3同位最小面值
		short _Three_Count 		= 0; 	// 3同位个数
		short _Two_Count 		= 0; 	// 2同位个数
		short _One_Count 		= 0; 	// 1同位个数
		for (short i=0; i<(short)CS_SLOT; ++i)
		{
			_idx = _PK_Slot[i];
			if ( _idx == 3 ) {
				_Three_Max_Value = (i >  _Three_Max_Value) ? i : _Three_Max_Value; // 高位置获取
				_Three_Min_Value = (i <= _Three_Min_Value) ? i : _Three_Min_Value; // 低位置获取
				_Three_Count++;
			}
			else if ( _idx == 2 ) {
				_Two_Count++;
			}
			else if ( _idx == 1 ) {
				_One_Count++;
			}
		}

		short _Div_0 = (_Three_Max_Value - _Three_Min_Value + 1) / _Three_Count;
		short _Div_1 = (_Three_Max_Value - _Three_Min_Value + 1) % _Three_Count;
		if ( (_Div_0 == 1) && (_Div_1 == 0) ) {
			byThanCount = _Three_Count;
			if ( _Three_Count == (_Two_Count * 2 + _One_Count) ) {
				return (PH_31); // 三带一
			}
			return (PH_0);
		}
		return (PH_0);
	}

	if ( _Max_Count == 4 ) {
        byThanCount = 4;
		if ( _Min_Count == 1 ) {
			if ( bySize == 5 )
				return (PH_41); // 四带一
			if ( bySize == 6 )
				return (PH_411); // 四带二
			return (PH_0);
		}
		if ( _Min_Count == 2 ) {
			if ( bySize == 6 )
				return (PH_42); // 四带一对
			if ( bySize == 8 )
				return (PH_422); // 四带两对
			return (PH_0);
		}
		return (PH_0);
	}

	return (PH_0);
}
/**********  end 出牌算法 **********/


/********** 字符串转数字 **********/
#define MAX_BUFF 32
int Battle_Poker_Get_Byte_Array( char *_data, BYTE * buff, int uiSize, char div)
{
	char szBuff[MAX_BUFF] = {0};
	char *obj = szBuff;
    char *src = _data;
    BYTE *bys = buff;
    int   sub  = 0;

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
/********** end 字符串转数字 **********/


/********** 下标数字转牌数字 **********/
int Battle_Poker_Transform ( BYTE * pbyIndex, BYTE byIndexSize, BYTE * pbyPK, BYTE byPKSize )
{
    if ( byIndexSize > byPKSize ) {
        return -1;
    }

    BYTE pv = 0;
    for ( int i=0; i<byIndexSize; i++) {
        pv = pbyIndex[i];
        if ( pv < MAX_POKER ) {
            pbyPK[i] = pv % 13;
			if ( pv==52) {
				pbyPK[i] = 13;
			}
			else if ( pv==53) {
				pbyPK[i] = 14;
			}
		}
    }
    return 0;
}
/********** end 下标数字转牌数字 **********/

