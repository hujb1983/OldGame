DELIMITER //
drop procedure if exists `p_RoomListInfo`;
CREATE PROCEDURE `p_RoomListInfo`()
PROC_LABEL: 
begin
	declare res_out   int default 0;
	declare res_room  int default 0;
	declare res_size  int default 0;
	declare res_name  varchar(33);
	declare res_done  int default -1;
	declare cur cursor for select `roomid`, `name` from `tb_roomlist`;
	declare continue handler for not found set res_done = 1;
	open cur;
	CUR_LOOP : LOOP
		fetch cur into res_room, res_name;
		if res_done = 1 then
			LEAVE CUR_LOOP;
		end if; 	
		set res_size = 1;
		select res_out as err, res_room as `room`, res_size as `size`, res_name as `name`;
	end LOOP CUR_LOOP;
	close cur;
end //
DELIMITER ;

call p_RoomListInfo();