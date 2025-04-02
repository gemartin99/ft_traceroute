#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/time.h>
#include <netinet/ip_icmp.h>
#include <time.h>

typedef struct s_traceroute
{
    char *ip;
    bool help;
} t_traceroute;

void    ft_exit(t_traceroute *data);
void    ft_exit_error(t_traceroute *data, char *msg);
void    print_help(t_traceroute *data);
void    handle_sigint(int sig);
void    parse(int argc, char **argv, t_traceroute *data);
void    sender(t_traceroute *data);

#endif