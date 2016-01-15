#include "Handler_Module.h"
#include "CnpokerServer.h"

/*****************************************************
    Battle_Poker_Get_Byte_Array : ת��Ϊ���±�
*****************************************************/
int Battle_Poker_Get_Byte_Array ( char *_data, BYTE * buff, int uiSize, char div);

/*****************************************************
    Battle_Poker_Parser : �ж��±��Ƿ����
*****************************************************/
int Battle_Poker_Has_Discards ( TablePacket & pack, BYTE * byIndex, BYTE bySize, BYTE byUserSign );

/*****************************************************
    Battle_Poker_Transform : �±�ת����ֵ
*****************************************************/
int Battle_Poker_Transform ( BYTE * byIndex, BYTE byIndexSize, BYTE * byPK, BYTE byPKSize );

/*****************************************************
    Battle_Poker_Parser : ֵ�Ʒ���
*****************************************************/
int Battle_Poker_Parser ( BYTE * byPK, BYTE bySize, BYTE & byThanValue, BYTE & byThanCount);

/*****************************************************
    Battle_Poker_Compare : �ƱȽ�
*****************************************************/
int Battle_Poker_Compare ( TablePacket & pack, BYTE * byPK, BYTE bySize );

/*****************************************************
    Battle_Hava_Continue : �Ƿ���Ҫ��������
*****************************************************/
int Battle_Hava_Continue ( TablePacket & pack );

/*****************************************************
    Battle_Get_Usercards
*****************************************************/
BYTE Battle_Get_Usercards( TablePacket & pack, BYTE byVal, BYTE seatId );


/*****************************************************
    MSG_Handler_Discards_REQ
*****************************************************/
void MSG_Handler_Discards_REQ ( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
{
    if ( wSize < sizeof(UserPacket) ) {
        return ;
    }

    BYTE _called(0);
    UserPacket user;
    user.SetPacket( (BYTE*)pMsg, wSize );
    int    _iCount = user.GetShowedSize();
    char * _szPokers = user.GetShowedPokers();

    GameTable * table = g_GameMgr.GetTable( user.GetTableId() );
    if (!table) {
        return;
    }

    TablePacket & pack = table->GetTablePacket();

    BYTE _seatid = user.GetSeatId();
    if (_seatid>2) {
        return;
    }

    if ( pack.GetPlaySeatId()!=_seatid ) {
        return;
    }

    if ( _iCount>0 )
    {
        BYTE byPokers[MAX_POKER+1] = {0};
        BYTE bySize = Battle_Poker_Get_Byte_Array( _szPokers, byPokers, sizeof(byPokers), ',');

        if (_iCount==bySize) {

            /* step3. �ж��Ƿ�������ӵ�е��� */
            BYTE byUserSign = PK_USER_0 + _seatid;
            if ( !Battle_Poker_Has_Discards( pack, byPokers, bySize, byUserSign )  ) {
                return;
            }

            if ( !Battle_Poker_Compare( pack, byPokers, bySize) ) {
                return;
            }

            pack.GetFirst() = false;
            pack.GetThanSeatId() = _seatid;
            pack.GetThanPokerSize() = bySize;
            char * pDis = pack.GetThanPokers();
            *pDis = '\0';
            strcat( pDis, _szPokers);

            Battle_Get_Usercards(pack, byUserSign, _seatid);

            pack.GetProtocol() = MAKEDWORD( Games_Protocol, Discards_BRD );
            g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );

            // ��������߿��Լ�������
            pack.GetPlaySeatId() = (++_seatid)%TEAM_SIZE;
            if ( pack.GetThanType()==PH_MAX) {
                pack.GetPlaySeatId() = _seatid;
                pack.GetFirst() = true;
            }

            pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
            g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
            return;
        }
    }
    else {
        if ( pack.GetFirst()==true ) {
            return; // ��һ�������߲��ܲ�����
        }
    }

    pack.GetDisplayPokerSize( _seatid ) = 0;
    char * pDis = pack.GetDisplayPokers( _seatid );
    *pDis = '\0';
    strcat( pDis, "-1");
    pack.GetProtocol() = MAKEDWORD( Games_Protocol, Discards_BRD );
    g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );

    {
        pack.GetPlaySeatId() = (++_seatid)%TEAM_SIZE;
        pack.GetProtocol() = MAKEDWORD( Games_Protocol, DiscardsLicense_BRD );
        g_pCnpokerServer->SendToAgentServer( (BYTE*)&pack, pack.GetPacketSize() );
    }
    return;
}


