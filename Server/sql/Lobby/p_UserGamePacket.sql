delimiter //
drop procedure if exists `p_UserGamePacket` ;
create procedure `p_UserGamePacket` (IN in_UserID int, IN in_GameID int)
begin
        declare res_status  int default 0;
        declare res_points  int default 0;
        declare res_wons    int default 0;
        declare res_faileds int default 0;
        declare res_aways   int default 0;
	declare res_username varchar(33);
        select count(*) into res_status from `tb_userinfo` where `uid` = in_UserID and `gameid` = in_GameID;
        IF res_status = 0 THEN
                insert into `tb_userinfo` (`uid`,`gameid`,`points`,`wons`,`faileds`,`aways`) values(in_UserID,in_GameID,0,0,0,0);
        END IF;
        set res_status = 0;
        select `points`, `wons`, `faileds`, `aways` into res_points, res_wons, res_faileds, res_aways from `tb_userinfo` where `uid`=in_UserID and `gameid`=in_GameID;
	select `username` into res_username from `tb_user` where `uid`=in_UserID ;
        select res_status as `err`, res_username as username, res_points as `points`, res_wons as `wons`, res_faileds as `faileds`, res_aways as `aways`;
end //
delimiter ;

select * from `tb_tableinfo` ;