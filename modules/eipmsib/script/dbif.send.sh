#!/bin/sh

# --clicktocall CallStart req--
# sessionID
# gwSessionID
# subscriberName
# recordingType
# callingNumber
# calledNumber
# serviceCode
# ringbackToneType
# waitingMentID
# scenarioType
# callMentID
# callingCID
# calledCID
# recordingFileName
# isAllRecording
# endIfRecordingFailed
# endIfRecordingEnded
# hostingCode
# wirelessTimeout
# wiredTimeout
dbifsim eipmsib 100 "c,s,s,s,i,s,s,i,i,i,i,i,s,s,s,i,i,i,i,i,i" -1 "S0001" "G0001" "ythan" 1 "01012345678" "01087654321" 1 0 0 0 11 "callingCID" "calledCID" "testFileName" 0 0 0 0 0 0

# --clicktocall CallStop req--
# serviceID
dbifsim eipmsib 101 "c,s" -1 "service0001"

# --clicktocall CallStartRecording req--
# serviceID
dbifsim eipmsib 102 "c,s,s" -1 "service0001" "testFileName"

# --clicktocall CallStopRecording req--
# serviceID
dbifsim eipmsib 103 "c,s" -1 "service0001"
