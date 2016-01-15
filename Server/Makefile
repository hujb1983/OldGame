CC = g++
CFLAGS = -g -fPIC -D_FILELINE -Wno-deprecated 
#HOME = /mnt/Shared/moche 
HOME = /usr/cnpoker/cngame_json_v0.01/Server

MYSQLINC = /usr/include/mysql
MYSQLLIB = -L /usr/lib64/mysql -l mysqlclient -lz -lm

base_INC = 	-I $(HOME)/Utility \
			-I $(HOME)/Common  \
			-I $(HOME)/HyMysql \
			-I $(HOME)/Network \
			-I $(HOME)/Public \
			-I $(MYSQLINC)
	  
agent_INC 	= 	-I $(HOME)/Agent 
cnpoker_INC = 	-I $(HOME)/Cnpoker 
lobby_INC 	= 	-I $(HOME)/Lobby
db_INC 		= 	-I $(HOME)/DB
	  
	  
PublicObjs 		= Public/InfoParser.o \
				  Public/JsonParser.o \
				  Public/HeadPacket.o \
				  Public/UserPacket.o \
				  Public/LobbyPacket.o \
				  Public/TablePacket.o \
				  Public/MsgBuff.o \
				  Public/Yond_drng.o \
				  Public/Yond_json.o \
				  Public/Yond_string.o \
				  Public/Yond_md5.o 
	  
UtilityObjs 	= Utility/SysLog.o \
				  Utility/Yond_mutex.o \
				  Utility/Yond_ini.o \
				  Utility/Yond_thread.o \
				  Utility/Yond_thread_pool.o \
				  Utility/ObjKeyGenerator.o

NetworkObjs 	= Network/Acceptor.o \
				  Network/Connector.o \
				  Network/IOCPServer.o \
				  Network/NetworkObject.o \
				  Network/Session.o \
				  Network/SessionList.o \
				  Network/SessionPool.o \
				  Network/SocketOpt.o \
				  Network/SyncHandler.o
		  
MysqlObject 	= HyMysql/HyDatabase.o \
				  HyMysql/IDBCInterface.o \
				  HyMysql/IMysql.o \
				  HyMysql/QueryExecuteProcessor.o \
				  HyMysql/QueryResult.o \

LoginObjs       = Login/DBSession.o \
				  Login/LoginFactory.o \
				  Login/LoginMain.o \
				  Login/LoginServer.o \
				  Login/ServerSession.o \
				  Login/UserSession.o \
				  Login/PacketHandler.o \
				  Login/Handler_PreLogin.o
				
CnpokerObjs     = Cnpoker/CnpokerMain.o \
				  Cnpoker/CnpokerFactory.o \
				  Cnpoker/CnpokerServer.o \
				  Cnpoker/ServerSession.o \
				  Cnpoker/TempSession.o \
				  Cnpoker/DBSession.o \
				  Cnpoker/AgentSession.o \
				  Cnpoker/GameMgr.o \
				  Cnpoker/GameTable.o \
				  Cnpoker/GamePoker.o \
				  Cnpoker/PacketHandler.o \
				  Cnpoker/Handler_JoinTable.o \
				  Cnpoker/Handler_StartGame.o \
				  Cnpoker/Handler_CalledBank.o \
				  Cnpoker/Handler_Discard.o \
				  Cnpoker/Handler_QuitTable.o \
				  Cnpoker/Handler_Trusteeship.o \
				  Cnpoker/Handler_Settlement.o \
				  Cnpoker/Handler_Reminder.o \

LobbyObjs   	= Lobby/LobbyMain.o \
				  Lobby/LobbyUser.o \
				  Lobby/LobbyUpdate_REQ.o \
				  Lobby/LobbyUpdate_ANC.o \
				  Lobby/LobbyUpdate_Base.o \
				  Lobby/LobbyUpdate_Json.o \
				  Lobby/LobbyUpdate_Query.o \
				  Lobby/LobbyFactory.o \
				  Lobby/LobbyServer.o \
				  Lobby/ServerSession.o \
				  Lobby/TempSession.o \
				  Lobby/DBSession.o \
				  Lobby/AgentSession.o \
				  Lobby/PacketHandler.o \
				  Lobby/Handler_Login.o \
				  Lobby/Handler_JoinTable.o \
				  Lobby/Handler_QuitTable.o \
				  Lobby/Handler_RoomInfo.o \
				  Lobby/Handler_TableInfo.o \
				  Lobby/Handler_Onlines.o \
				  Lobby/Handler_WRankInfo.o \
				  Lobby/Handler_DRankInfo.o \

