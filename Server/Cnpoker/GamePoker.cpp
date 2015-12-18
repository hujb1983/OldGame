#include "GamePoker.h"

GamePoker::GamePoker()
{

}

GamePoker::~GamePoker()
{

}

void GamePoker::SetPoker( BYTE * _byIndex, BYTE _byIndexSize )
{
    memcpy( PokerTypeTable,  0x0, sizeof(PokerTypeTable) );
    memcpy( PokerIndexArray, 0x0, sizeof(PokerIndexArray) );
    memcpy( PokerIndexTable, 0x0, sizeof(PokerIndexTable) );

    BYTE j = 0;
    BYTE pv = 0;
    for ( int i=0; i<_byIndexSize; i++) {
        pv = _byIndex[i];
        PokerIndexArray[i]  = pv;
        PokerIndexTable[pv] = 1;
        if ( pv < MAX_POKER ) {
            j = pv % 13;
            PokerTypeTable[j]++;
			if ( pv==52) {
				PokerTypeTable[13] = 1;
			}
			else if ( pv==53) {
				PokerTypeTable[14] = 1;
			}
		}
    }

    PokerSize = _byIndexSize;
}

void GamePoker::Parser( BYTE * _poker, BYTE _size )
{
    SetPoker( _poker, _size);

    // 单牌的分析;
    Poker_1g_size       = _size; // 牌的整数;

    int  i  = 0;  // 指定全过程下标;
    BYTE pv = 0;
    BYTE pv_old   = -1;
    BYTE pv_count = 0;

    // 先做初始化循环
    for ( i=0; i<(MAX_PK_TYPE-2); i++)
    {
        pv = PokerTypeTable[i];
        if ( pv==0 ) {
            continue;
        }

        // TODE: 单牌的启始值;
        if ( Poker_1g_size==0 ) {
            Poker_1g_max_value = pv;
            Poker_1g_min_value = pv;
        }

        // TODE: 2牌的启始值;
        else if ( (pv==2) && (Poker_2g_size==0) ) {
            Poker_2g_max_value = pv;
            Poker_2g_min_value = pv;
        }

        // TODE: 3牌的启始值;
        else if ( pv==3 && (Poker_3g_size==0) ) {
            Poker_3g_max_value = pv;
            Poker_3g_min_value = pv;
        }

        // TODE4 4牌的启始值;
        else if ( pv==4 && (Poker_4g_size==0) ) {
            Poker_4g_max_value = pv;
            Poker_4g_min_value = pv;
        }
    }

    // 再进行排比查找
    for ( i=0; i<(MAX_PK_TYPE-2); i++)
    {
        pv = PokerTypeTable[i];

        if ( pv==0 ) {          // 为零的时候初始化
            pv_old   = -1;
            pv_count =  0;
            continue;
        }

        if ( pv==pv_old ) {
            pv_count++;        // 数量连接相同，可能是顺子
        }
        else {
            pv_old   = pv;
            pv_count = 1;
        }

        // TODE: 单牌的最大值;
        if ( pv==1 ) {
            Poker_1g_size++;
            Poker_1g_max_value = pv;
            Poker_1g_seq_size = ( pv_count > Poker_1g_seq_size ) ? pv_count : Poker_1g_seq_size;
        }

        // TODE: 2牌的最大值;
        else if ( pv==2 ) {
            Poker_2g_size++;
            Poker_2g_max_value = pv;
            Poker_2g_seq_size = ( pv_count > Poker_2g_seq_size ) ? pv_count : Poker_2g_seq_size;
        }

        // TODE: 3牌的最大值;
        else if ( pv==3 ) {
            Poker_3g_size++;
            Poker_3g_max_value = pv;
            Poker_3g_seq_size = ( pv_count > Poker_3g_seq_size ) ? pv_count : Poker_3g_seq_size;
        }

        // TODE4 4牌的最大值;
        else if ( pv==4 ) {
            Poker_4g_size++;
            Poker_4g_max_value = pv;
        }
    }

    // 小王的分析;
    if ( PokerTypeTable[13]==1 ) {
        Poker_king_size++;
        Poker_king_min_value = 13;
        Poker_1g_max_value   = 13;      // 也算单牌
    }

    // 大王的分析;
    if ( PokerTypeTable[14]==1 ) {
        Poker_king_size++;
        Poker_king_max_value = 14;
        Poker_1g_max_value   = 14;      // 也算单牌
    }
}

