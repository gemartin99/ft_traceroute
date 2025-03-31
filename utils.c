#include "ft_traceroute.h"

void ft_exit(t_traceroute *data)
{
    if (data)
        free(data);
    exit(1);
}

void ft_exit_error(t_traceroute *data, char *msg)
{
    if (data)
        free(data);
    if (msg)
        fprintf(stderr, "%s\n", msg);
    exit(1);
}

void print_help(t_traceroute *data)
{
    printf("Usage: ft_traceroute <host> [options]\n");

    if (data)
        free(data);

    exit(0);
}


void handle_sigint(int sig)
{
    (void)sig;
    exit(1);
}