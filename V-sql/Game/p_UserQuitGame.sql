DELIMITER //
drop procedure if exists `p_UserQuitGame`;
CREATE PROCEDURE `p_UserQuitGame`( IN inUserid int )
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
	
	-- 更新这一行;
	update `tb_battleinfo` set 
	`status`=0, `calls`=0, `userid`=0,  `userkey`=0, `agentkey`=0, 
	`allcards`=NULL, `lastcards`=NULL
	where `autoid` = res_index;


	-- 返回复本和坐位;
	set res_index = 0;
	select res_index as `error`, res_battleid as battleid, res_seatid as seatid;
end //
DELIMITER ;

-- call p_UserQuitGame(555);

-- select * from `tb_battleinfo`;