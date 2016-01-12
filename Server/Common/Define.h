#ifndef DEFINE_H_INCLUDED_
#define DEFINE_H_INCLUDED_


/*******************************************
    LoginServer
*******************************************/
#define CODE_KEY_LEN 			( 32 ) 		// LoginSrv密钥长度
#define AGENT_KEY_LEN 			( 24 ) 		// Agent密钥长度
#define TICKET_LEN 				( 64 ) 		// Ticket长度
#define MAX_CODE_LEN 			( 1024 ) 	// 加密数据Buff大小

#define MAX_PLAYER_NUM  		( 1000 )

#define IP_MAX_LEN 				( 32 )
#define PORT_MAX				(0xFFFF)

#define REQUEST_SAFETY			( 3 )


/*******************************************
    GamePoker && GameBattle
*******************************************/
#define MAX_BHOLD   ( 20 )
#define MAX_PHOLD   ( 17 )
#define MAX_PK_TYPE ( 15 )
#define MAX_POKER   ( 54 )

/*******************************************
    UserPacket && TablePacket
*******************************************/
#define TEAM_SIZE       (3)
#define POKER_SIZE      (54)

#define USER_PLAYER     (17)        // 玩家0
#define USER_BANKER     (20)        // 玩家1

#define PK_DISCARD      (99)        // 已经丢牌
#define PK_USER_0       (100)       // 玩家0
#define PK_USER_1       (101)       // 玩家1
#define PK_USER_2       (102)       // 玩家2
#define PK_BANKER       (103)       // 地主牌标识

#define PK_READY        (1)         // 准备好了
#define PK_CALLED       (1)         // 已经叫地主
#define PK_ONLINE       (1)         // 在线
#define PK_TRUSTEESHIP  (1)         // 托管好了
#define PK_INITCARDS    (1)         // 初始化牌


#endif // DEFINE_H_INCLUDED_
