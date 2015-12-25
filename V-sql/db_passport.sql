-- MySQL dump 10.13  Distrib 5.1.73, for redhat-linux-gnu (x86_64)
--
-- Host: localhost    Database: db_passport
-- ------------------------------------------------------
-- Server version	5.5.42-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `tb_access`
--

DROP TABLE IF EXISTS `tb_access`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_access` (
  `aid` int(10) unsigned NOT NULL,
  `status` tinyint(4) NOT NULL,
  `atime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `sshkey` varchar(33) NOT NULL,
  `aip` varchar(16) NOT NULL,
  PRIMARY KEY (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_access`
--

LOCK TABLES `tb_access` WRITE;
/*!40000 ALTER TABLE `tb_access` DISABLE KEYS */;
INSERT INTO `tb_access` VALUES (100,1,'2015-09-04 02:14:17','123','10.60.22.10'),(102,1,'2015-09-10 08:34:52','123','10.60.22.10');
/*!40000 ALTER TABLE `tb_access` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_account`
--

DROP TABLE IF EXISTS `tb_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_account` (
  `uid` int(10) unsigned NOT NULL,
  `password` varchar(33) NOT NULL,
  `paidtotal` decimal(14,4) NOT NULL,
  `money` decimal(14,4) NOT NULL,
  `frozen` decimal(14,4) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_account`
--

LOCK TABLES `tb_account` WRITE;
/*!40000 ALTER TABLE `tb_account` DISABLE KEYS */;
/*!40000 ALTER TABLE `tb_account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_allserver`
--

DROP TABLE IF EXISTS `tb_allserver`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_allserver` (
  `srv_id` int(10) unsigned NOT NULL,
  `ip` varchar(16) NOT NULL,
  `port` int(10) unsigned NOT NULL,
  `gameid` int(10) unsigned NOT NULL,
  `logintype` tinyint(4) NOT NULL,
  `logins` int(10) unsigned NOT NULL,
  PRIMARY KEY (`srv_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_allserver`
--

LOCK TABLES `tb_allserver` WRITE;
/*!40000 ALTER TABLE `tb_allserver` DISABLE KEYS */;
INSERT INTO `tb_allserver` VALUES (1,'10.60.22.10',1234,1,6,1000),(2,'10.60.22.39',1234,2,6,1000);
/*!40000 ALTER TABLE `tb_allserver` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_battleinfo`
--

DROP TABLE IF EXISTS `tb_battleinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_battleinfo` (
  `autoid` int(12) unsigned NOT NULL AUTO_INCREMENT,
  `battleid` int(12) unsigned NOT NULL,
  `roomid` tinyint(3) unsigned NOT NULL,
  `tableid` tinyint(3) unsigned NOT NULL,
  `seatid` tinyint(3) unsigned NOT NULL,
  `status` tinyint(3) unsigned DEFAULT '0',
  `calls` tinyint(3) unsigned DEFAULT '0',
  `shows` tinyint(3) unsigned DEFAULT '0',
  `userid` int(12) unsigned DEFAULT '0',
  `userkey` int(12) unsigned DEFAULT '0',
  `agentkey` int(12) unsigned DEFAULT '0',
  `allcards` varchar(128) DEFAULT NULL,
  `lastcards` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`autoid`)
) ENGINE=InnoDB AUTO_INCREMENT=1073 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_battleinfo`
--

LOCK TABLES `tb_battleinfo` WRITE;
/*!40000 ALTER TABLE `tb_battleinfo` DISABLE KEYS */;
INSERT INTO `tb_battleinfo` VALUES (1046,1,1,2,0,1,0,0,888,6,32767,NULL,NULL),(1047,1,1,2,1,0,0,0,888,3,1,NULL,NULL),(1048,1,1,2,2,0,0,0,0,0,0,NULL,NULL),(1049,2,1,1,0,1,0,0,888,2,32767,NULL,NULL),(1050,2,1,1,1,0,0,0,0,0,0,NULL,NULL),(1051,2,1,1,2,0,0,0,0,0,0,NULL,NULL),(1052,3,1,2,0,0,0,0,0,0,0,NULL,NULL),(1053,3,1,2,1,0,0,0,0,0,0,NULL,NULL),(1054,3,1,2,2,0,0,0,0,0,0,NULL,NULL),(1055,4,1,2,0,0,0,0,0,0,0,NULL,NULL),(1056,4,1,2,1,0,0,0,0,0,0,NULL,NULL),(1057,4,1,2,2,0,0,0,0,0,0,NULL,NULL),(1058,5,1,2,0,0,0,0,0,0,0,NULL,NULL),(1059,5,1,2,1,0,0,0,0,0,0,NULL,NULL),(1060,5,1,2,2,0,0,0,0,0,0,NULL,NULL),(1061,6,1,2,0,0,0,0,0,0,0,NULL,NULL),(1062,6,1,2,1,0,0,0,0,0,0,NULL,NULL),(1063,6,1,2,2,0,0,0,0,0,0,NULL,NULL),(1064,7,1,2,0,0,0,0,0,0,0,NULL,NULL),(1065,7,1,2,1,0,0,0,0,0,0,NULL,NULL),(1066,7,1,2,2,0,0,0,0,0,0,NULL,NULL),(1067,8,1,2,0,0,0,0,0,0,0,NULL,NULL),(1068,8,1,2,1,0,0,0,0,0,0,NULL,NULL),(1069,8,1,2,2,0,0,0,0,0,0,NULL,NULL),(1070,0,1,2,0,0,0,0,0,0,0,NULL,NULL),(1071,0,1,2,1,0,0,0,0,0,0,NULL,NULL),(1072,0,1,2,2,0,0,0,0,0,0,NULL,NULL);
/*!40000 ALTER TABLE `tb_battleinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_dayrank`
--

DROP TABLE IF EXISTS `tb_dayrank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_dayrank` (
  `uid` int(10) unsigned NOT NULL,
  `gid` int(10) unsigned NOT NULL,
  `update` datetime DEFAULT NULL,
  `name` varchar(33) NOT NULL,
  `rate` tinyint(3) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_dayrank`
--

LOCK TABLES `tb_dayrank` WRITE;
/*!40000 ALTER TABLE `tb_dayrank` DISABLE KEYS */;
INSERT INTO `tb_dayrank` VALUES (555,6,'2015-10-09 17:03:40','kim',80),(666,6,'2015-10-09 17:04:45','james',79),(777,6,'2015-10-09 17:05:00','sam',78),(888,6,'2015-10-09 17:05:14','longsir',77),(588,6,'2015-10-09 17:25:14','kinji',75),(1024,6,'2015-12-15 11:44:01','nndk',71),(1025,6,'2015-12-15 11:44:11','nndk2ww',70),(1026,6,'2015-12-15 11:44:28','nfd2ww',69),(1028,6,'2015-12-15 11:44:41','df221dlw',63),(1029,6,'2015-12-15 11:45:15','fa221dlw',60);
/*!40000 ALTER TABLE `tb_dayrank` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_discard`
--

DROP TABLE IF EXISTS `tb_discard`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_discard` (
  `roomid` tinyint(3) unsigned NOT NULL,
  `tableid` tinyint(3) unsigned NOT NULL,
  `seatid` tinyint(3) unsigned NOT NULL,
  `userid` int(12) unsigned DEFAULT '0',
  `stime` tinyint(3) unsigned DEFAULT '0',
  `usercards` varchar(128) NOT NULL,
  `discards` varchar(128) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_discard`
--

LOCK TABLES `tb_discard` WRITE;
/*!40000 ALTER TABLE `tb_discard` DISABLE KEYS */;
/*!40000 ALTER TABLE `tb_discard` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_gametype`
--

DROP TABLE IF EXISTS `tb_gametype`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_gametype` (
  `gameid` int(10) unsigned NOT NULL,
  `gname` varchar(128) NOT NULL,
  `status` tinyint(4) NOT NULL,
  PRIMARY KEY (`gameid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_gametype`
--

LOCK TABLES `tb_gametype` WRITE;
/*!40000 ALTER TABLE `tb_gametype` DISABLE KEYS */;
/*!40000 ALTER TABLE `tb_gametype` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_log`
--

DROP TABLE IF EXISTS `tb_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_log` (
  `updatetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `rid` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL,
  `logtype` tinyint(4) NOT NULL,
  `errorno` varchar(128) NOT NULL,
  PRIMARY KEY (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_log`
--

LOCK TABLES `tb_log` WRITE;
/*!40000 ALTER TABLE `tb_log` DISABLE KEYS */;
/*!40000 ALTER TABLE `tb_log` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_prelogin`
--

DROP TABLE IF EXISTS `tb_prelogin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_prelogin` (
  `uid` int(10) unsigned NOT NULL,
  `status` tinyint(4) NOT NULL,
  `lasttime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `sshkey` varchar(33) NOT NULL,
  `srv_id` int(10) unsigned NOT NULL,
  `logintype` tinyint(4) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_prelogin`
--

LOCK TABLES `tb_prelogin` WRITE;
/*!40000 ALTER TABLE `tb_prelogin` DISABLE KEYS */;
INSERT INTO `tb_prelogin` VALUES (555,1,'2015-12-16 08:10:03','e85769039ba76677e236164e40279d5f',2,6),(777,1,'2015-10-19 06:01:09','ff700a3f473fb107caa99aa9e7a5b556',2,6),(888,1,'2015-12-15 03:29:51','c3e1195fdb34ac9b0c0d58e435c76e49',2,6);
/*!40000 ALTER TABLE `tb_prelogin` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_roominfo`
--

DROP TABLE IF EXISTS `tb_roominfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_roominfo` (
  `roomid` tinyint(3) unsigned NOT NULL,
  `tableid` tinyint(3) unsigned NOT NULL,
  `status` tinyint(3) unsigned DEFAULT '0',
  `tdouble` tinyint(1) unsigned DEFAULT '0',
  `money` int(12) unsigned DEFAULT '0',
  `remark` varchar(30) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_roominfo`
--

LOCK TABLES `tb_roominfo` WRITE;
/*!40000 ALTER TABLE `tb_roominfo` DISABLE KEYS */;
INSERT INTO `tb_roominfo` VALUES (1,1,1,1,200,NULL),(1,2,1,1,200,NULL),(2,1,1,1,200,NULL),(2,2,1,1,200,NULL),(3,1,1,1,200,NULL),(3,2,1,1,200,NULL),(4,1,1,1,200,NULL),(4,2,1,1,200,NULL),(5,1,1,1,200,NULL),(5,2,1,1,200,NULL);
/*!40000 ALTER TABLE `tb_roominfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_roomlist`
--

DROP TABLE IF EXISTS `tb_roomlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_roomlist` (
  `roomid` tinyint(3) unsigned NOT NULL,
  `name` varchar(33) NOT NULL,
  `status` tinyint(3) unsigned DEFAULT '0',
  `doubles` tinyint(1) unsigned DEFAULT '0',
  `mmax` int(12) unsigned DEFAULT '0',
  `mmin` int(12) unsigned DEFAULT '0',
  `remark` varchar(30) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_roomlist`
--

LOCK TABLES `tb_roomlist` WRITE;
/*!40000 ALTER TABLE `tb_roomlist` DISABLE KEYS */;
INSERT INTO `tb_roomlist` VALUES (1,'è¯•çŽ©åœº',1,1,0,0,'OK'),(2,'åˆçº§åœº',1,1,100,10,'OK'),(3,'ä¸­çº§åœº',1,1,1000,100,'OK'),(4,'é«˜çº§åœº',1,1,10000,1000,'OK'),(5,'æ¯”èµ›åœº',1,1,100000,10000,'OK');
/*!40000 ALTER TABLE `tb_roomlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_tablelist`
--

DROP TABLE IF EXISTS `tb_tablelist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_tablelist` (
  `roomid` tinyint(3) unsigned NOT NULL,
  `tableid` tinyint(3) unsigned NOT NULL,
  `name` varchar(33) NOT NULL,
  `status` tinyint(3) unsigned DEFAULT '0',
  `doubles` tinyint(1) unsigned DEFAULT '0',
  `onlines` tinyint(3) unsigned DEFAULT '0',
  `mmax` int(12) unsigned DEFAULT '0',
  `mmin` int(12) unsigned DEFAULT '0',
  `remark` varchar(30) DEFAULT NULL,
  `brkerage` int(12) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_tablelist`
--

LOCK TABLES `tb_tablelist` WRITE;
/*!40000 ALTER TABLE `tb_tablelist` DISABLE KEYS */;
INSERT INTO `tb_tablelist` VALUES (1,1,'è¯•çŽ©åœº-1',1,1,200,0,0,'OK',500),(2,1,'åˆçº§åœº-1',1,1,200,100,10,'OK',500),(3,1,'ä¸­çº§åœº-1',1,1,200,1000,100,'OK',500),(4,1,'é«˜çº§åœº-1',1,1,200,10000,1000,'OK',500),(5,1,'æ¯”èµ›åœº-1',1,1,200,100000,10000,'OK',500),(1,2,'è¯•çŽ©åœº-2',1,0,200,0,0,'OK',500);
/*!40000 ALTER TABLE `tb_tablelist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_user`
--

DROP TABLE IF EXISTS `tb_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_user` (
  `aid` int(10) unsigned NOT NULL,
  `uid` int(10) unsigned NOT NULL,
  `username` varchar(33) NOT NULL,
  `password` varchar(33) NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_user`
--

LOCK TABLES `tb_user` WRITE;
/*!40000 ALTER TABLE `tb_user` DISABLE KEYS */;
INSERT INTO `tb_user` VALUES (100,555,'kim','123qwe'),(101,666,'daniel','123qwe'),(100,777,'longsir','123qwe'),(102,888,'james','123qwe');
/*!40000 ALTER TABLE `tb_user` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_usergame`
--

DROP TABLE IF EXISTS `tb_usergame`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_usergame` (
  `aid` int(10) unsigned NOT NULL,
  `gameid` int(10) unsigned NOT NULL,
  `status` tinyint(4) NOT NULL,
  PRIMARY KEY (`aid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_usergame`
--

LOCK TABLES `tb_usergame` WRITE;
/*!40000 ALTER TABLE `tb_usergame` DISABLE KEYS */;
INSERT INTO `tb_usergame` VALUES (100,1,1),(101,2,0);
/*!40000 ALTER TABLE `tb_usergame` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_userinfo`
--

DROP TABLE IF EXISTS `tb_userinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_userinfo` (
  `uid` int(10) unsigned NOT NULL,
  `gameid` int(10) unsigned NOT NULL,
  `points` int(10) unsigned NOT NULL,
  `wons` int(10) unsigned NOT NULL,
  `faileds` int(10) unsigned NOT NULL,
  `aways` int(10) unsigned NOT NULL,
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_userinfo`
--

LOCK TABLES `tb_userinfo` WRITE;
/*!40000 ALTER TABLE `tb_userinfo` DISABLE KEYS */;
INSERT INTO `tb_userinfo` VALUES (555,6,0,110,90,0),(666,6,0,100,75,0),(888,6,0,0,0,0);
/*!40000 ALTER TABLE `tb_userinfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_userlog`
--

DROP TABLE IF EXISTS `tb_userlog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_userlog` (
  `uid` int(10) unsigned NOT NULL,
  `gid` int(10) unsigned NOT NULL,
  `btime` datetime DEFAULT NULL,
  `etime` datetime DEFAULT NULL,
  `points` int(12) DEFAULT '0',
  `wons` tinyint(1) unsigned DEFAULT '0',
  `faileds` tinyint(1) unsigned DEFAULT '0',
  `aways` tinyint(1) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_userlog`
--

LOCK TABLES `tb_userlog` WRITE;
/*!40000 ALTER TABLE `tb_userlog` DISABLE KEYS */;
/*!40000 ALTER TABLE `tb_userlog` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_userstatus`
--

DROP TABLE IF EXISTS `tb_userstatus`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_userstatus` (
  `uid` int(10) unsigned NOT NULL,
  `gid` int(10) unsigned NOT NULL,
  `btime` datetime DEFAULT NULL,
  `etime` datetime DEFAULT NULL,
  `ip` varchar(16) DEFAULT '0',
  `port` int(6) unsigned DEFAULT '0',
  `siteid` tinyint(3) unsigned DEFAULT '0',
  `tindex` tinyint(3) unsigned DEFAULT '0',
  `status` tinyint(1) unsigned DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_userstatus`
--

LOCK TABLES `tb_userstatus` WRITE;
/*!40000 ALTER TABLE `tb_userstatus` DISABLE KEYS */;
INSERT INTO `tb_userstatus` VALUES (555,6,'2015-10-09 14:51:58','2015-10-09 14:51:58','127.0.0.1',5600,1,1,1);
/*!40000 ALTER TABLE `tb_userstatus` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tb_weekrank`
--

DROP TABLE IF EXISTS `tb_weekrank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tb_weekrank` (
  `uid` int(10) unsigned NOT NULL,
  `gid` int(10) unsigned NOT NULL,
  `update` datetime DEFAULT NULL,
  `name` varchar(33) NOT NULL,
  `rate` tinyint(3) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tb_weekrank`
--

LOCK TABLES `tb_weekrank` WRITE;
/*!40000 ALTER TABLE `tb_weekrank` DISABLE KEYS */;
INSERT INTO `tb_weekrank` VALUES (555,6,'2015-10-09 17:03:14','kim',80),(888,6,'2015-10-09 17:07:07','longsir',77),(777,6,'2015-10-09 17:07:18','sam',76),(666,6,'2015-10-09 17:07:27','james',75),(1024,6,'2015-12-15 11:41:46','nndk',71),(1025,6,'2015-12-15 11:42:03','nndk2ww',70),(1026,6,'2015-12-15 11:42:17','nfd2ww',69),(1027,6,'2015-12-15 11:42:39','jkldlw',66),(1028,6,'2015-12-15 11:42:52','df221dlw',63),(1029,6,'2015-12-15 11:43:02','fa221dlw',60);
/*!40000 ALTER TABLE `tb_weekrank` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-17 10:20:49
