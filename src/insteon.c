#include "insteon.h"
#include "main.h"
#include "rs232.h"
#include "printer.h"

extern int comport;

void sendBuffer(int* buffer, int bufferLen)
{
    int i;
    print_debug("Sending bytes... ");
    for (i = 0; i < bufferLen; i++)
    {
        print_debug("%02X ", buffer[i]);
        SendByte(comport, buffer[i]);
    }
    print_debug("\n");
    print_debug("%i bytes sent!\n", bufferLen);
}

Command parse_command(char* commandName)
{
    if (strcmp(commandName, "on") == 0)
    {
        return CMD_ON;
    }
    else if (strcmp(commandName, "off") == 0)
    {
        return CMD_OFF;
    }
    else if (strcmp(commandName, "status") == 0)
    {
        return CMD_STATUS;
    }
    else if (strcmp(commandName, "temp_ambient") == 0)
    {
        return CMD_TEMP_AMBIENT;
    }
    
    return -1;
}

void parse_device(char* deviceStr, unsigned int* deviceId)
{
    sscanf(deviceStr, "%2X%2X%2X", &deviceId[0], &deviceId[1], &deviceId[2]);
}

int parse_extra(char* extra)
{
    return atoi(extra);
}

int send_command(unsigned int* deviceId, Command command, int extra)
{
    switch (command)
    {
        case CMD_ON:
            return insteon_turn_on(deviceId, extra);
        case CMD_OFF:
            return insteon_turn_off(deviceId);
        case CMD_STATUS:
            return insteon_status(deviceId);
        case CMD_TEMP_AMBIENT:
            return insteon_temp_ambient(deviceId);
        default:
            return -1;
    }
}

int parse_message(Command command, unsigned char* message)
{
    switch (command)
    {
        case CMD_ON:
        case CMD_OFF:
            print_debug("Returning result 'ok'\n");
            print_result("ok\n");
            return 0;
        case CMD_STATUS:
            print_debug("Returning result '%i'\n", message[10]);
            print_result("%i\n", message[10]);
            return 0;
        case CMD_TEMP_AMBIENT:
            print_debug("Returning result '%i'\n", message[10] / 2);
            print_result("%i\n", message[10] / 2);
            return 0;
        default:
            print_error("Unable to parse message!\n");
            return 1;
    }
}

int insteon_turn_on(unsigned int* deviceId, int level)
{
    int buffer[8];
    
    if (level == -1)
        level = 0xFF;
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_ON;
    buffer[7] = level;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_turn_off(unsigned int* deviceId)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_OFF;
    buffer[7] = 0x00;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_status(unsigned int* deviceId)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_STATUS;
    buffer[7] = 0x00;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_temp_ambient(unsigned int* deviceId)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_CONTROL;
    buffer[7] = CMD_TEMP_AMBIENT;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}
