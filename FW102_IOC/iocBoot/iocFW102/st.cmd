#!../../bin/linux-x86_64/FW102

## You may have to change FW102 to something else
## everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/FW102.dbd"
FW102_registerRecordDeviceDriver pdbbase

epicsEnvSet("STREAM_PROTOCOL_PATH","${TOP}/protocols")

## Load record instances
#dbLoadRecords("db/xxx.db","user=epicsdev")
dbLoadTemplate("db/FW102.substitutions")

drvAsynSerialPortConfigure("FW","/dev/ttyUSB0")

asynSetOption("FW",0,"baud","115200")
asynSetOption("FW",0,"bits","8")
asynSetOption("FW",0,"parity","none")
asynSetOption("FW",0,"stop","1")
asynSetOption("FW",0,"clocal","Y")
asynSetOption("FW",0,"cstscts","N")


cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=epicsdev"
