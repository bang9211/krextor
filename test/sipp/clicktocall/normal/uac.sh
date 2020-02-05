#!/bin/sh

#host=`cat ../config/uas_host`
#port=`cat ../config/uas_port`
#peer_host=`cat ../config/scpas_host`
#peer_port=`cat ../config/scpas_port`

service=invite
host=`uname -n`
ipaddr=`cat /etc/hosts | grep $host | awk '{ print $1 }' `
pxyport=`cat ../config/agent.csv | grep -v 'SEQ' | grep -v '#' | awk '{ split($0,arr,";"); printf("%s",arr[1]); }'`
uacport=`cat ../config/agent.csv | grep -v 'SEQ' | grep -v '#' | awk '{ split($0,arr,";"); printf("%s",arr[2]); }'`
uasport=`cat ../config/agent.csv | grep -v 'SEQ' | grep -v '#' | awk '{ split($0,arr,";"); printf("%s",arr[3]); }'`
pxyaddr=$ipaddr:$pxyport
uacaddr=$ipaddr:$uacport
uasaddr=$ipaddr:$uasport

cmd="sipp $pxyaddr -p $uacport -sf uac.xml -inf ../config/agent.csv -inf ../config/caller.csv -inf ../config/callee.csv -s $service -r 1 -l 1 -m 1 -rsa $pxyaddr $uacaddr"

simctrl="-nostdin -trace_msg -message_file ../report/01_invite/uac.res.tmp"
simctrl2="-nostdin -trace_msg -message_file"
args=""

while [ $# -gt 0 ]
do
    if [ $1 == "simctrl" ]
    then
        args="${args} ${simctrl}"
    elif [ $1 == "simctrl2" ]
    then
        args="${args} ${simctrl2}"
    else
        args="${args} ${1}"
    fi
    shift
done

echo $cmd $args
$cmd $args
