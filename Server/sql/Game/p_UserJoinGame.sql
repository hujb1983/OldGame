DELIMITER //
drop procedure if exists p_UserJoinGame;
CREATE PROCEDURE `p_UserJoinGame`(
	IN in_UserID  int,
	IN in_UserKey int,
	IN in_AgentKey int,
	IN in_RoomID  int,
	IN in_TableID int,
	IN in_SeatID  int,
	IN in_BattleID int)
PROC_LABEL: 
begin
	declare res_error  int default 0;
	declare res_index  int default -1;
	declare res_MaxMoney   int default  -1;
	declare res_MinMoney   int default  -1;
	declare res_Brokerage  int default  -1;

	-- 查找用户是否已经在了；
	select autoid into res_index from `tb_battleinfo`
	where `battleid` = in_BattleID limit 1;
	
	IF res_index = -1 THEN
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
		
	set res_index = -1;
	select autoid into res_index from `tb_battleinfo`
	where `userid` = in_UserID limit 1;
	
	IF res_index = -1 THEN 
		set res_error = -1;
		select res_error as `error`;
		LEAVE PROC_LABEL;
	END if;
	
	select `mmax`, `mmin`, `brokerage` into 
	res_MaxMoney,  res_MinMoney, res_Brokerage from `tb_tablelist` 
	where `roomid` = in_RoomID and `tableid` = in_TableID limit 1;
	
	set res_error = 0;
	select res_error as `error`, 
		res_index as autoid,
		res_MaxMoney  as maxmoney,
		res_MinMoney  as minmoney,
		res_Brokerage as brokerage;
end //
DELIMITER ;

call p_UserJoinGame(555, 1251, 512, 1, 1, 0, 1 );
select * from `tb_battleinfo` ;