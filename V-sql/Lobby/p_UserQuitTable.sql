DELIMITER //
drop procedure if exists p_UserQuitTable;
CREATE PROCEDURE `p_UserQuitTable`( IN inUserid int )
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
	
	-- 删除这一行
	delete from `tb_battleinfo` where `autoid` = res_index;
	
	-- 然后重新插入新记录
	insert into `tb_battleinfo`(`roomid`, `tableid`, `battleid`, `seatid`)
	values(res_roomid, res_tableid, res_battleid, res_seatid);
	
	-- 然后坐位就没有人坐了
	set res_index = 0;
	select res_index as `error`;
end //
DELIMITER ;

-- call p_UserQuitTable(555);

-- select * from `tb_battleinfo`;