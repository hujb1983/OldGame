#ifndef _CONST_H_INCLUDED_
#define _CONST_H_INCLUDED_

// 服务器类别
enum eSERVER_TYPE
{
	UNKNOWN_SERVER 	 = 0,

	AGENT_SERVER		, 		// 代理服务器
	GAME_SERVER 		,		// 游戏服务器
	DB_SERVER           ,       // 数据库服务器
	LOGIN_SERVER 		,		// 登录服务器
	WORLD_SERVER 		,		// 聊天服务器
	LOBBY_SERVER 		,		// 大厅
	TEMP_SERVER 		,		// 临时
};

enum ePK_TYPE   // 出牌的类型
{
    PH_0 	= 0    , 		// 无效
    PH_1 	= 10   , 		// 单牌
    PH_2 	       , 		// 对子
    PH_3 	       ,		// 三个
    PH_31 	       ,		// 三带一
    PH_32 	       ,		// 三带二
    PH_41 	       ,		// 四带一
    PH_411 	       ,		// 四带两单
    PH_42 	       ,		// 四带一对
    PH_422 	       ,		// 四带两对
    PH_C1 	       , 		// 顺子
    PH_C2 	       ,		// 连对
    PH_C3 	       ,		// 飞机1
    PH_C31 	       ,		// 飞机2
    PH_C32 	       ,		// 飞机3
    PH_4           ,		// 炸弹
    PH_MAX 	       ,		// 王炸
    PH_End  = 0xFF ,		// 无效
};


// 设置 m_byTableStatus[x] 状态
enum
{
    eGB_EMPTY   = 0x00,
    eGB_LEAVE   = 0x01,
    eGB_SIT     = 0x02,   // 坐下阶段
    eGB_DEALING = 0x03,   // 发牌阶段
    eGB_CALLING = 0x05,   // 叫牌阶段
    eGB_PLAYING = 0x06,   // 正完阶段

};

// 设置 m_byStart[] 和 m_byShow[] 状态
enum
{
    eGB_Ready   = 0x01,   // 开始游戏
    eGB_Show    = 0x02,   // 明牌游戏
};

enum
{
    eGB_Banker  = 0x10,   // 庄家
    eGB_Player  = 0x20,   // 贤家
};

// 设置 m_byOnline[] 现在状态
enum
{
    eGB_Online  = 0x01,   // 在线
    eGB_Offline = 0x02,   // 离线
    eGB_Leave   = 0x03,   // 离开
};

// 设置 m_byCalls[] 状态
enum
{
    eGB_Apply   = 0x01,   // 申请
    eGB_Waiver  = 0x02,   // 放弃
};

// 设置 m_byUsercards [] 状态
enum
{
    eGB_Not   = 0x00,   // 争取
    eGB_Have  = 0x01,   // ...
    eGB_Disc  = 0x02,   // ...
};

// 设置 m_byUsercards [] 状态
enum
{
    eGB_HaveNot      = 0x00,   // 没有启动
    eGB_TimerOpen    = 0x01,   // 已经打开
    eGB_TimerClose   = 0x02,   // 已经关闭
    eGB_TimerLock    = 0x03,   // 已经锁定
};

enum
{
    eGB_Trusteeship_No     = 0x00,   // 没有托管
    eGB_Trusteeship_Yes    = 0x01,   // 已经托管
};

#endif // _CONST_H_INCLUDED_
