DELIMITER //
drop procedure if exists `p_TableListInfo`;
CREATE PROCEDURE `p_TableListInfo`()
PROC_LABEL: 
begin
	declare res_out     int default 0;
	declare res_room    int default 0;
	declare res_table   int default 0;
	declare res_name    varchar(33);
	declare res_double  int default 0;
	declare res_onlines int default 0;
	declare res_mmin    int default 0;
	
	declare res_done  int default -1;
	declare cur cursor for select `roomid`, `tableid`, `name`, `doubles`, `onlines`, `mmin`
	from `tb_tablelist` where `status` = 1;
	declare continue handler for not found set res_done = 1;
	open cur;
	CUR_LOOP : LOOP
		fetch cur into res_room, res_table, res_name, res_double, res_onlines, res_mmin;
		if res_done = 1 then
			LEAVE CUR_LOOP;
		end if; 	
		select res_out     as  err, 
			   res_room    as `roomid`, 
			   res_table   as `tableid`, 
			   res_name    as `name`,
			   res_double  as `doubles`, 
			   res_onlines as `onlines`, 
			   res_mmin    as `mmin`;
	end LOOP CUR_LOOP;
	close cur;
end //
DELIMITER ;

call p_TableListInfo();