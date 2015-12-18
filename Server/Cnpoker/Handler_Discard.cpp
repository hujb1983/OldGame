#include "Handler_Module.h"
#include "CnpokerServer.h"

/* �����ƣ�����Pokerֵ���õ� byThanType��byThanValue��byThanCount; */
int Battle_Poker_Parser ( BYTE * byPK, BYTE bySize, BYTE & byThanValue, BYTE & byThanCount);

/* �Ƚ��ƴ�С */
int Battle_Poker_Compare ( GameBattle * pBattle, BYTE * byPK, BYTE bySize, int & byPokerType );

/* ת����ByteArray */
int Battle_Poker_Get_Byte_Array ( char *_data, BYTE * buff, int uiSize, char div);

/* ת���ɰ�����ֵת�� */
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

    // @{{{ ������
    if ( (_iSeatid<0) || (_iSeatid>=3) ) {
        return FALSE;
    }

    GameBattle *pBattle = g_GameMgr.GetBattle( _iBattleid );
    if ( pBattle==NULL ) {
        return FALSE;
    }

    // �ж��Ƿ���������
    if ( pBattle->getBattleStatus() != eGB_PLAYING ) {
        return FALSE;
    }

    // �жϳ�����λ
    if ( pBattle->getPlaying() != _iSeatid ) {
        return FALSE;
    }

    int _byPokerType = 0;
    if ( _iCount>0 ) {

        /* step1. �������ж� */
        BYTE byPokers[MAX_POKER+1] = {0};
        int nSize = Battle_Poker_Get_Byte_Array( _szPoker, byPokers, sizeof(byPokers), ' ');

        /* step2. ��һ�����Ʋ����ж� */
        if ( pBattle->getDiscardTimes()>0 )     // ��һ�����Ʋ����Ƚ�
        {
            /* step3. �ж��Ƿ�������ӵ�е��� */
            if ( !pBattle->hadDiscards( _iSeatid, byPokers, nSize )  ) {
                return FALSE;
            }

            /* step4. �жϱȽ��Ƿ����ǰ������ */
            if ( Battle_Poker_Compare( pBattle, byPokers, nSize, _byPokerType) == FALSE ) {
                return FALSE;
            }
        }

        /* step5. ���ò��� */
        pBattle->getDiscardTimes()++;
        pBattle->SetDiscards( _iSeatid, byPokers, nSize );
        pBattle->SetDiscardString( _iSeatid, _szPoker );
        pBattle->SetLastSeat( _iSeatid );  // ���Ƶ���λ;
    }
    else {
         /* step6. ���û���ƣ��ж��Ƿ��һ������һ�����ܲ����Ƶģ���! */
        if ( pBattle->getDiscardTimes()==0 ) {
            _errmsg = -1;
        }
    }

    /* step6. ��ӡ���������пͻ��� */
    {
        // @{{{ ������е��ƣ����͸��������;
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
        // }}}@ ������е���

        // @{{{ ���͵�������ң�
        WORD nLen = strlen( szMsg );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)szMsg, nLen );

        if ( pBattle->canEnd()==FALSE ) {
            BYTE _byNextSeatid;
            _byNextSeatid = pBattle->nextSeat( _iSeatid );  // ��Ȩ-���ж�
            pBattle->SetPlaying( _byNextSeatid );
        }
    }
}

void MSG_Handler_Discards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize ) {

    // �����ƺ���
    Processed_Discards( pServerSession, (char*) pMsg );
}


