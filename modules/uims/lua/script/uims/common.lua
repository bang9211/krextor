require "uxlib"
require "uxcutor"
require "upasip"
require "usip"

--[[ example)

local sess = uxc_sdmvars.get_sess(sdmvars);
local sndmsg = sess:get_sndmsg()
local pasipmsg = uxc_msg.get_msg(sndmsg)
local sipmsg = upa_sipmsg.get_stackmsg(pasipmsg)

local hdr = sipmsg:get_hdr('Path')
hdr.uri.user = term_ind
hdr.uri.host = ip_adress

local rv = sipmsg:set_hdrstr('Path', 0, some_str)

--]]