AgentObjs   	= Agent/AgentMain.o \
				  Agent/AgentFactory.o \
				  Agent/AgentServer.o \
				  Agent/ServerSession.o \
				  Agent/TempSession.o \
				  Agent/UserSession.o \
				  Agent/GameSession.o \
				  Agent/LobbySession.o \
				  Agent/PacketHandler.o \
				  Agent/Handler_Login.o \
				  Agent/Handler_GamePacket.o \
				  Agent/Handler_RoomInfo.o \
				  Agent/Handler_TableInfo.o \
				  Agent/Handler_Onlines.o \
				  Agent/Handler_WRankInfo.o \
				  Agent/Handler_DRankInfo.o \
				  Agent/Handler_FastGame.o \
				  Agent/Handler_JoinTable.o \
				  Agent/Handler_QuitTable.o \
				  Agent/Handler_StartGame.o \
				  Agent/Handler_InitCards.o \
				  Agent/Handler_Settlement.o \
				  Agent/Handler_CalledBank.o \
				  Agent/Handler_CreateBank.o \
				  Agent/Handler_Discard.o \
				  Agent/Handler_Upline.o \
				  Agent/Handler_Offline.o \
				  Agent/Handler_CalledLicense.o \
				  Agent/Handler_DiscardsLicense.o \
				  Agent/Handler_CalledOvertime.o \
				  Agent/Handler_DiscardsOvertime.o \
				  Agent/Handler_Trusteeship.o

				 
DBObjs   		= DB/DBMain.o \
				  DB/DBFactory.o \
				  DB/DBServer.o \
				  DB/ServerSession.o \
				  DB/TempSession.o \
				  DB/LobbySession.o \
				  DB/GameSession.o \
				  DB/LoginSession.o \
				  DB/PacketHandler.o \
				  DB/Handler_Login.o \
				  DB/Handler_RoomInfo.o \
				  DB/Handler_TableInfo.o \
				  DB/Handler_Onlines.o \
				  DB/Handler_WRankInfo.o \
				  DB/Handler_DRankInfo.o \
				  DB/Handler_JoinTable.o \
				  DB/Handler_QuitTable.o \
				  DB/Handler_PreLogin.o \
				  DB/Handler_UpdateBattle.o \
				  DB/Handler_Settlement.o 
				  
ClientObjs   	= ClientAI/Handler_Packet.o \
				  ClientAI/Handler_Login.o \
				  ClientAI/Handler_GamePacket.o \
				  ClientAI/Handler_RoomInfo.o \
				  ClientAI/Handler_TableInfo.o \
				  ClientAI/Handler_Onlines.o \
				  ClientAI/Handler_WRankInfo.o \
				  ClientAI/Handler_DRankInfo.o \
				  ClientAI/Handler_JoinGame.o \
				  ClientAI/Handler_StartGame.o \
				  ClientAI/Handler_InitCards.o \
				  ClientAI/Handler_EnterType.o \
				  ClientAI/Handler_ShowCards.o \
				  ClientAI/Handler_CalledBank.o \
				  ClientAI/Handler_CreateBank.o \
				  ClientAI/Handler_CalledLicense.o \
				  ClientAI/Handler_DiscardsLicense.o \
				  ClientAI/Handler_Discard.o \
				  ClientAI/GameUser.o \
				  ClientAI/Client.o

BINDIR = bin

all: checkbin \
	$(BINDIR)/LoginServer \
	$(BINDIR)/DBServer \
	$(BINDIR)/AgentServer \
	$(BINDIR)/LobbyServer \
	$(BINDIR)/CnpokerServer \
	$(BINDIR)/Client

$(BINDIR)/LoginServer: $(UtilityObjs) $(PublicObjs) $(NetworkObjs) $(LoginObjs)
	$(CC) -g $^ -o $@ -pthread	

$(BINDIR)/AgentServer: $(UtilityObjs) $(PublicObjs) $(NetworkObjs) $(AgentObjs)
	$(CC) -g $^ -o $@ -pthread	

$(BINDIR)/CnpokerServer: $(UtilityObjs) $(PublicObjs) $(NetworkObjs) $(CnpokerObjs)
	$(CC) -g $^ -o $@ -pthread

$(BINDIR)/LobbyServer: $(UtilityObjs) $(PublicObjs) $(NetworkObjs) $(LobbyObjs)
	$(CC) -g $^ -o $@ -pthread

$(BINDIR)/DBServer: $(UtilityObjs) $(PublicObjs) $(NetworkObjs) $(MysqlObject) $(DBObjs)
	$(CC) -g $(MYSQLLIB) $^ -o $@ -pthread
	
$(BINDIR)/Client: $(UtilityObjs) $(PublicObjs) $(NetworkObjs) $(ClientObjs)
	$(CC) -g $^ -o $@ -pthread

.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CC) $(CFLAGS) $(base_INC) -o $@ -c $<

.PHONY: checkbin
checkbin:
	test -d "${BINDIR}" || mkdir $(BINDIR)

.PHONY:	clean cleanGame
clean:
	rm -f Network/*.o
	rm -f Utility/*.o
	rm -f Public/*.o
	rm -f HyMysql/*.o
	rm -f Lobby/*.o
	rm -f Login/*.o
	rm -f Agent/*.o
	rm -f Cnpoker/*.o
	rm -f DB/*.o
	rm -f ClientAI/*.o
	rm -f bin/LoginServer
	rm -f bin/AgentServer
	rm -f bin/CnpokerServer
	rm -f bin/LobbyServer
	rm -f bin/DBServer
	rm -f bin/Client