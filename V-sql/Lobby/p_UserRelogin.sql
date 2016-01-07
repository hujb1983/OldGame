DELIMITER //
drop procedure if exists p_UserRelogin;
CREATE PROCEDURE `p_UserRelogin`( IN inUserid int )
PROC_LABEL: 
begin
	declare res_index  int default 0;
	declare res_battleid int;
	declare res_filedid int;
	declare res_roomid int;
	declare res_tableid int;
	declare res_seatid int;
	declare res_userkey int;
	
	select `battleid`, `userkey`, `filedid`, `roomid`, `tableid`, `seatid` into 
	res_battleid, res_userkey, res_filedid, res_roomid, res_tableid, res_seatid 
	from `tb_battleinfo` where `userid` = inUserid limit 1;
	
	set res_index = res_battleid;
	IF res_index=0 THEN
		set res_index = -1;
		select res_index as `error`;
		LEAVE PROC_LABEL;
	END IF;
	
	set res_index = 0;
	select  res_index as `error`, 
		res_battleid as `battleid`
		res_filedid as `filedid`;
		res_roomid as `roomid`,
		res_tableid as `tableid`,
		res_seatid as `seatid`;
		res_userkey as `userkey`,
end //
DELIMITER ;

-- call p_UserRelogin(555, 20);

-- select * from `tb_battleinfo`;