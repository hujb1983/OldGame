DELIMITER //
drop procedure if exists p_UpdateBattle;
CREATE PROCEDURE `p_UpdateBattle`(
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
end //
DELIMITER ;

-- call p_UpdateBattle();
-- select * from `tb_battleinfo` ;