// 判断是否有大于他手上的牌;
BOOL GamePoker::MoreThan( BYTE _type, BYTE _size, BYTE _value, BYTE _count)
{
    // 握有双王无敌，直接返回;
    if ( Poker_king_size==2 ) {
        return TRUE;
    }

    // 不是炸弹，而你有炸弹，直接返回;
    if ( _type!=PH_MAX && _type!= PH_4 ) {
        if ( Poker_4g_size>0 ) {
            return TRUE;
        }
    }

    // 来的天炸，直接返回;
    if ( _type==PH_MAX ) {
        return FALSE;
    }

    // 来的天炸，直接返回;
    if ( _type==PH_4 ) {
        if ( Poker_4g_size>0 ) {
            if ( Poker_4g_max_value > _value ) {
                return TRUE;
            }
            return FALSE;
        }
        else {
            return FALSE;
        }
    }

    if ( _type==PH_1 ) {
        if ( Poker_1g_max_value>_value ) {
            return TRUE;
        }
        return FALSE;
    }
    else if ( _type==PH_2 ) {
        if ( Poker_2g_max_value>_value ) {
            return TRUE;
        }
        return FALSE;
    }
    else if ( _type==PH_3 ) {
        if ( Poker_3g_max_value>_value ) {
            return TRUE;
        }
        return FALSE;
    }
    else if ( _type==PH_31 ) {
        if ( PokerSize>3 ) {
            if ( Poker_3g_size>0 ) {
                if ( Poker_3g_max_value>_value ) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else if ( _type==PH_32 ) {
        if ( PokerSize>4 ) {
            if ( Poker_3g_size>0 && Poker_2g_size>0 ) {
                if ( Poker_3g_max_value>_value ) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else if ( _type==PH_41 ) {
       if ( PokerSize>4 ) {
            if ( Poker_4g_size>0 ) {
                if ( Poker_4g_max_value>_value ) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else if ( _type==PH_411 ) {
       if ( PokerSize>5 ) {
            if ( Poker_4g_size>0 ) {
                if ( Poker_4g_max_value>_value ) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else if ( _type==PH_42 ) {
       if ( PokerSize>5 ) {
            if ( Poker_4g_size>0 && Poker_2g_size>0 ) {
                if ( Poker_4g_max_value>_value ) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else if ( _type==PH_422 ) {
       if ( PokerSize>7 ) {
            if ( Poker_4g_size>0 && Poker_2g_size>1 ) {
                if ( Poker_4g_max_value>_value ) {
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    else if ( _type==PH_C1 ) {
        if ( Poker_1g_seq_size >= _count ||
             Poker_2g_seq_size >= _count ||
             Poker_3g_seq_size >= _count ) {
            return TRUE;
        }
        return FALSE;
    }
    else if ( _type==PH_C2 ) {
        if ( Poker_2g_seq_size >= _count ||
             Poker_3g_seq_size >= _count ) {
            return TRUE;
        }
        return FALSE;
    }
    else if ( _type==PH_C3 ) {
        if ( Poker_3g_seq_size >= _count ) {
            return TRUE;
        }
        return FALSE;
    }
}

// 匹配出1个对象
BOOL GamePoker::MarchObject ( BYTE & _type, BYTE & _size, BYTE & _than_value, BYTE & _than_count )
{
    _type = PH_0;
    _size = PokerSize;
    _than_value  = 0;
    _than_count  = 0;
    // _attach_type = PH_0;

    if ( _size==1 ) { // 单牌
        _type       = PH_1;
        _than_value = Poker_1g_min_value;
        _than_count = 1;
        return TRUE;
    }

    if ( _size == 2 ) {
        if ( Poker_2g_size==1 ) {    // 一个队子;
            _type = PH_2;
            _than_value = Poker_2g_min_value;
            _than_count = 2;
            return TRUE;
        }
        else if ( Poker_king_size==2 ) {  // 握有双王;
            _type = PH_MAX;
            return TRUE;
        }
        return FALSE;
    }

    if ( _size==3 ) {
        if ( Poker_3g_size==1 ) {    // 三个;
            _type = PH_3;
            _than_value = Poker_3g_min_value;
            _than_count = 3;
            return TRUE;
        }
        return FALSE;
    }

    if ( _size==4 ) {
        if ( Poker_3g_size==3 ) {    // 3带1;
            _type = PH_31;
            _than_value = Poker_3g_min_value;
            _than_count = 3;
            return TRUE;
        }
        else if ( Poker_4g_size==4 ) {    // 炸弹;
            _type = PH_4;
            _than_value = Poker_4g_min_value;
            _than_count = 4;
            return TRUE;
        }
        return FALSE;
    }

    if ( _size>4 ) {
        if ( Poker_1g_size>4 ) {    // 顺子
            BYTE byLen = Poker_1g_max_value - Poker_1g_max_value + 1;
            if ( byLen==Poker_1g_size ) {
                _type = PH_C1;
                _than_value = Poker_1g_max_value;
                _than_count = Poker_1g_size;
                return TRUE;
            }
        }
        else if ( Poker_2g_size>2) {    // 连队
            BYTE byLen  = Poker_2g_max_value - Poker_2g_min_value + 1;
            _than_value = Poker_2g_max_value;
            _than_count = Poker_2g_size;
            if ( byLen==Poker_2g_size ) {
                _type = PH_C2;
                return TRUE;
            }
        }
        else if ( Poker_3g_size==1 ) {  // 3带;
            _than_value = Poker_3g_max_value;
            _than_count = Poker_3g_size;
             if ( Poker_2g_size==Poker_3g_size ) {
                if ( _size==5 ) {
                    _type = PH_32;
                    return TRUE;
                }
            }
            return TRUE;
        }
        else if ( Poker_3g_size>1 ) {  // 3飞机带;
            BYTE byLen  = Poker_3g_max_value - Poker_3g_min_value + 1;
            _than_value = Poker_3g_max_value;
            _than_count = Poker_3g_size;
            if ( byLen==Poker_3g_size ) {
                if ( _size==(Poker_3g_size/3) ) {
                    _type = PH_C3;
                    return TRUE;
                }
                else {
                    BYTE byNewSize = _size-(Poker_3g_size/3);
                    if ( Poker_1g_size==Poker_3g_size ) {
                        if ( byNewSize==Poker_1g_size ) {
                            _type        = PH_C31;  // 带一个
                            return TRUE;
                        }
                    }
                    else if ( Poker_2g_size==Poker_3g_size ) {
                        if ( byNewSize==(Poker_2g_size*2) ) {
                            _type        = PH_C32;  // 带两个
                            return TRUE;
                        }
                    }
                }
                return TRUE;
            }
        }
        else if ( Poker_4g_size==1 ) {
            _than_value = Poker_4g_max_value;
            _than_count = Poker_4g_size;
            if ( Poker_1g_size==2 ) {   // 4带两个单牌为6个
                if ( _size==6 ) {
                    _type = PH_411;
                    return TRUE;
                }
            }
            else if ( Poker_2g_size==1 ) {   // 4带1个双牌算两个单牌为6个
                if ( _size==6 ) {
                    _type = PH_411;
                    return TRUE;
                }
            }
            else if ( Poker_2g_size==2 ) {   // 4带两个双牌为8个
                if ( _size==8 ) {
                    _type = PH_422;
                    return TRUE;
                }
            }
            else if ( Poker_4g_size==1 ) {   // 4带另外4个算带双牌为8个
                if ( _size==8 ) {
                    _type = PH_422;
                    return TRUE;
                }
            }
        }
        return FALSE;
    }

    return FALSE;
}

// 合理的给初始值;
BOOL GamePoker::FindObjectMoreThanToClone( BYTE _type, BYTE _size, BYTE _than_value, BYTE _than_count )
{
    if ( _type==PH_MAX ) {
        return FALSE;
    }

    memcpy( PokerCloneArray, 0x0, sizeof(PokerCloneArray) );
    PokerCloneType  = _type;
    PokerCloneValue = _than_value+1;
    PokerCloneCount = _than_count;
    BYTE &_val = PokerCloneValue;
    BYTE &_len = PokerCloneCount;
    if ( _type==PH_1 ) {
        MainSearch(1, _val, _len);
    }
    else if ( _type==PH_2 ) {
        MainSearch(2, _val, _len);
    }
    else if ( _type==PH_3 ) {
        MainSearch(3, _val, _len);
    }
    else if ( _type==PH_4 ) {
        MainSearch(4, _val, _len);
    }
    else if ( _type==PH_C1 ) {
        _len = _than_count;
        MainSearch(1, _val, _len);
    }
    else if ( _type==PH_31 ) {
        MainSearch(3, _val, _len);
        AttachSearch(1, 0, _len);
    }
    else if ( _type==PH_32 ) {
        if ( MainSearch(3, _val, _len)==TRUE ) {
            AttachSearch(2, 0, _len);
        }
    }
    else if ( _type==PH_C2 ) {
        _len = _than_count;
        MainSearch(2, _val, _len);
    }
    else if ( _type==PH_C3 ) {
        _len = _than_count;
        MainSearch(3, _val, _len);
    }
    else if ( _type==PH_411 ) {
        if ( MainSearch(4, _val, _len)==TRUE ) {
            if ( AttachSearch(1, 0, _len)==TRUE ) {
                AttachSearch(1, 0, _len);
            }
        }
    }
    else if ( _type==PH_42 ) {
        if ( MainSearch(4, _val, _len)==TRUE ) {
            AttachSearch(2, 0, _len);
        }
    }
    else if ( _type==PH_C31 ) {
        _len = _than_count;
        if ( MainSearch(3, _val, _len)==TRUE ) {
            AttachSearch(1, 0, _len);
        }
    }
    else if ( _type==PH_C32 ) {
        _len = _than_count;
        if ( MainSearch(3, _val, _len)==TRUE ) {
            AttachSearch(2, 0, _len);
        }
    }
    else if ( _type==PH_422 ) {
        if ( MainSearch(4, _val, _len)==TRUE ) {
            if ( AttachSearch(2, 0, _len)==TRUE ) {
                AttachSearch(2, 0, _len);
            }
        }
    }

    _val = 0;
    _len = 1;
    if (PokerCloneSize==0) {
        PokerCloneType = PH_4;
        MainSearch(4, _val, _len);
    }

    if (PokerCloneSize==0) {
        PokerCloneType = PH_MAX;
        if ( AttachSearch(1, 13, _len)==TRUE ) {
            AttachSearch(1, 14, _len);
        }
    }

    if (PokerCloneSize==0) {  // 找不到对应的克隆;
        PokerCloneType = PH_0;
        memcpy( PokerCloneArray, 0x0, sizeof(PokerCloneArray) );
    }

    return (PokerCloneSize!=0);
}

BOOL GamePoker::ClonePoker( BYTE * _poker, BYTE & _size )
{
    if (_size<PokerCloneSize) {
        return FALSE;
    }

    if (PokerCloneSize==0) {
        return FALSE;
    }

    short shIdx = 0;
    BYTE _count = 0;
    BYTE _pv    = 0;
    for ( int _i=0; _i<PokerCloneSize; _i++) {
        _pv = PokerCloneArray[_i];
        if ( _pv<13 ) {
            shIdx = Transform(_pv);
            if ( shIdx != -1 ) {
                _poker[_i] = shIdx;
            }
		}
		else if ( _pv==13 ) {
            _poker[_i] = 52;
		}
		else if ( _pv==13 ) {
            _poker[_i] = 53;
		}
    }

    _size = PokerCloneSize;
    return TRUE;
}

void GamePoker::ClonePokerType(BYTE & _type, BYTE & _size, BYTE & _value, BYTE & _count) {
    _type  = PokerCloneType;
    _size  = PokerCloneSize;
    _value = PokerCloneValue;
    _count = PokerCloneCount;
}

short GamePoker::Transform( BYTE _value )
{
    if ( _value<MAX_POKER ) {
        BYTE _index = _value;
        if (PokerIndexTable[_value]==1) {
            PokerIndexTable[_value] = 0;
            return PokerIndexTable[_value];
        }
        _index += 13;
        if (PokerIndexTable[_value]==1) {
            PokerIndexTable[_value] = 0;
            return PokerIndexTable[_value];
        }
        _index += 13;
        if (PokerIndexTable[_value]==1) {
            PokerIndexTable[_value] = 0;
            return PokerIndexTable[_value];
        }
        _index += 13;
        if (PokerIndexTable[_value]==1) {
            PokerIndexTable[_value] = 0;
            return PokerIndexTable[_value];
        }
    }
    return -1;
}

BOOL GamePoker::MainSearch(BYTE _size, BYTE & _value, BYTE _len)
{
    int  _i     = 0;
    BYTE _count = 0;
    BYTE _pv    = 0;
    for ( _i=_value; _i<MAX_PK_TYPE; _i++) {
        _pv = PokerTypeTable[_i];
        if ( _pv >= _size ) {
            _count++;
		}
		else {
            _count = 0;
		}
		if ( _count==_len ) {
            break;
		}
    }

    if ( _count==0 ) {
        PokerCloneSize = 0;
        return FALSE;
    }

    int  _c = PokerCloneSize - 1;
    int  _j = _i-_len;
    for ( ; _j<_i; _j++) {
        PokerCloneArray[_c] = PokerTypeTable[_j];
    }

    PokerCloneSize += _count;
    return TRUE;
}

BOOL GamePoker::AttachSearch(BYTE _size, BYTE _value, BYTE _len)
{
    int  _i     = 0;
    BYTE _count = 0;
    BYTE _pv    = 0;
    for ( _i=_value; _i<MAX_PK_TYPE; _i++) {
        _pv = PokerTypeTable[_i];
        if ( _pv >= _size ) {
            _count++;
		}
		else {
            _count = 0;
		}
		if ( _count==_len ) {
            break;
		}
    }

    if ( _count==0 ) {
        PokerCloneSize = 0;
        return FALSE;
    }

    int  _c = PokerCloneSize - 1;
    int  _j = _i-_len;
    for ( ; _j<_i; _j++) {
        PokerCloneArray[_c] = PokerTypeTable[_j];
    }

    PokerCloneSize += _count;
    return TRUE;
}

// 合理的给初始值;
void GamePoker::Zero()
{
    Poker_1g_size          = 0;
    Poker_1g_max_value     = 0;
    Poker_1g_min_value     = 0;
    Poker_1g_seq_size      = 0;
    Poker_1g_seq_max_value = 0;
    Poker_1g_seq_min_value = 0;
    Poker_2g_size          = 0;
    Poker_2g_max_value     = 0;
    Poker_2g_min_value     = 0;
    Poker_2g_seq_size      = 0;
    Poker_2g_seq_max_value = 0;
    Poker_2g_seq_min_value = 0;
    Poker_3g_size          = 0;
    Poker_3g_max_value     = 0;
    Poker_3g_min_value     = 0;
    Poker_3g_seq_size      = 0;
    Poker_3g_seq_max_value = 0;
    Poker_3g_seq_min_value = 0;
    Poker_4g_size          = 0;
    Poker_4g_max_value     = 0;
    Poker_4g_min_value     = 0;
    Poker_king_size        = 0;
    Poker_king_max_value   = 0;
    Poker_king_min_value   = 0;
}

