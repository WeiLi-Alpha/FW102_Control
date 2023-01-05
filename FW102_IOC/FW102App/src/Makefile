TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = FW102
# FW102.dbd will be created and installed
DBD += FW102.dbd

# FW102.dbd will be made up from these files:
FW102_DBD += base.dbd

# Include dbd files from all support applications:
#FW102_DBD += xxx.dbd
FW102_DBD += calc.dbd
FW102_DBD += stream.dbd
FW102_DBD += asyn.dbd
FW102_DBD += drvAsynSerialPort.dbd

# Add all the support libraries needed by this IOC
#FW102_LIBS += xxx
FW102_LIBS += asyn
FW102_LIBS += calc
FW102_LIBS += stream

# FW102_registerRecordDeviceDriver.cpp derives from FW102.dbd
FW102_SRCS += FW102_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
FW102_SRCS_DEFAULT += FW102Main.cpp
FW102_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#FW102_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
FW102_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

