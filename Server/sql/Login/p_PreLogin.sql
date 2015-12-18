DELIMITER ;;
CREATE PROCEDURE `p_PreLogin`( IN in_username varchar(32), 
	IN in_password varchar(32), 
	IN in_asshkey varchar(33),
	IN in_usshkey varchar(33),
	IN in_gameid tinyint, 
	IN in_logintype tinyint
	)
PROC_LABEL: 
begin
	declare res_aid int default 0;
	declare res_uid int default 0;
	declare res_srvid int default 0;
	declare res_status tinyint default 0;
	declare res_cnt int default 0;
	declare res_sshkey varchar(33) default NULL;
	declare res_ip varchar(16) default NULL;
	declare res_port int default 0;
	declare res_out int default 0;
	select uid,aid into res_uid,res_aid from tb_user where username = in_username and password = in_password;
	IF coalesce( res_uid, 0 ) = 0 then
		SET res_out = -1;
		select res_out as result;
		LEAVE PROC_LABEL;
	END IF;
	select status,sshkey into res_status,res_sshkey from tb_access where aid = res_aid;
	IF res_status <> 1 then
		SET res_out = -2;
		select res_out as result;
		LEAVE PROC_LABEL;
	END IF;
	IF in_asshkey <> res_sshkey then
		SET res_out = -3;
		select res_out as result;
		LEAVE PROC_LABEL;
	END IF;
	select status into res_status from tb_usergame where aid = res_aid;
	IF coalesce( res_status, 0 ) = 0 then
		SET res_out = -4;
		select res_out as result;
		LEAVE PROC_LABEL;
	END IF;
	select srv_id,ip,port into res_srvid,res_ip,res_port from tb_allserver where gameid = in_gameid and logintype = in_logintype;
	IF coalesce( res_srvid, 0 ) = 0 then
		SET res_out = -5;
		select res_out as result;
		LEAVE PROC_LABEL;
	END IF;
	select count(*) into res_cnt from tb_prelogin where uid = res_uid;
	IF coalesce( res_cnt, 0 ) = 0 then
		insert into tb_prelogin(uid, status, lasttime, sshkey, srv_id, logintype ) values(res_uid, 1, now(), in_usshkey, res_srvid, in_logintype);
	ELSE
		update tb_prelogin SET status = 1, lasttime = now(), sshkey = in_usshkey, srv_id = res_srvid, logintype = in_logintype where uid = res_uid;
	END IF;
	select res_out as err, res_uid as userid, res_port as port, res_ip as ip;
end ;;
DELIMITER ;
