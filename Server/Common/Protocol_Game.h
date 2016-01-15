#ifndef _PROTOCOLAG_H_INCLUDED_
#define _PROTOCOLAG_H_INCLUDED_

/***** 游戏服务器 -> 代理服务器 *****/
/***** Agent -> Game *****/

enum eGAME_PROTOCOL
{
    StartGame_REQ           = 300,  // 开始游戏
	StartGame_BRD,                  // 游戏应答
	StartGame_NAK,
	StartGame_DBR,                  // 游戏DBR

	SelectRoom_REQ          = 310, 	// 选择房间请求：【玩家X】
	SelectRoom_BRD, 		        // 选择房间应答：【房间ID, 玩家{1,2,3}】
	SelectRoom_NAK,
    SelectRoom_DBR,

	FastGame_REQ            = 320,  // 快速加入游戏
	FastGame_ANC, 			        // 广播错误消息
	FastGame_DBR,                   // 数据库请求

	JoinTable_REQ           = 340, 	// 加入桌子请求
	JoinTable_BRD, 			        // 加入桌子应答
	JoinTable_NAK,                  // 加入桌子错误
	JoinTable_DBR, 			        // 加入桌子应答
	JoinTable_ANC, 			        // 加入桌子应答

	CreateTable_DBR         = 345, 	// 创建桌子数据

	QuitTable_REQ           = 350, 	// 加入桌子请求
	QuitTable_BRD,
	QuitTable_NAK,
	QuitTable_DBR,

	InitCards_REQ           = 360,  // 初始化发牌
	InitCards_BRD,
	InitCards_NAK,

	Called_REQ              = 370, 	// 叫庄请求
	Called_BRD,
	Called_NAK,
	Called_DBR,

	CalledLicense_REQ       = 380, 	// 出牌许可
	CalledLicense_BRD,
	CalledLicense_NAK,

	CreateBank_REQ          = 390, 	// 创建一个庄
	CreateBank_BRD,
	CreateBank_NAK,

	ShowCards_REQ           = 400, 	// 明牌
	ShowCards_BRD,
	ShowCards_NAK,
	ShowCards_DBR,

	DiscardsLicense_REQ     = 410, 	// 出牌许可
	DiscardsLicense_BRD,
	DiscardsLicense_NAK,

	Discards_REQ            = 420, 	// 玩家出牌
	Discards_BRD,
	Discards_NAK,
	Discards_DBR,

	Reminder_REQ            = 425, 	// 出牌提示
	Reminder_BRD,

	Settlement_REQ          = 430, 	// 结算
	Settlement_BRD,
	Settlement_NAK,
	Settlement_DBR,

	CalledOverTime_NAK      = 440,  // 叫牌超时通知
	DiscardsOverTime_NAK,

	Trusteeship_REQ         = 450,  // 托管请求
	Trusteeship_BRD,
	Trusteeship_NAK,

	UpdateBattle_REQ        = 460,  // 更新请求
	UpdateBattle_DBR,
	UpdateBattle_NAK,

	ErroNo_BRD              = 500,   // 错误广播
};

#endif // _PROTOCOLGA_H_INCLUDED_
