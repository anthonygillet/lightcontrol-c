#include "insteon.h"
#include "main.h"
#include "rs232.h"
#include "printer.h"

extern int comport;

int insteon_turn_on(unsigned int* deviceId, int level);
int insteon_turn_off(unsigned int* deviceId);
int insteon_status(unsigned int* deviceId);
int insteon_temp_get_ambient(unsigned int* deviceId);
int insteon_temp_get_mode(unsigned int* deviceId);
int insteon_temp_set_mode(unsigned int* deviceId, int mode);
int insteon_temp_get_setpoint(unsigned int* deviceId);
int insteon_temp_set_cool_setpoint(unsigned int* deviceId, int setpoint);
int insteon_temp_set_heat_setpoint(unsigned int* deviceId, int setpoint);

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
    else if (strcmp(commandName, "temp_get_ambient") == 0)
    {
        return CMD_TEMP_GET_AMBIENT;
    }
    else if (strcmp(commandName, "temp_get_mode") == 0)
    {
        return CMD_TEMP_GET_MODE;
    }
    else if (strcmp(commandName, "temp_set_mode") == 0)
    {
        return CMD_TEMP_SET_MODE;
    }
    else if (strcmp(commandName, "temp_get_setpoint") == 0)
    {
        return CMD_TEMP_GET_SETPOINT;
    }
    else if (strcmp(commandName, "temp_set_cool_setpoint") == 0)
    {
        return CMD_TEMP_SET_COOL_SETPOINT;
    }
    else if (strcmp(commandName, "temp_set_heat_setpoint") == 0)
    {
        return CMD_TEMP_SET_HEAT_SETPOINT;
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
        case CMD_TEMP_GET_AMBIENT:
            return insteon_temp_get_ambient(deviceId);
        case CMD_TEMP_GET_MODE:
            return insteon_temp_get_mode(deviceId);
        case CMD_TEMP_SET_MODE:
            return insteon_temp_set_mode(deviceId, extra);
        case CMD_TEMP_GET_SETPOINT:
            return insteon_temp_get_setpoint(deviceId);
        case CMD_TEMP_SET_COOL_SETPOINT:
            return insteon_temp_set_cool_setpoint(deviceId, extra);
        case CMD_TEMP_SET_HEAT_SETPOINT:
            return insteon_temp_set_heat_setpoint(deviceId, extra);
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
        case CMD_TEMP_SET_COOL_SETPOINT:
        case CMD_TEMP_SET_HEAT_SETPOINT:
        case CMD_TEMP_SET_MODE:
            print_debug("Returning result 'ok'\n");
            print_result("ok\n");
            return 0;
        case CMD_STATUS:
            print_debug("Returning result '%i'\n", message[10]);
            print_result("%i\n", message[10]);
            return 0;
        case CMD_TEMP_GET_AMBIENT:
            print_debug("Returning result '%i'\n", message[10] / 2);
            print_result("%i\n", message[10] / 2);
            return 0;
        case CMD_TEMP_GET_MODE:
            print_debug("Returning result '%i'\n", message[10]);
            print_result("%i\n", message[10]);
            return 0;
        case CMD_TEMP_GET_SETPOINT:
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

int insteon_temp_get_ambient(unsigned int* deviceId)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_CONTROL;
    buffer[7] = CMD_TEMP_GET_AMBIENT;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_temp_get_mode(unsigned int* deviceId)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_CONTROL;
    buffer[7] = CMD_TEMP_GET_MODE;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_temp_set_mode(unsigned int* deviceId, int mode)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_CONTROL;
    
    switch (mode)
    {
        case 1:
            buffer[7] = CMD_TEMP_MODE_HEAT;
            break;
        case 2:
            buffer[7] = CMD_TEMP_MODE_COOL;
            break;
        case 3:
            buffer[7] = CMD_TEMP_MODE_AUTO;
            break;
        default:
            buffer[7] = CMD_TEMP_MODE_OFF;
            break;
    }
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_temp_get_setpoint(unsigned int* deviceId)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_ZONE_INFO;
    buffer[7] = CMD_TEMP_GET_SETPOINT;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_temp_set_cool_setpoint(unsigned int* deviceId, int setpoint)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_SET_COOL_SETPOINT;
    buffer[7] = setpoint*2;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}

int insteon_temp_set_heat_setpoint(unsigned int* deviceId, int setpoint)
{
    int buffer[8];
    
    buffer[0] = CMD_PREFIX;
    buffer[1] = CMD_INSTEON;
    buffer[2] = deviceId[0];
    buffer[3] = deviceId[1];
    buffer[4] = deviceId[2];
    buffer[5] = CMD_STD_FLAGS;
    buffer[6] = CMD_TEMP_SET_HEAT_SETPOINT;
    buffer[7] = setpoint*2;
    
    sendBuffer(buffer, ARRAYLEN(buffer));
    
    return ARRAYLEN(buffer);
}
