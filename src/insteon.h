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
    CMD_TEMP_MODE=0x02,
    CMD_TEMP_AMBIENT=0x03
} Command;

Command parse_command(char* commandName);
void parse_device(char* deviceStr, unsigned int* deviceId);
int parse_extra(char* extra);
int send_command(unsigned int* deviceId, Command command, int extra);
int parse_message(Command command, unsigned char* message);
int insteon_turn_on(unsigned int* deviceId, int level);
int insteon_turn_off(unsigned int* deviceId);
int insteon_status(unsigned int* deviceId);
int insteon_temp_ambient(unsigned int* deviceId);
