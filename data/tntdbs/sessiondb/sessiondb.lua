box.schema.user.passwd('admin', 'admin')
if not box.schema.user.exists('replicator') then 
	box.schema.user.create('replicator', {password = 'uangel.123'})
	box.schema.role.grant('replication','read,write','universe')
	box.schema.user.grant('replicator','execute','role','replication')
end

function sessiondb_init()
	dofile(tntdb_home .. '/' .. tntdb_name .. '.schema.lua')
end

box.once("sessiondb_init", sessiondb_init);
if tntdb_repltype ~= "SLAVE" and tntdb_replhost and tntdb_replhost ~= "" then
	box.cfg{ replication_source = tntdb_replhost}
end

local log = require('log')
local expirationd = require('expirationd')
local expire_time = 3600;
local SESSION_SPACE_ID = 1

if expirationd then
	print("sessiondb.expiretaion start");

	function tntdb_delete_tuple(space_id, args, tuple)
		box.space[space_id]:delete{tuple[1]}
	end
--[[
	expirationd.start("session_expired", SESSION_SPACE_ID,
		function( args, tuple)
			if tuple[8] and tuple[8]+expire_time < os.time() then
				log.info("session '%s' is expired", tuple[1])
				return true
			else 
				return false
			end
		end,
		{
			process_expired_tuple = tntdb_delete_tuple,
			args = nil,
			tuple_per_item = 50,
			full_scan_time = 3600,
			force = true
		}
	)
--]]
end

