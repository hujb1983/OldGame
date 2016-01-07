DELIMITER //
drop procedure if exists p_UserJoinTable;
CREATE PROCEDURE `p_UserJoinTable`( IN inUserID   int, IN inUserKey  int, IN inAgentKey int, IN inFieldId int, IN inRoomID  int )
PROC_LABEL: 
begin
	declare res_index int default 0;
	declare res_battleid int default 0;
	declare res_tableid int default 0;
	declare res_seatid int default 0;
	
	-- 判断指定的游戏复本；
	-- status=0; 空位置;
	-- status=1; 已占位;
	-- status=2; 在游戏;
	select `battleid`, `tableid`, `seatid` into res_battleid, res_tableid, res_seatid from `tb_battleinfo` 
	where `fieldid` = inFieldId and `roomid` = inRoomID  and `status` = 0 limit 1;
	
	set res_index = res_battleid;
	IF res_index = 0 THEN
		set res_index = -2;
		select res_index as `error`;
		LEAVE PROC_LABEL;
	END IF;
	
	-- 如果查找到座位就直接写入坐位；
	update `tb_battleinfo` set `userid`=inUserID, `status`=1, `userkey`=inUserKey, `agentkey`=inAgentKey 
	where `battleid`=res_index;
	
	-- 返回座位的id;
	set res_index = 0;
	select res_index as `error`, 
	res_battleid as `battleid`, 
	res_tableid as `tableid`, 
	res_seatid as `seatid`;
end //
DELIMITER ;

call p_UserJoinTable(555, 123, 123, 1, 1);
select * from `tb_tableinfo` ;