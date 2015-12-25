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
-- Dumping routines for database 'db_passport'
--
/*!50003 DROP PROCEDURE IF EXISTS `p_DayRank` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_DayRank`()
PROC_LABEL: 
begin
declare res_out   int default 0;
declare res_index int default 0;
declare res_name  varchar(33);
declare res_rate  int default 0;
declare res_done  int default -1;
declare cur cursor for select `name`, `rate` from `tb_dayrank` 
ORDER BY `rate` DESC;
declare continue handler for not found set res_done = 1;
open cur;
CUR_LOOP : LOOP
fetch cur into res_name, res_rate;
if res_done=1 then
LEAVE CUR_LOOP;
end if; 
set res_index = res_index + 1;
select res_out as err, res_index as idx, res_name as name, res_rate as rate;
end LOOP CUR_LOOP;
close cur;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_GamePacket` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_GamePacket`(IN in_UserID int, IN in_GameID int)
begin
        declare res_status  int default 0;
        declare res_points  int default 0;
        declare res_wons    int default 0;
        declare res_faileds int default 0;
        declare res_aways   int default 0;
        select count(*) into res_status from `tb_userinfo` where `uid` = in_UserID and `gameid` = in_GameID;
        IF res_status = 0 THEN
                insert into `tb_userinfo` (`uid`,`gameid`,`points`,`wons`,`faileds`,`aways`) values(in_UserID,in_GameID,0,0,0,0);
        END IF;
        set res_status = 0;
        select `points`, `wons`, `faileds`, `aways` into res_points, res_wons, res_faileds, res_aways from `tb_userinfo` where `uid`=in_UserID and `gameid`=in_GameID;
        select res_status as `err`, res_points as `points`, res_wons as `wons`, res_faileds as `faileds`, res_aways as `aways`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_Login` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_Login`( IN in_UserID int, in in_UserKey varchar(33) )
PROC_SEG:
begin
        declare res_error int;
        declare res_status int;
        declare res_lasttime timestamp;
        declare res_sid int;
        declare res_gtype int;
        declare res_nowtime datetime;
        declare res_secdeff int;
        select `status`, `lasttime`, `srv_id` into res_status, res_lasttime, res_sid from `tb_prelogin` where `uid` = in_UserID and `sshkey` = in_UserKey;
        IF res_status <> 1 THEN
                set res_error = -1;
                select res_error as `error`;
                LEAVE PROC_SEG;
        END IF;
        set res_nowtime = from_unixtime( unix_timestamp(res_lasttime) );
        set res_secdeff = timestampdiff( second, res_nowtime, now() );
        IF res_secdeff > 3000 THEN
                set res_error = -2;
                select res_error as `error`;
                LEAVE PROC_SEG;
        END IF;
        set res_error  = 0;
        set res_gtype  = 6;
        select res_error as err, res_status as status, res_sid as sid, res_gtype as gtype;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_OnlinesInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_OnlinesInfo`()
PROC_LABEL: 
begin
declare res_out   int default 0;
declare res_count int default 0;
select count(*) into res_count from `tb_userstatus` where `status` = 1;
select res_out as `error`, res_count as `count`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_PreLogin` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_PreLogin`( IN in_username varchar(32), 
IN in_password varchar(32), 
IN in_asshkey varchar(33),
IN in_usshkey varchar(33),
IN in_gameid tinyint, 
IN in_logintype tinyint
)
PROC_LABEL: begin
declare res_aid int default 0;
declare res_uid int default 0;
declare res_srvid int default 0;
declare res_status tinyint default 0;
declare res_cnt int default 0;
declare res_sshkey varchar(33) default NULL;
declare res_ip varchar(16) default NULL;
declare res_port int default 0;
declare res_out int default 0;
select uid,aid into res_uid,res_aid from tb_user where username = in_username and password = in_password;
IF coalesce( res_uid, 0 ) = 0 then
SET res_out = -1;
select res_out as result;
LEAVE PROC_LABEL;
END IF;
select status,sshkey into res_status,res_sshkey from tb_access where aid = res_aid;
IF res_status <> 1 then
SET res_out = -2;
select res_out as result;
LEAVE PROC_LABEL;
END IF;
IF in_asshkey <> res_sshkey then
SET res_out = -3;
select res_out as result;
LEAVE PROC_LABEL;
END IF;
select status into res_status from tb_usergame where aid = res_aid;
IF coalesce( res_status, 0 ) = 0 then
SET res_out = -4;
select res_out as result;
LEAVE PROC_LABEL;
END IF;
select srv_id,ip,port into res_srvid,res_ip,res_port from tb_allserver where gameid = in_gameid and logintype = in_logintype;
IF coalesce( res_srvid, 0 ) = 0 then
SET res_out = -5;
select res_out as result;
LEAVE PROC_LABEL;
END IF;
select count(*) into res_cnt from tb_prelogin where uid = res_uid;
IF coalesce( res_cnt, 0 ) = 0 then
insert into tb_prelogin(uid, status, lasttime, sshkey, srv_id, logintype ) values(res_uid, 1, now(), in_usshkey, res_srvid, in_logintype);
ELSE
update tb_prelogin SET status = 1, lasttime = now(), sshkey = in_usshkey, srv_id = res_srvid, logintype = in_logintype where uid = res_uid;
END IF;
select res_out as err, res_uid as userid, res_port as port, res_ip as ip;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_RoomInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_RoomInfo`(IN in_siteid int)
PROC_LABEL: 
begin
declare res_out     int default 0;
declare res_site    int default 0;
declare res_status  int default 0;
declare res_index   int default 0;
declare res_tdouble int default 0;
declare res_onlines int default 0;
declare res_ulimit  int default 0;
declare done int default -1;
declare cur cursor for select `tindex`,`status`,`tdouble`,`umoney` 
from `tb_roominfo` where `siteid` = in_siteid;
declare continue handler for not found set done = 1;
open cur;
CUR_LOOP : LOOP
fetch cur into res_index,res_status,res_tdouble,res_ulimit;
if done=1 then
LEAVE CUR_LOOP;
end if; 
IF coalesce( res_index, 0 ) = 0 then
SET res_out = -1;
select res_out as result;
END IF;
IF res_status <> 1 then
SET res_out = -2;
select res_out as result;
LEAVE PROC_LABEL;
END IF;
set res_site = in_siteid;
set res_onlines = 0;
select res_out as err, res_site as siteid, res_index as tableid, res_tdouble as tdouble, res_onlines as onlines, res_ulimit as money;
end LOOP CUR_LOOP;
close cur;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_RoomListInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_RoomListInfo`()
PROC_LABEL: 
begin
declare res_out   int default 0;
declare res_room  int default 0;
declare res_size  int default 0;
declare res_name  varchar(33);
declare res_done  int default -1;
declare cur cursor for select `roomid`, `name` from `tb_roomlist`;
declare continue handler for not found set res_done = 1;
open cur;
CUR_LOOP : LOOP
fetch cur into res_room, res_name;
if res_done = 1 then
LEAVE CUR_LOOP;
end if; 
set res_size = 1;
select res_out as err, res_room as `room`, res_size as `size`, res_name as `name`;
end LOOP CUR_LOOP;
close cur;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_Settlement` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_Settlement`(
IN inBattleid int, 
IN inSeatid int,
IN inType int,
IN inBombs int)
PROC_LABEL: 
begin

declare res_index    int default 0;
declare res_userid0  int default 0;
declare res_userid1  int default 0;
declare res_userid2  int default 0;
declare res_banker   int default 0;
declare res_roomid   int default 0;
declare res_tableid  int default 0;


select `userid` into res_userid0
from `tb_battleinfo` where `battleid`=inBattleid and `seatid`=0 limit 1;
select `userid` into res_userid1
from `tb_battleinfo` where `battleid`=inBattleid and `seatid`=0 limit 1;
select `userid` into res_userid2
from `tb_battleinfo` where `battleid`=inBattleid and `seatid`=0 limit 1;

select `seatid` into res_banker
from `tb_battleinfo` where `battleid`=inBattleid and `banker`=1 limit 1;

select `roomid`, `tableid` into res_roomid, res_tableid
from `tb_battleinfo` where `battleid`=inBattleid limit 1;

if inType=0 Then
set res_index = 0;
ELSEIF inType=1 Then
set res_index = 1;
ELSEIF inType=2 Then
set res_index = 2;
end if;

set res_index = 0;
select res_index as `error`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_TableListInfo` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_TableListInfo`()
PROC_LABEL: 
begin
declare res_out     int default 0;
declare res_room    int default 0;
declare res_table   int default 0;
declare res_name    varchar(33);
declare res_double  int default 0;
declare res_onlines int default 0;
declare res_mmin    int default 0;

declare res_done  int default -1;
declare cur cursor for select `roomid`, `tableid`, `name`, `doubles`, `onlines`, `mmin`
from `tb_tablelist` where `status` = 1;
declare continue handler for not found set res_done = 1;
open cur;
CUR_LOOP : LOOP
fetch cur into res_room, res_table, res_name, res_double, res_onlines, res_mmin;
if res_done = 1 then
LEAVE CUR_LOOP;
end if; 
select res_out     as  err, 
   res_room    as `roomid`, 
   res_table   as `tableid`, 
   res_name    as `name`,
   res_double  as `doubles`, 
   res_onlines as `onlines`, 
   res_mmin    as `mmin`;
end LOOP CUR_LOOP;
close cur;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UpdateBattle` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UpdateBattle`(
IN inBattleid  int,
IN inSeatid    int,
IN inBanker    int,
IN inShowcard  int,
IN inCalled    int,
IN inUsercard  varchar(128),
IN inDiscards  varchar(128))
PROC_LABEL: 
begin
declare res_index int default 0;

select `autoid` into res_index
from `tb_battleinfo` 
where `battleid`=inBattleid 
  and `seatid`=inSeatid
limit 1;

IF res_index=0 THEN
set res_index = -1;
select res_index as `error`;
LEAVE PROC_LABEL;
END IF;

update `tb_battleinfo` 
set `banker`    = inBanker,
`shows`     = inShowcard,
`calls`     = inCalled,
`allcards`   = inUsercard,
`lastcards` = inDiscards
where `autoid` = res_index limit 1;

set res_index = 0;
select res_index as `error`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserAllcards` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserAllcards`( IN in_UserID int)
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_index  int default 0;
declare res_allcards    varchar(128);
declare res_status int default 0;


select tid, allcards into res_index, res_allcards   
from `tb_tableinfo` where `userid` = in_UserID limit 1;
IF res_index = 0 THEN
set res_out = -1;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;

select res_out as err, in_UserID as userid, res_allcards as allcards;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserAlloccards` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserAlloccards`( IN in_UserID int, IN in_Newccards varchar(128) )
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_index  int default 0;
declare res_status int default 0;


select `tid` into res_index
from `tb_tableinfo` where `userid` = in_UserID limit 1;
IF res_index = 0 THEN
set res_out = -1;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;


update `tb_tableinfo` set `allcards` = in_Newccards
where `userid` = in_UserID limit 1;


set res_index  = 0;
set res_status = 0;
select res_out as err, in_UserID as userid, res_status as status;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserDiscards` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserDiscards`( IN in_UserID int, IN in_Discards varchar(128) )
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_index  int default 0;
declare res_discards    varchar(128);
declare res_newcards    varchar(128);
declare res_status int default 0;


select tid, discards into res_index, res_discards   
from `tb_tableinfo` where `userid` = in_UserID limit 1;
IF res_index = 0 THEN
set res_out = -1;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;

IF LENGTH(res_discards) <> 0 THEN
select concat(res_discards, ';', in_Discards) into res_newcards;
ELSE
select concat(in_Discards) into res_newcards;
END IF;

select res_newcards as discards;


update `tb_tableinfo` set `discards` = res_newcards
where `userid` = in_UserID limit 1;


set res_index  = 0;
set res_status = 1;
select res_out as err, in_UserID as userid, res_status as status;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserGamePacket` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserGamePacket`(IN in_UserID int, IN in_GameID int)
begin
        declare res_status  int default 0;
        declare res_points  int default 0;
        declare res_wons    int default 0;
        declare res_faileds int default 0;
        declare res_aways   int default 0;
declare res_username varchar(33);
        select count(*) into res_status from `tb_userinfo` where `uid` = in_UserID and `gameid` = in_GameID;
        IF res_status = 0 THEN
                insert into `tb_userinfo` (`uid`,`gameid`,`points`,`wons`,`faileds`,`aways`) values(in_UserID,in_GameID,0,0,0,0);
        END IF;
        set res_status = 0;
        select `points`, `wons`, `faileds`, `aways` into res_points, res_wons, res_faileds, res_aways from `tb_userinfo` where `uid`=in_UserID and `gameid`=in_GameID;
select `username` into res_username from `tb_user` where `uid`=in_UserID ;
        select res_status as `err`, res_username as username, res_points as `points`, res_wons as `wons`, res_faileds as `faileds`, res_aways as `aways`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserJoinGame` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserJoinGame`(
IN in_UserID  int,
IN in_UserKey int,
IN in_AgentKey int,
IN in_RoomID  int,
IN in_TableID int,
IN in_SeatID  int,
IN in_BattleID int)
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_index  int default 0;


select autoid into res_out from `tb_battleinfo`
where `battleid` = in_BattleID limit 1;

IF res_out = 0 THEN
insert into `tb_battleinfo`(`battleid`,`roomid`,`tableid`,`seatid`)  
values(in_BattleID, in_RoomID, in_TableID, 0);
insert into `tb_battleinfo`(`battleid`,`roomid`,`tableid`, `seatid`)  
values(in_BattleID, in_RoomID, in_TableID, 1);
insert into `tb_battleinfo`(`battleid`,`roomid`,`tableid`, `seatid`)  
values(in_BattleID, in_RoomID, in_TableID, 2);
END IF;

update `tb_battleinfo` set 
`userid` = in_UserID,
`userkey` = in_UserKey,
`agentkey` = in_AgentKey,
`status` = 1
where `roomid`=in_RoomID and `tableid`=in_TableID and `seatid`=in_SeatID limit 1;

select autoid into res_out from `tb_battleinfo`
where `userid` = in_UserID limit 1;

IF res_out = 0 THEN 
set res_index = -1;
select res_index as `error`, res_index as autoid;
LEAVE PROC_LABEL;
END if;

set res_index = res_out;
set res_out = 0;
select res_out as `error`, res_index as autoid;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserJoinTable` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserJoinTable`( 
IN inUserID   int, 
IN inUserKey  int, 
IN inAgentKey int, 
IN inRoomID   int, 
IN inTableID  int )
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_index  int default 0;
declare res_room   int default 0;
declare res_table  int default 0;
declare res_seat   int default 0;


select `autoid`, `seatid` into res_index, res_seat from `tb_battleinfo` 
where `userid` = inUserID and `status` = 0 limit 1;
IF res_index <> 0 THEN
set res_out = -1;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;





select `autoid`, `seatid` into res_index, res_seat from `tb_battleinfo` 
where `roomid` = inRoomID and `tableid` = inTableID and `status` = 0 limit 1;
IF res_index = 0 THEN
set res_out = -2;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;


update `tb_battleinfo` 
set `userid`=inUserID,`seatid`=1,`userkey`=inUserKey,`agentkey`=inAgentKey 
where `autoid`=res_index;


set res_out = 0;
select res_out as `error`, res_index as `index`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserLogin` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserLogin`( IN in_UserID int, in in_UserKey varchar(33) )
PROC_SEG:
begin
        declare res_error int;
        declare res_status int;
        declare res_lasttime timestamp;
        declare res_sid int;
        declare res_gtype int;
        declare res_nowtime datetime;
        declare res_secdeff int;
        select `status`, `lasttime`, `srv_id` into res_status, res_lasttime, res_sid from `tb_prelogin` where `uid` = in_UserID and `sshkey` = in_UserKey;
        IF res_status <> 1 THEN
                set res_error = -1;
                select res_error as `error`;
                LEAVE PROC_SEG;
        END IF;
        

        set res_error  = 0;
        set res_gtype  = 6;

        select res_error as err, res_status as status, res_sid as sid, res_gtype as gtype;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserQuitTable` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserQuitTable`( IN inUserid int )
PROC_LABEL: 
begin
declare res_index  int default 0;
declare res_roomid   int;
declare res_tableid  int;
declare res_battleid int;
declare res_seatid   int;

select `autoid`, `roomid`, `tableid`, `battleid`, `seatid` 
into res_index, res_roomid, res_tableid, res_battleid, res_seatid 
from `tb_battleinfo` 
where `userid` = inUserid
limit 1;

IF res_index=0 THEN
set res_index = -1;
select res_index as `error`;
LEAVE PROC_LABEL;
END IF;


delete from `tb_battleinfo` where `autoid` = res_index;


insert into `tb_battleinfo`(`roomid`, `tableid`, `battleid`, `seatid`)
values(res_roomid, res_tableid, res_battleid, res_seatid);


set res_index = 0;
select res_index as `error`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserRelogin` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserRelogin`( IN inUserid int, IN inUserkey int)
PROC_LABEL: 
begin
declare res_index  int default 0;
declare res_battleid int;
declare res_seatid   int;
select `autoid`, `battleid`, `seatid` into res_index, res_battleid, res_seatid 
from `tb_battleinfo` 
where `userid` = inUserid
limit 1;

IF res_index=0 THEN
set res_index = -1;
select res_index as `error`;
LEAVE PROC_LABEL;
END IF;

update `tb_battleinfo` set `userkey`  = inUserKey
where `userid` = inUserid limit 1;

set res_index = 0;
select res_index as `error`, res_battleid as `battleid`, res_seatid as `seatid`;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserShowcards` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserShowcards`(IN in_UserID int)
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_count  int default 0;
declare res_index  int default 0;
declare res_room   int default 0;
declare res_table  int default 0;
declare res_seat   int default 0;
declare res_shows  int default 0;
declare res_status int default 0;


select `tid`, `roomid`, `typeid`, `seatid`, `shows`
into res_index, res_room, res_table, res_seat, res_shows 
from `tb_tableinfo` where `userid` = in_UserID limit 1;
IF res_index = 0 THEN
set res_out = -1;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;


IF res_shows <> 0 THEN
set res_out = -2;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;

update `tb_tableinfo` set `shows` = 1 where `userid` = in_UserID limit 1;


set res_index  = 0;
set res_status = 1;
select res_out as err, in_UserID as userid, res_status as status;

end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_UserStartGame` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_UserStartGame`(IN in_UserID int)
PROC_LABEL: 
begin
declare res_out    int default 0;
declare res_count  int default 0;
declare res_index  int default 0;
declare res_room   int default 0;
declare res_table  int default 0;
declare res_seat   int default 0;
declare res_status int default 0;


select `tid`, `roomid`, `typeid`, `seatid`
into res_index, res_room, res_table, res_seat 
from `tb_tableinfo` where `userid` = in_UserID limit 1;
IF res_index = 0 THEN
set res_out = -1;
select res_out as `error`;
LEAVE PROC_LABEL;
END IF;


update `tb_tableinfo` set `status` = 2 where `userid` = in_UserID limit 1;


select count(*) into res_count from `tb_tableinfo` 
where `roomid` = roomid and `typeid` = typeid and `status` = 2 limit 1;
IF res_count <> 3 THEN
set res_out = 0;
set res_status = 0;
select res_out as err, in_UserID as userid, res_status as status;
LEAVE PROC_LABEL;
END IF;


set res_index  = 0;
set res_status = 1;
select res_out as err, in_UserID as userid, res_status as status;

end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `p_WeekRank` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = latin1 */ ;
/*!50003 SET character_set_results = latin1 */ ;
/*!50003 SET collation_connection  = latin1_swedish_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50020 DEFINER=`root`@`localhost`*/ /*!50003 PROCEDURE `p_WeekRank`()
PROC_LABEL: 
begin
declare res_out   int default 0;
declare res_index int default 0;
declare res_name  varchar(33);
declare res_rate  int default 0;
declare res_done  int default -1;
declare cur cursor for select `name`, `rate` from `tb_weekrank` 
ORDER BY `rate` DESC;
declare continue handler for not found set res_done = 1;
open cur;
CUR_LOOP : LOOP
fetch cur into res_name, res_rate;
if res_done=1 then
LEAVE CUR_LOOP;
end if; 
set res_index = res_index + 1;
select res_out as err, res_index as idx, res_name as name, res_rate as rate;
end LOOP CUR_LOOP;
close cur;
end */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-17 10:24:14
