/* 
*  Copyright 2014 Matthias Fuchs
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*/

#include "stromx/raspi/impl/Gpio.h"

#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LOW  0
#define HIGH 1

namespace stromx
{
namespace raspi
{
namespace impl
{

int GPIOExport(int pin)
{
#define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) 
    {
        fprintf(stderr, "Failed to open export for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

int GPIOUnexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd)
    {
        fprintf(stderr, "Failed to open unexport for writing!\n");
        return(-1);
    }

    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

int GPIODirection(int pin, int dir)
{
    static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd)
    {
        fprintf(stderr, "Failed to open gpio direction for writing!\n");
        return(-1);
    }

    if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3))
    {
        fprintf(stderr, "Failed to set direction!\n");
        return(-1);
    }

    close(fd);
    return(0);
}

int GPIOEdge(int pin, int edge)
{
    static const char s_edges_str[]  = "rising\0falling\0both";

#define EDGE_MAX 30
    char path[EDGE_MAX];
    int fd;

    snprintf(path, EDGE_MAX, "/sys/class/gpio/gpio%d/edge", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd)
    {
        fprintf(stderr, "Failed to open gpio edge for writing!\n");
        return(-1);
    }
    
    const char* edge_str = 0;
    int str_length = 0;
    switch(edge)
    {
        case RISING:
            edge_str = &s_edges_str[0];
            str_length = 6;
            break;
        case FALLING:
            edge_str = &s_edges_str[7];
            str_length = 7;
            break;
        case BOTH:
            edge_str = &s_edges_str[15];
            str_length = 4;
            break;
        default:
            fprintf(stderr, "Unknown edge flag\n");
            return(-1);
    }

    if (-1 == write(fd, edge_str, str_length))
    {
        fprintf(stderr, "Failed to set edge!\n");
        return(-1);
    }

    close(fd);
    return(0);    
}


int GPIORead(int pin)
{
#define VALUE_MAX 30
    char path[VALUE_MAX];
    char value_str[3] = "";
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (-1 == fd)
    {
        fprintf(stderr, "Failed to open gpio value for reading!\n");
        return(-1);
    }

    if (-1 == read(fd, value_str, 3)) 
    {
        fprintf(stderr, "Failed to read value!\n");
        return(-1);
    }

    close(fd);

    return(atoi(value_str));
}

int GPIOWrite(int pin, int value)
{
    static const char s_values_str[] = "01";

    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) 
    {
        fprintf(stderr, "Failed to open gpio value for writing!\n");
        return(-1);
    }

    if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) 
    {
        fprintf(stderr, "Failed to write value!\n");
        return(-1);
    }

    close(fd);
    return(0);
}

int GPIOOpen(int pin, int& socket)
{
    char path[VALUE_MAX];
    int fd;
    char value_str[3] = "";

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY| O_NONBLOCK);
    if (-1 == fd)
    {
        fprintf(stderr, "Failed to open gpio value for reading!\n");
        return(-1);
    }

    // read the current value to make sure the first call to poll() does not
    // return immediately
    if (-1 == read(fd, value_str, 3)) 
    {
        fprintf(stderr, "Failed to read value!\n");
        return(-1);
    }
    
    socket = fd;
    return(0);
}

int GPIOCreatePipe(int& readEnd, int& writeEnd)
{
    int pipefd[2] = {0, 0};
    int rc;
    rc = pipe(pipefd);
    
    if (-1 == rc) 
    {
        fprintf(stderr, "Failed to create pipe!\n");
        return(-1);
    }
    
    readEnd = pipefd[0];
    writeEnd = pipefd[1];
    return(0);
}

int GPIOPoll(int gpio, int readEnd, bool & interrupt)
{
#define MAX_BUF 64
    struct pollfd fdset[2];
    int nfds = 2;
    int rc;
    char buf[MAX_BUF] = "";
    interrupt = false;
    
    memset((void*)fdset, 0, sizeof(fdset));
    
    fdset[0].fd = readEnd;
    fdset[0].events = POLLIN;

    fdset[1].fd = gpio;
    fdset[1].events = POLLPRI;

    rc = poll(fdset, nfds, -1);      

    if (rc < 0)
    {
        fprintf(stderr, "Failed to poll!\n");
        return(-1);
    }

    if (fdset[1].revents & POLLPRI)
    {
        return(0);
    }

    if (fdset[0].revents & POLLIN)
    {
        interrupt = true;
        if (-1 == read(fdset[0].fd, buf, 1))
        {
            fprintf(stderr, "Failed to read pipe value!\n"); 
            return(-1);
        }
        return(0);
    }
    
    fprintf(stderr, "Unexpected return of poll()!\n"); 
    return(-1);
}

int GPIOGetValue(int socket)
{
    char buf[MAX_BUF] = "";
    
    if (-1 == lseek(socket, 0, SEEK_SET)) 
    {
        fprintf(stderr, "Failed to seek GPIO value!\n"); 
        return(-1);
    }
    
    if (-1 == read(socket, buf, MAX_BUF)) 
    {
        fprintf(stderr, "Failed to read GPIO value!\n"); 
        return(-1);
    }
    
    return(atoi(buf));
}

int GPIOCloseSocket(int socket)
{
    if (close(socket))
    {
        fprintf(stderr, "Failed to close socket!\n");
    }
    
    return(0);
}

int GPIOSendInterrupt(int writeEnd)
{
    if (write(writeEnd, "\n", 1) != 1)
    {
        fprintf(stderr, "Failed to send interrupt!\n");
    }
    
    return(0);
}



}
}
}
