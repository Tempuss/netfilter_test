#include "define.h"

bool inArray(uint32_t arr[], uint32_t needle)
{
    int i=0;
    int limit = sizeof(arr);
    for(;i<limit;i++)
    {
        if (arr[i] == needle)
            return true;
    }

    return false;
}

bool inArray(uint8_t arr[], uint8_t needle)
{
    int i=0;
    int limit = sizeof(arr);
    for(;i<limit;i++)
    {
        if (arr[i] == needle)
            return true;
    }

    return false;
}


int inArrayCheck(uint32_t arr[], uint32_t needle)
{
    int i=0;
    int limit = sizeof(arr);
    for(;i<limit;i++)
    {
        if (arr[i] == needle)
        {
                return i;
         }
    }

    return -1;

}

int inArrayCheck(uint8_t arr[], uint8_t needle)
{
    int i=0;
    int limit = sizeof(arr);
    for(;i<limit;i++)
    {
        if (arr[i] == needle)
        {
                return i;
         }
    }

    return -1;

}

bool inArray(char *arr[], char *needle)
{

    int i=0;
    int limit = sizeof(arr);
    for(;i<limit;i++)
    {
        if (strcmp(arr[i], needle) == 0){
            return true;
        }
    }

    return false;
}

int inArrayCheck(char *arr[], char *needle)
{

    int i=0;
    int limit = sizeof(arr);
    for(;i<limit;i++)
    {
        if (strcmp(arr[i], needle) == 0){
            return i;
        }
    }

    return -1;

}



char *ipToCharIp(uint32_t ip)
{
    struct in_addr ip_addr;
    ip_addr.s_addr =ip;

    return inet_ntoa(ip_addr);
}



/**
 * @brief printHex
 * @detail Print Packet Data with Hex Format
 * @param length
 * @param packet
 */
void printHex(int length, u_char* packet ) {

    int i=0;
    while(i<length) {
        printf("%02X ", packet[i]);

        ++i;
        if (i%8==0)
        {
            printf(" ");
        }
        if (i%16 == 0)
        {
            printf("\n");
        }
    }

    printf("\n");
}
void printHex(int length, const u_char* packet ) {

    int i=0;
    while(i<length) {
        printf("%02X ", packet[i]);

        ++i;
        if (i%8==0)
        {
            printf(" ");
        }
        if (i%16 == 0)
        {
            printf("\n");
        }
    }

    printf("\n");
}

void printHex(int length, const packet* packet ) {

    int i=0;
    while(i<length) {
        printf("%02X ", packet[i]);

        ++i;
        if (i%8==0)
        {
            printf(" ");
        }
        if (i%16 == 0)
        {
            printf("\n");
        }
    }

    printf("\n");
}
