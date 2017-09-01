DEBUG=1
PART=TM4C1294NCPDT

# root dirs
ROOT=../tivaware
CC3100_SDK_ROOT=../cc3100-sdk
PAHO_MQTTPACKET_ROOT=./paho

include ${ROOT}/makedefs

# source files macro
VPATH=.
VPATH+=./platform
VPATH+=./common
VPATH+=./simplelink/include
VPATH+=./simplelink
VPATH+=./simplelink/source
VPATH+=${ROOT}/utils
VPATH+=${CC3100_SDK_ROOT}
VPATH+=${PAHO_MQTTPACKET_ROOT}/MQTTPacket/src

# header files macro
IPATH=.
IPATH+=./platform
IPATH+=./common
IPATH+=./simplelink/include
IPATH+=./simplelink
IPATH+=./simplelink/source
IPATH+=${ROOT}
IPATH+=${PAHO_MQTTPACKET_ROOT}/MQTTPacket/src

# make all
all: ${COMPILER}
all: ${COMPILER}/mqtt_cli.axf

# make clean
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

# target dir
${COMPILER}:
	@mkdir -p ${COMPILER}

# build rules
# project
${COMPILER}/mqtt_cli.axf: ${COMPILER}/device.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/driver.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/flowcont.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/fs.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/netapp.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/netcfg.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/nonos.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/socket.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/spawn.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/wlan.o

${COMPILER}/mqtt_cli.axf: ${COMPILER}/MQTTConnectClient.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/MQTTPacket.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/MQTTSerializePublish.o

${COMPILER}/mqtt_cli.axf: ${COMPILER}/board.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/cli_uart.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/spi.o

${COMPILER}/mqtt_cli.axf: ${COMPILER}/ustdlib.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/uartstdio.o

${COMPILER}/mqtt_cli.axf: ${COMPILER}/vectors.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/main.o
${COMPILER}/mqtt_cli.axf: ${COMPILER}/sl_config.o

${COMPILER}/mqtt_cli.axf: ${ROOT}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/mqtt_cli.axf: TM4C1294NCPDT.ld

SCATTERgcc_mqtt_cli=TM4C1294NCPDT.ld
ENTRY_mqtt_cli=startup
CFLAGSgcc=-DTARGET_IS_TM4C129_RA1 -D_USE_CLI_

ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
