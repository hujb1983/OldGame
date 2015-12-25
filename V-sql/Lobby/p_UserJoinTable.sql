DELIMITER //
drop procedure if exists p_UserJoinTable;
CREATE PROCEDURE `p_UserJoinTable`( 
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
	declare res_battle int default 0;
	declare res_seat   int default 0;
	
	-- 判断是否已经在游戏中；
	select `autoid`, `seatid` into res_index, res_seat from `tb_battleinfo` 
	where `userid` = inUserID limit 1;
	IF res_index <> 0 THEN
		set res_out = -1;
		select res_out as `error`;
		LEAVE PROC_LABEL;
	END IF;
	
	-- 判断指定的游戏复本；
	-- status=0; 空位置;
	-- status=1; 已占位;
	-- status=2; 在游戏;
	select `autoid`, `battleid`, `seatid` into res_index, res_battle, res_seat from `tb_battleinfo` 
	where `roomid` = inRoomID and `tableid` = inTableID and `status` = 0 limit 1;
	IF res_index = 0 THEN
		set res_out = -2;
		select res_out as `error`;
		LEAVE PROC_LABEL;
	END IF;
	
	-- 如果查找到座位就直接写入坐位；
	update `tb_battleinfo` set 
	`userid`=inUserID,
	`status`=1,
	`userkey`=inUserKey,
	`agentkey`=inAgentKey 
	where `autoid`=res_index;
	
	-- 返回座位的id;
	set res_out = 0;
	select res_out as `error`, res_battle as `battle`, res_seat as battle;
end //
DELIMITER ;

call p_UserJoinTable(555, 123, 123, 1, 1);
select * from `tb_tableinfo` ;