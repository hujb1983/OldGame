delimiter ;;
drop procedure if exists `p_UserLogin`;
create procedure `p_UserLogin`( IN in_UserID int, in in_UserKey varchar(33) )
PROC_SEG:
begin
        declare res_error int;
        declare res_status int;
        declare res_lasttime timestamp;
        declare res_sid int;
        declare res_gtype int;
        declare res_nowtime datetime;
        declare res_secdeff int;
        select `status`, `lasttime`, `srv_id` into res_status, res_lasttime, res_sid 
		from `tb_prelogin` where `uid` = in_UserID and `sshkey` = in_UserKey;
        IF res_status <> 1 THEN
                set res_error = -1;
                select res_error as `error`;
                LEAVE PROC_SEG;
        END IF;
        /* set res_nowtime = from_unixtime( unix_timestamp(res_lasttime) );
        set res_secdeff = timestampdiff( second, res_nowtime, now() );
        IF res_secdeff > 30000 THEN
                set res_error = -2;
                select res_error as `error`;
                LEAVE PROC_SEG;
        END IF; */

        set res_error  = 0;
        set res_gtype  = 6;
	-- update `tb_prelogin` set `status` = 2 where `uid` = in_UserID;
        select res_error as err, res_status as status, res_sid as sid, res_gtype as gtype;
end ;;
delimiter ;