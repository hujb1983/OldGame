DELIMITER //
drop procedure if exists p_WeekRank;
CREATE PROCEDURE `p_WeekRank`()
PROC_LABEL: 
begin
	declare res_out   int default 0;
	declare res_index int default 0;
	declare res_name  varchar(33);
	declare res_rate  int default 0;
	declare res_done  int default -1;
	declare cur cursor for select `name`, `rate` from `tb_weekrank` 
	ORDER BY `rate` DESC;
	declare continue handler for not found set res_done = 1;
	open cur;
	CUR_LOOP : LOOP
		fetch cur into res_name, res_rate;
		if res_done=1 then
			LEAVE CUR_LOOP;
		end if; 	
		set res_index = res_index + 1;
		select res_out as err, res_index as idx, res_name as name, res_rate as rate;
	end LOOP CUR_LOOP;
	close cur;	
end //
DELIMITER ;
