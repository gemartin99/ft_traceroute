#include "ft_traceroute.h"

t_traceroute *init_struct()
{
    t_traceroute *data;
    
    data = (t_traceroute *)malloc(sizeof(t_traceroute));
    if (!data)
        ft_exit(NULL);
    bzero(data, sizeof(*data));
    data->ip = NULL;
    data->help = false;
    return (data);
}


int main(int argc, char **argv)
{
    t_traceroute *data;

    if (argc == 1)
    {
        fprintf(stderr, "%s: usage error: Destination address required\n", argv[0]);
        return(1);
    }
    signal(SIGINT, handle_sigint);
    data = init_struct();
    parse(argc, argv, data);
    //send_socket(data);
    exit(0);
}