#!/bin/sh

#host=`cat ../config/uas_host`
#port=`cat ../config/uas_port`
#peer_host=`cat ../config/scpas_host`
#peer_port=`cat ../config/scpas_port`

host=`uname -n`
ipaddr=`cat /etc/hosts | grep $host | awk '{ print $1 }' `
pxyport=`cat ../config/agent.csv | grep -v 'SEQ' | grep -v '#' | awk '{ split($0,arr,";"); printf("%s",arr[1]); }'`
uasport=`cat ../config/agent.csv | grep -v 'SEQ' | grep -v '#' | awk '{ split($0,arr,";"); printf("%s",arr[3]); }'`
pxyaddr=$ipaddr:$pxyport
uasaddr=$ipaddr:$uasport

cmd="sipp $ipaddr -p $uasport -l 100 -sf called.xml" 

simctrl="-m 1 -f 600 -nostdin -trace_msg -message_file ../report/called.res.tmp"
simctrl2="-m 1 -f 600 -nostdin -trace_msg -message_file"
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
