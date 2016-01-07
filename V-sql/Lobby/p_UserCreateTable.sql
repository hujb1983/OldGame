DELIMITER //
drop procedure if exists p_UserCreateTable;
CREATE PROCEDURE `p_UserCreateTable`( IN in_UserID  int, IN in_UserKey int, IN in_AgentKey int, 
									IN in_FieldId  int, IN in_RoomID int, IN in_TableID int)
PROC_LABEL: 
begin
	declare res_error  int default 0;
	declare res_index  int default -1;
	declare res_seatid int default 0;

	insert into `tb_battleinfo`(`fieldid`, `roomid`,`tableid`,`seatid`,`userid`, `userkey`, `agentkey`, `status`)  
	values	(in_FieldId, in_RoomID, in_TableID, 0, in_UserID, in_UserKey, in_AgentKey, 1), 
			(in_FieldId, in_RoomID, in_TableID, 1, 0, 0, 0, 0 ),
			(in_FieldId, in_RoomID, in_TableID, 2, 0, 0, 0, 0);
	
	set res_index = -1;
	select `battleid` into res_index from `tb_battleinfo` where `userid` = in_UserID limit 1;
	
	IF res_index = -1 THEN 
		set res_error = -1;
		select res_error as `error`;
		LEAVE PROC_LABEL;
	END if;
	
	set res_error = 0;
	select res_error as `error`, res_index as `battleid`, in_TableID as `tableid`, res_seatid as `seatid`;
end //
DELIMITER ;

call p_UserCreateTable(555, 2, 1, 1, 1, 1 );
select * from `tb_battleinfo` ;