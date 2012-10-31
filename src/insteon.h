#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>

typedef enum {
    CMD_PREFIX=0x02,
    CMD_INSTEON=0x62,
    CMD_INSTEON_STD_RECEIVED=0x50,
    CMD_INSTEON_EXT_RECEIVED=0x51,
    CMD_ACK=0x06,
    CMD_NACK=0x15,
    CMD_STD_FLAGS=0x0F,
    CMD_EXT_FLAGS=0x1F,
    CMD_X10_RECEIVED=0x52,
    CMD_ON=0x11,
    CMD_OFF=0x13,
    CMD_STATUS=0x19,
    CMD_TEMP_CONTROL=0x6B,
    CMD_TEMP_ZONE_INFO=0x6A,
    CMD_TEMP_GET_AMBIENT=0x03,
    CMD_TEMP_GET_MODE=0x02,
    CMD_TEMP_SET_MODE=0x0F,
    CMD_TEMP_GET_SETPOINT=0x20,
    CMD_TEMP_SET_COOL_SETPOINT=0x6C,
    CMD_TEMP_SET_HEAT_SETPOINT=0x6D,
    CMD_TEMP_MODE_OFF = 0x09,
    CMD_TEMP_MODE_HEAT = 0x04,
    CMD_TEMP_MODE_COOL = 0x05,
    CMD_TEMP_MODE_AUTO = 0x06,
} Command;

Command parse_command(char* commandName);
void parse_device(char* deviceStr, unsigned int* deviceId);
int parse_extra(char* extra);
int send_command(unsigned int* deviceId, Command command, int extra);
int parse_message(Command command, unsigned char* message);