/**********  �Ƚ� **********/
int Battle_Poker_Compare( GameBattle * pBattle, BYTE * pbyIndex, BYTE byIndexSize, int & byPokerType )
{
    BYTE byThanType, byThanValue, byThanCount;
    BYTE _byType, _byValue, _byCount, _bySize;

    // ��ת��Ϊһ������
    BYTE pbyPK[MAX_PK_TYPE] = {0};
    BYTE byPKSize = MAX_PK_TYPE;
    Battle_Poker_Transform( pbyIndex, byIndexSize, pbyPK, byPKSize );
    byThanType  = Battle_Poker_Parser(pbyPK, byIndexSize, byThanValue, byThanCount);
    byPokerType = byThanType; // �����ʹ���ȥ;

    // step1: �����������⣬ֱ�ӷ��ش���;
    if ( byThanType==PH_0 ) {
        goto Compare_Wrong;
    }

    _byType  = pBattle->getLastType();
    _byValue = pBattle->getLastValue();
    _byCount = pBattle->getLastCount();
    _bySize  = pBattle->getLastSize();

    if ( pBattle->getDiscardTimes()==0 ) {
        goto Compare_Correct;   // ��һ������ֱ������
    }

    // �������Ƶ����ͽ��бȽ�;
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


/**********  begin of ���ƵĹ�ֵ�㷨 **********/
int Battle_Poker_Parser( BYTE * byPK, BYTE bySize, BYTE & byThanValue, BYTE & byThanCount)
{
	if ( bySize <= 0 || byPK == NULL ) {
		return (PH_0);
	}

	byThanCount = 0;
	byThanValue = byPK[0]; // ȡ��һ����Ϊ�Ƚ�ֵ

	// (�߻��� + �͸��Ӷ�) �ж���
	if ( bySize == 1 ) {
        byThanCount = 1;
		return (PH_1); // ����
	}
	else if ( bySize == 2)
    {
        byThanCount = 2;
		if ( byPK[0] == byPK[1] ) {
			return (PH_2); // ����
		}
		if ( (byPK[0] == 14) && (byPK[1] == 15) ) {
			return (PH_MAX); // ��ը
		}
		return -1;
	}

	short _Max_Value = 0; 	// PK�����ֵ
	short _Min_Value = 15; 	// PK��С��ֵ
	short _Max_Count = 0; 	// PK�������
	short _Min_Count = 0; 	// PK��������
	short _Min_Index = 0; 	// PK������Ҫһ���±�λ���ж�

	const short CS_SLOT = 15; // PK���±��������
	short _PK_Slot[CS_SLOT] = {0}; // PK��SLOT����

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

		_Max_Value = (_idx >  _Max_Value) ? _idx : _Max_Value; 	// ��λ�û�ȡ
		_Min_Value = (_idx <= _Min_Value) ? _idx : _Min_Value; 	// ��λ�û�ȡ
		_Max_Count = (_val >  _Max_Count) ? _val : _Max_Count; 	// �߼�����ȡ
	}

	if ( (_Max_Value == _Min_Value) ) { // PK ��С��ֵ�����ʱ
		if ( bySize == 3 ) {
            byThanCount = 3;
			return (PH_3); // 3��
		}
		if ( bySize == 4 ) {
		    byThanCount = 4;
			return (PH_4); // ը��
		}
		return (PH_0);
	}

	if ( _Max_Count > 4) {
		return (PH_0);
	}

	// �ó���С��ֵ����
	for (short i=0; i<(short)bySize; ++i)
	{
		_idx = byPK[i];
		_val = _PK_Slot[_idx];
		_Min_Count = (_val <= _Min_Count) ? _val : _Min_Count; // �ͼ�����ȡ
	}

	byThanValue = _Max_Value; // ������ȡ���ֵΪ�Ƚ�ֵ
	if ( _Max_Count == _Min_Count ) { // PK ���������ʱ
		byThanCount = bySize / _Max_Count;
		short _Div_0 = bySize / (_Max_Value - _Min_Value + 1);
		short _Div_1 = bySize % (_Max_Value - _Min_Value + 1);
		short _Div_2 = bySize / _Max_Count;
		short _Div_3 = bySize % _Max_Count;
		if ( (_Max_Count == _Div_0) && (_Div_1 == 0) ) {
			if ( _Max_Count == 1 ) {
				if ( (_Div_2 >= 5) && (_Div_3 == 0) )
					return (PH_4); // ˳��
				else
					return (PH_0); // ˳������
			}
			if ( _Max_Count == 2 ) {
				if ( (_Div_2 >= 3) && (_Div_3 == 0) )
					return 0; // ����
				else
					return (PH_0); // ��������
			}
			if ( _Max_Count == 3 ) {
				if ( (_Div_2 >= 2) && (_Div_3 == 0) )
					return 0; // �ɻ�
				else
					return (PH_0); // �ɲ�����
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

	byThanValue = _idx; // ������ȡ�������ֵΪ�Ƚ�ֵ:
	if ( _Max_Count == 3 ) {
		byThanCount = 3;
		if ( (_Min_Count == 1) && (bySize == 4) ) {
			return (PH_31); // ����һ
		}
		if ( (_Min_Count == 2) && (bySize == 5) ) {
			return (PH_32); // ������
		}

		// �Ƿ�������������
		short _Three_Max_Value 	= 0; 	// 3ͬλ�����ֵ
		short _Three_Min_Value 	= 15; 	// 3ͬλ��С��ֵ
		short _Three_Count 		= 0; 	// 3ͬλ����
		short _Two_Count 		= 0; 	// 2ͬλ����
		short _One_Count 		= 0; 	// 1ͬλ����
		for (short i=0; i<(short)CS_SLOT; ++i)
		{
			_idx = _PK_Slot[i];
			if ( _idx == 3 ) {
				_Three_Max_Value = (i >  _Three_Max_Value) ? i : _Three_Max_Value; // ��λ�û�ȡ
				_Three_Min_Value = (i <= _Three_Min_Value) ? i : _Three_Min_Value; // ��λ�û�ȡ
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
				return (PH_31); // ����һ
			}
			return (PH_0);
		}
		return (PH_0);
	}

	if ( _Max_Count == 4 ) {
        byThanCount = 4;
		if ( _Min_Count == 1 ) {
			if ( bySize == 5 )
				return (PH_41); // �Ĵ�һ
			if ( bySize == 6 )
				return (PH_411); // �Ĵ���
			return (PH_0);
		}
		if ( _Min_Count == 2 ) {
			if ( bySize == 6 )
				return (PH_42); // �Ĵ�һ��
			if ( bySize == 8 )
				return (PH_422); // �Ĵ�����
			return (PH_0);
		}
		return (PH_0);
	}

	return (PH_0);
}
/**********  end �����㷨 **********/


/********** �ַ���ת���� **********/
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
/********** end �ַ���ת���� **********/


/********** �±�����ת������ **********/
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
/********** end �±�����ת������ **********/

