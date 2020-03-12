#!/bin/sh

# --clicktocall start req--
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
dbifsim eipmsib 100 "c,s,s,s,i,s,s,i,i,i,i,i,s,s,s,i,i,i,i,i,i" -1 0 0 "S0001" "G0001" "ythan" 1 "01012345678" "01087654321" 1 0 0 0 11 "callingCID" "calledCID" "testFileName" 0 0 0 0 0 0
