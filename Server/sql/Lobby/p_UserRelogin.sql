DELIMITER //
drop procedure if exists p_UserRelogin;
CREATE PROCEDURE `p_UserRelogin`( IN inUserid int, IN inUserkey int)
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
end //
DELIMITER ;

-- call p_UserRelogin(555, 20);

-- select * from `tb_battleinfo`;