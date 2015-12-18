DROP TABLE IF EXISTS `tb_gametype`;
CREATE TABLE `tb_gametype` (
  `gameid` int(10) unsigned NOT NULL,
  `gname` varchar(128) NOT NULL,
  `status` tinyint(4) NOT NULL,
  PRIMARY KEY (`gameid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_gametype` WRITE;
UNLOCK TABLES;


DROP TABLE IF EXISTS `tb_usergame`;
CREATE TABLE `tb_usergame` (
  `aid` int(10) unsigned NOT NULL,
  `gameid` int(10) unsigned NOT NULL,
  `status` tinyint(4) NOT NULL,
  PRIMARY KEY (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_usergame` WRITE;
INSERT INTO `tb_usergame` VALUES 
	(100,1,1),(101,2,0);
UNLOCK TABLES;

DROP TABLE IF EXISTS `tb_roominfo`;
CREATE TABLE `tb_roominfo` (
  `roomid`   tinyint(3) 	unsigned NOT NULL,
  `tableid`  tinyint(3) 	unsigned NOT NULL,
  `status`   tinyint(3) 	unsigned DEFAULT 0,
  `tdouble`  tinyint(1) 	unsigned DEFAULT 0,
  `money`    int(12)    	unsigned DEFAULT 0,
  `remark`   varchar(30)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

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



DROP TABLE IF EXISTS `tb_userlog`;
CREATE TABLE `tb_userlog` (
  `uid`     int(10) 		unsigned NOT NULL,
  `gid`     int(10) 		unsigned NOT NULL,
  `btime`   datetime,
  `etime`   datetime,
  `points`  int(12)     		 DEFAULT 0,
  `wons`    tinyint(1)  unsigned DEFAULT 0,
  `faileds` tinyint(1)  unsigned DEFAULT 0,
  `aways`   tinyint(1)  unsigned DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_userlog` WRITE;
INSERT INTO `tb_userlog` VALUES 
	(555, 1, now(), now(), 0, 0, 0, 1), (555, 1, now(), now(), -50, 0, 1, 0)
UNLOCK TABLES;

