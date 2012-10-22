#include "main.h"

#include <stdio.h>

#include "rs232.h"
#include "printer.h"
#include "insteon.h"

int comport = 16;

#define LOCKFILE "/tmp/insteon_lock"

int acquire_lock()
{
    int i = 0;

    print_debug("Acquiring file lock...");

    while(i < 20000)
    {
        if (!mkdir(LOCKFILE, S_IRWXU))
        {
            print_debug("\nAcquired lock successfully.\n");
            return 0;
        }
        print_debug(".");
        usleep(1000);
        i++;
    }
    
    print_debug("\nCould not acquire file lock in time!\n");
    return 1;
}

void release_lock()
{
    rmdir(LOCKFILE);
}

int main(int argc, char *argv[])
{
    unsigned char buffer[4096] = {0};
    int bufferLen = 0;
    int bytesReceived = 0;
    int bytesWritten = 0;
    int timer = 0;
    unsigned int deviceId[3];
    Command command;
    int extra = -1;
    
    if (argc < 3)
    {
        print_error("Usage: insteon deviceId [on|off|status|temp_ambient]\n");
        return 1;
    }

    // Parse arguments
    parse_device(argv[1], deviceId);
    command = parse_command(argv[2]);
    if (argc > 3)
        extra = parse_extra(argv[3]);

    if (command < 0)
    {
        print_error("Invalid command name!\n");
        return 1;
    }

    print_debug("*****************\n");
    print_debug("Device: %s\n", argv[1]);
    print_debug("Command: %s\n", argv[2]);
    if (argc > 3)
        print_debug("Extra: %s\n", argv[3]);
    print_debug("*****************\n");

    if (acquire_lock())
    {
        print_error("Timeout while acquiring lock!\n");
        return 1;
    }
    
    print_debug("Opening comport\n");

    if(OpenComport(comport, 19200))
    {
        print_error("Can not open comport\n");
        release_lock();
        return 1;
    }
    
    // Send command
    bytesWritten = send_command(deviceId, command, extra);
    if (bytesWritten < 0)
    {
        print_error("Invalid command or internal error!\n");
        goto return_error;
    }

    // Wait for response
    while(1)
    {
        bytesReceived = PollComport(16, buffer + bufferLen, 4095);
        //print_debug("br=%d bl=%d  ", bytesReceived, bufferLen);
        bufferLen += bytesReceived;
        //print_debug("br=%d bl=%d\n", bytesReceived, bufferLen);
        if(bytesReceived > 0)
        {
            int i;
            timer = 0;
            print_debug("received %i bytes: (%i total)", bytesReceived, bufferLen);
            for(i=0; i < bufferLen; i++)
            {
                print_debug("%02X ", buffer[i]);
            }
            print_debug("\n");

            if (bufferLen > bytesWritten)
            {
                unsigned int ack = *(buffer + bytesWritten);
                unsigned char* message = buffer + bytesWritten + 1;
                int messageLen = bufferLen - bytesWritten - 1;
                
                if (ack != CMD_ACK)
                {
                    print_error("Message not acknowledged!\n");
                    goto return_error;
                }
                
                if (messageLen > 1)
                {
                    if (message[0] == CMD_PREFIX)
                    {
                        if (message[1] == CMD_INSTEON_STD_RECEIVED)
                        {
                            if (messageLen >= 11)
                            {
                                print_debug("Insteon standard message received!\n");
                                if(parse_message(command, message))
                                {
                                    goto return_error;
                                }
                                goto return_success;
                            }
                        }
                        else if (message[1] == CMD_INSTEON_EXT_RECEIVED)
                        {
                            if (messageLen >= 25)
                            {
                                print_debug("Insteon extended message received!\n");
                                if(parse_message(command, message))
                                {
                                    goto return_error;
                                }
                                goto return_success;
                            }
                        }
                        else if (message[1] == CMD_X10_RECEIVED)
                        {
                            if (messageLen >= 4)
                            {
                                print_debug("X10 message received!\n");
                                if(parse_message(command, message))
                                {
                                    goto return_error;
                                }
                                goto return_success;
                            }
                        }
                        else
                        {
                            print_error("Unexpected message received! '");
                            for(i=0; i < messageLen; i++)
                            {
                                print_error("%02X ", message[i]);
                            }
                            print_error("'\n");
                            goto return_error;
                        }
                    }
                    else
                    {
                        print_error("Unexpected message header received! '");
                        for(i=0; i < messageLen; i++)
                        {
                            print_error("%02X ", message[i]);
                        }
                        print_error("'\n");
                        goto return_error;
                    }
                }
            }
        }

        usleep(250);
        timer++;
        if (timer > 4000)
        {
            print_error("Timeout while waiting for bytes!");
            goto return_error;
        }
    }

return_success:
    CloseComport(16);
    release_lock();
    return 0;
return_error:
    CloseComport(16);
    release_lock();
    return 1;
}

