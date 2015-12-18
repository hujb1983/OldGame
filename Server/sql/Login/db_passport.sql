-- 平台的接入的接口表
DROP TABLE IF EXISTS `tb_access`;
CREATE TABLE `tb_access` (
  `aid` int(10) unsigned NOT NULL,
  `status` tinyint(4) NOT NULL,
  `atime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `sshkey` varchar(33) NOT NULL,
  `aip` varchar(16) NOT NULL,
  PRIMARY KEY (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_access` WRITE;
INSERT INTO `tb_access` VALUES 
	(100,1,'2015-09-04 02:14:17','123','10.60.22.10'),
	(102,1,'2015-09-10 08:34:52','123','10.60.22.10');
UNLOCK TABLES;

-- 平台的帐户表
DROP TABLE IF EXISTS `tb_account`;
CREATE TABLE `tb_account` (
  `uid` int(10) unsigned NOT NULL,
  `password` varchar(33) NOT NULL,
  `paidtotal` decimal(14,4) NOT NULL,
  `money` decimal(14,4) NOT NULL,
  `frozen` decimal(14,4) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_account` WRITE;
UNLOCK TABLES;

-- 服务器列表
DROP TABLE IF EXISTS `tb_allserver`;
CREATE TABLE `tb_allserver` (
  `srv_id` int(10) unsigned NOT NULL,
  `ip` varchar(16) NOT NULL,
  `port` int(10) unsigned NOT NULL,
  `gameid` int(10) unsigned NOT NULL,
  `logintype` tinyint(4) NOT NULL,
  `logins` int(10) unsigned NOT NULL,
  PRIMARY KEY (`srv_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 预录陆表
DROP TABLE IF EXISTS `tb_prelogin`;
CREATE TABLE `tb_prelogin` (
  `uid` int(10) unsigned NOT NULL,
  `status` tinyint(4) NOT NULL,
  `lasttime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `sshkey` varchar(33) NOT NULL,
  `srv_id` int(10) unsigned NOT NULL,
  `logintype` tinyint(4) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_prelogin` WRITE;
INSERT INTO `tb_prelogin` VALUES 
	(555,1,'2015-09-10 09:13:28','322842c2ab91400f806ddb8a8f0647c0',1,6);
UNLOCK TABLES;

-- 用户登录信息
DROP TABLE IF EXISTS `tb_user`;
CREATE TABLE `tb_user` (
  `aid` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL,
  `username` varchar(33) NOT NULL,
  `password` varchar(33) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `tb_user` WRITE;
INSERT INTO `tb_user` VALUES 
	(100,555,'kim','123qwe'),
	(101,666,'daniel','123qwe'),
	(102,888,'james','123qwe');
UNLOCK TABLES;
