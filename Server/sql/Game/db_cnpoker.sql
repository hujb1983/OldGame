-- 用户游戏信息表
DROP TABLE IF EXISTS `tb_userinfo`;
CREATE TABLE `tb_userinfo` (
  `uid` int(10) unsigned NOT NULL,
  `gameid` int(10) unsigned NOT NULL,
  `points` int(10) unsigned NOT NULL,
  `wons` int(10) unsigned NOT NULL,
  `faileds` int(10) unsigned NOT NULL,
  `aways` int(10) unsigned NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_userinfo` WRITE;
UNLOCK TABLES;


-- 房间信息
DROP TABLE IF EXISTS `tb_roomlist`;
CREATE TABLE `tb_roomlist` (
  `roomid`   tinyint(3) 	unsigned NOT NULL,
  `name`  	 varchar(33)  			 NOT NULL,
  `status`   tinyint(3) 	unsigned DEFAULT 0,
  `doubles`  tinyint(1) 	unsigned DEFAULT 0,
  `mmax`     int(12)    	unsigned DEFAULT 0,
  `mmin`     int(12)    	unsigned DEFAULT 0,
  `remark`   varchar(30)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_roomlist` WRITE;
INSERT INTO `tb_roomlist` VALUES 
	( 1, '试玩场', 1, 1, 0, 0, 'OK' ),
	( 2, '初级场', 1, 1, 100, 10, 'OK' ),
	( 3, '中级场', 1, 1, 1000, 100, 'OK' ),
	( 4, '高级场', 1, 1, 10000, 1000, 'OK' ),
	( 5, '比赛场', 1, 1, 100000, 10000, 'OK' );
UNLOCK TABLES;


-- 桌子信息
DROP TABLE IF EXISTS `tb_tablelist`;
CREATE TABLE `tb_tablelist` (
  `roomid`   tinyint(3) 	unsigned NOT NULL,
  `tableid`  tinyint(3) 	unsigned NOT NULL,
  `name`     varchar(33)  	NOT NULL,
  `status`   tinyint(3) 	unsigned DEFAULT 0,
  `doubles`  tinyint(1) 	unsigned DEFAULT 0,
  `onlines`  tinyint(3) 	unsigned DEFAULT 0,
  `mmax`     int(12)    	unsigned DEFAULT 0,
  `mmin`     int(12)    	unsigned DEFAULT 0,
  `brkerage` int(12)    	unsigned DEFAULT 0,
  `remark`   varchar(30)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_tablelist` WRITE;
INSERT INTO `tb_tablelist` VALUES 
	( 1, 2, '试玩场-1', 1, 1, 200, 0, 0, 'OK' ),
	( 2, 1, '初级场-1', 1, 1, 200, 100, 10, 'OK' ),
	( 3, 1, '中级场-1', 1, 1, 200, 1000, 100, 'OK' ),
	( 4, 1, '高级场-1', 1, 1, 200, 10000, 1000, 'OK' ),
	( 5, 1, '比赛场-1', 1, 1, 200, 100000, 10000, 'OK' );
UNLOCK TABLES;


-- 出牌信息
DROP TABLE IF EXISTS `tb_discardlog`;
CREATE TABLE `tb_discard` (
  `roomid`    tinyint(3) 	unsigned NOT NULL,
  `tableid`   tinyint(3) 	unsigned NOT NULL,
  `seatid`    tinyint(3) 	unsigned NOT NULL,
  `userid`    int(12) 		unsigned DEFAULT 0,
  `stime`     tinyint(3) 	unsigned DEFAULT 0,
  `usercards` varchar(128)	NOT NULL,
  `discards`  varchar(128)	NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_discard` WRITE;
INSERT INTO `tb_discard` VALUES 
UNLOCK TABLES;


-- 结算信息
DROP TABLE IF EXISTS `tb_settlement`;
CREATE TABLE ``tb_gameinfo` (
  `gid`      tinyint(3) 	unsigned NOT NULL,
  `roomid`   tinyint(3) 	unsigned NOT NULL,
  `tableid`  tinyint(3) 	unsigned NOT NULL,
  `battleid` tinyint(3) 	unsigned NOT NULL,
  `stime`    tinyint(3) 	unsigned DEFAULT 0,
  `etime`    tinyint(3) 	unsigned DEFAULT 0,
  `userid1`  int(12) 		unsigned DEFAULT 0,
  `userid2`  int(12) 		unsigned DEFAULT 0,
  `userid3`  int(12) 		unsigned DEFAULT 0,
  `banker`   int(12) 		unsigned DEFAULT 0,
  `isdouble` tinyint(3),
  `mult`     varchar(4),
  `wonner`   int(12)    	unsigned DEFAULT 0,
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_gameinfo` WRITE;
INSERT INTO `tb_gameinfo` VALUES 
UNLOCK TABLES;

-- 账户日志信息
DROP TABLE IF EXISTS `tb_accountlog`;
CREATE TABLE `tb_userlog` (
  `uid`     int(10) 		unsigned NOT NULL,
  `gid`     int(10) 		unsigned NOT NULL,
  `update`  datetime,
  `points`  int(12)     		 DEFAULT 0,
  `wons`    tinyint(1)  	unsigned DEFAULT 0,
  `faileds` tinyint(1)  	unsigned DEFAULT 0,
  `aways`   tinyint(1)  	unsigned DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_userlog` WRITE;
INSERT INTO `tb_userlog` VALUES 
	(555, 1, now(), now(), 0, 0, 0, 1), (555, 1, now(), now(), -50, 0, 1, 0)
UNLOCK TABLES;

-- 客户端登陆信息
DROP TABLE IF EXISTS `tb_clientlog`;
CREATE TABLE `tb_userstatus` (
  `uid`     int(10) 		 unsigned NOT NULL,
  `gid`     int(10) 		 unsigned NOT NULL,
  `btime`   datetime,
  `etime`   datetime,
  `ip` 	    varchar(16)  	 DEFAULT 0,
  `port`    int(6)  	unsigned DEFAULT 0,
  `siteid`  tinyint(3)  unsigned DEFAULT 0,
  `tindex`  tinyint(3)  unsigned DEFAULT 0,
  `status`  tinyint(1)  unsigned DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_userstatus` WRITE;
INSERT INTO `tb_userstatus` VALUES 
	(555, 6, now(), now(), '127.0.0.1', 5600, 1, 1, 1)
UNLOCK TABLES;

-- 周报信息
DROP TABLE IF EXISTS ` tb_weekrank`;
CREATE TABLE `tb_weekrank` (
  `uid`     int(10) 		unsigned NOT NULL,
  `gid`   	int(10) 		unsigned NOT NULL,
  `update`  datetime,
  `name`    varchar(33)  			 NOT NULL,
  `rate`    tinyint(3)  	unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_weekrank` WRITE;
INSERT INTO `tb_weekrank` VALUES
	( 555, 6, now(), 'kim', 80 )
UNLOCK TABLES;


-- 日报信息
DROP TABLE IF EXISTS ` tb_dayrank`;
CREATE TABLE `tb_dayrank` (
  `uid`     int(10) 		unsigned NOT NULL,
  `gid`   	int(10) 		unsigned NOT NULL,
  `update`  datetime,
  `name`    varchar(33)  			 NOT NULL,
  `rate`    tinyint(3)  	unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_dayrank` WRITE;
INSERT INTO `tb_dayrank` VALUES
	( 555, 6, now(), 'kim', 80 )
UNLOCK TABLES;


-- 战斗房间信息
DROP TABLE IF EXISTS `tb_battleinfo`;
CREATE TABLE `tb_tableinfo` (
  `tid`      int(12)		unsigned NOT NULL,   
  `roomid`   tinyint(3) 	unsigned NOT NULL,
  `tableid`  tinyint(3) 	unsigned NOT NULL,
  `battleid` tinyint(3) 	unsigned NOT NULL,
  `seatid`   tinyint(3) 	unsigned NOT NULL,
  `status`   tinyint(3) 	unsigned DEFAULT 0,
  `banker`   tinyint(3) 	unsigned DEFAULT 0,
  `calls`    tinyint(3) 	unsigned DEFAULT 0,
  `shows`    tinyint(3) 	unsigned DEFAULT 0,
  `userid`   int(12) 		unsigned DEFAULT 0,
  `userkey`  int(12) 		unsigned DEFAULT 0,
  `agentkey` int(12) 		unsigned DEFAULT 0,
  `allcards` varchar(128),
  `lastcards` varchar(128)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_tableinfo` WRITE;
INSERT INTO `tb_tableinfo` VALUES 
	(3,1,1,3,0,0,0,0,0);
UNLOCK TABLES;