
DELIMITER //
drop procedure if exists p_Settlement;
CREATE PROCEDURE `p_Settlement`(
	IN inBattleid int, 
	IN inSeatid   int,
	IN inType     int,
	IN inScore    int,
	IN inBombs    int )
PROC_LABEL: 
begin
	declare res_index    int default 0;
	declare res_userid0  int default 0;
	declare res_userid1  int default 0;
	declare res_userid2  int default 0;
	declare res_banker   int default 0;
	declare res_roomid   int default 0;
	declare res_tableid  int default 0;
	
	-- 先找出 battleid 的 DB
	select `userid` into res_userid0
	from `tb_battleinfo` where `battleid`=inBattleid and `seatid`=0 limit 1;
	select `userid` into res_userid1
	from `tb_battleinfo` where `battleid`=inBattleid and `seatid`=1 limit 1;
	select `userid` into res_userid2
	from `tb_battleinfo` where `battleid`=inBattleid and `seatid`=2 limit 1;
	
	-- 查找地主位置
	select `seatid` into res_banker
	from `tb_battleinfo` where `battleid`=inBattleid and `banker`=1 limit 1;
	
	-- 查找房间
	select `roomid`, `tableid` into res_roomid, res_tableid
	from `tb_battleinfo` where `battleid`=inBattleid limit 1;
	
	if inType=0 Then		-- 积分场
		set res_index = 0;
	ELSEIF inType=1 Then	-- 翻倍场
		set res_index = 1;
	ELSEIF inType=2 Then	-- 比赛场
		set res_index = 2;
	end if;
	
	set res_index = 0;
	select res_index as `error`;
end //
DELIMITER ;

-- call p_Settlement(1, 0, 1, 5);
-- select * from `tb_discard` ;