/*****************************************************
    Battle_Poker_Get_Byte_Array : ת��Ϊ���±�
*****************************************************/
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


/*****************************************************
    Battle_Poker_Parser : �ж��±��Ƿ����
*****************************************************/
int Battle_Poker_Has_Discards ( TablePacket & pack, BYTE * byIndex, BYTE bySize, BYTE byUserSign )
{
    BYTE byCount(0), index(0);
    BYTE * pMove = pack.GetPokers();
    for (int i=0; i<(bySize); i++) {
        index = byIndex[i];
        if ( index<MAX_POKER ) {
            if (pMove[index]==byUserSign) {
               byCount++;
            }
        }
    }
    if (byCount==bySize)
    {
        for (int i=0; i<(bySize); i++) {
        index = byIndex[i];
            if ( index<MAX_POKER ) {
                if (pMove[index]==byUserSign) {
                   pMove[index]=PK_DISCARD;
                }
            }
        }
        return true;
    }
    return false;
}

/*****************************************************
    Battle_Poker_Transform : �±�ת����ֵ
*****************************************************/
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


/*****************************************************
    Battle_Poker_Parser : ֵ�Ʒ���
*****************************************************/
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
		return PH_0;
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
	_Min_Count = _Max_Count;
	for (short i=0; i<(short)bySize; ++i)
	{
		_idx = byPK[i];
		_val = _PK_Slot[_idx];
		_Min_Count = (_val < _Min_Count) ? _val : _Min_Count; // �ͼ�����ȡ
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
					return (PH_C2); // ����
				else
					return (PH_0); // ��������
			}
			if ( _Max_Count == 3 ) {
				if ( (_Div_2 >= 2) && (_Div_3 == 0) )
					return (PH_C3); // �ɻ�
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


/*****************************************************
    Battle_Poker_Compare : �ƱȽ�
*****************************************************/
int Battle_Poker_Compare( TablePacket & pack, BYTE * pbyIndex, BYTE byIndexSize)
{
    BYTE byThanType, byThanValue, byThanCount;
    BYTE _byType, _byValue, _byCount, _bySize;

    // ��ת��Ϊһ������
    BYTE pbyPK[MAX_PK_TYPE] = {0};
    Battle_Poker_Transform( pbyIndex, byIndexSize, pbyPK, MAX_PK_TYPE );

    byThanType  = Battle_Poker_Parser(pbyPK, byIndexSize, byThanValue, byThanCount);

    // step1: �����������⣬ֱ�ӷ��ش���;
    if ( byThanType==PH_0 ) {
        goto Compare_Wrong;
    }

    if (pack.GetFirst()==true) {
        goto Compare_Correct;   // ͬһ���˳���
    }

    _byType  = pack.GetThanType();
    _byValue = pack.GetThanValue();
    _byCount = pack.GetThanCount();
    _bySize  = pack.GetIndexSize();

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
    pack.GetThanType () = byThanType;
    pack.GetThanValue() = byThanValue;
    pack.GetThanCount() = byThanCount;
    pack.GetIndexSize () = byIndexSize;
    return byThanType;

Compare_Wrong:
    return FALSE;
}


/*****************************************************
    Battle_Hava_Continue : �Ƿ���Ҫ��������
*****************************************************/
int Battle_Hava_Continue ( TablePacket & pack )
{
    return true;
}

/*****************************************************
    Battle_Get_Usercards
*****************************************************/
BYTE Battle_Get_Usercards( TablePacket & pack, BYTE byVal, BYTE seatId ) {

    char * poker = pack.GetDisplayPokers(seatId);
    BYTE * pMove = pack.GetPokers();
    char szPoker[8] = {0};
    char szPokerList[128] = {0};
    BYTE byPoker(0), byCount(0);
    for (int  i=0; i<POKER_SIZE; i++) {
        byPoker = pMove[i];
        if ( byPoker == byVal ) {
            memset( szPoker, 0x0, sizeof(szPoker) );
            if ( byCount!=0 ) {
                strcat( szPokerList, ",");
            }
            sprintf( szPoker, "%d", i );
            strcat( szPokerList, szPoker);
            byCount++;
        }
    }
    DEBUG_MSG( LVL_DEBUG, "byCount = %d ",  byCount);
    *poker = '\0';
    strcat( poker, szPokerList);
    pack.GetDisplayPokerSize(seatId) = byCount;
}
