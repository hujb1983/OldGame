DELIMITER //
drop procedure if exists p_OnlinesInfo;
CREATE PROCEDURE `p_OnlinesInfo`()
PROC_LABEL: 
begin
	declare res_out   int default 0;
	declare res_count int default 0;
	select count(*) into res_count from `tb_userstatus` where `status` = 1;
	select res_out as `error`, res_count as `count`;
end //
DELIMITER ;
