#include "ft_traceroute.h"

int is_digits(char *s, t_traceroute *data)
{
    int i = -1;
    if (!s)
        return (0);
    while(s[++i])
    {
        if (s[i] < 48 || s[i] > 57)
        {
            fprintf(stderr, "ft_traceroute: invalid argument: '%s'\n", s);
            ft_exit(data);
        }
    }
    return (1);
}

int valid_ip(char *ip)
{
    struct sockaddr_in sa;

    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1)
        return 1;
    return 0;
}

int valid_hostname(char *hostname, t_traceroute *data)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // Solo IPv4
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
    {
        return 0; //si hay algo mal
    }
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ip_str, sizeof(ip_str));
    data->ip = strdup(ip_str);
    freeaddrinfo(res); 
    return 1; //si todo va bien
}


void parse(int argc, char **argv, t_traceroute *data)
{
    int i = 1;
    while (i <= argc && argv[i])
    {
        if (!strcmp("-?", argv[i]) || !strcmp("-h", argv[i]))
            print_help(data);
        //else if (!strcmp("-s", argv[i]))
        //    data->silent = true;
        else if (!strcmp("-t", argv[i])) //flag timeout
        {
            if (i + 1 >= argc)
            {
                print_help(data);
            }
            else if (is_digits(argv[i + 1], data) == 1 && atoi(argv[i + 1]) > 0 && atoi(argv[i + 1]) < 6)
            {
                data->timeout = atoi(argv[i + 1]);
                i++;
            }
            else if (atoi(argv[i + 1]) != 0)
            {
                fprintf(stderr, "%s: invalid argument: '%s': out of range: 1 <= value <= 5\n", argv[0], argv[i + 1]);
                ft_exit(data);
            }
            else
            {
                fprintf(stderr, "%s: invalid argument: '%s'\n", argv[0], argv[i + 1]);
                ft_exit(data);
            }
        }
        else if (!strcmp("-p", argv[i])) //flag num packets
        {
            if (i + 1 >= argc)
            {
                print_help(data);
            }
            else if (is_digits(argv[i + 1], data) == 1 && atoi(argv[i + 1]) > 0 && atoi(argv[i + 1]) < 11)
            {
                data->num_packets = atoi(argv[i + 1]);
                i++;
            }
            else if (atoi(argv[i + 1]) != 0)
            {
                fprintf(stderr, "%s: invalid argument: '%s': out of range: 1 <= value <= 10\n", argv[0], argv[i + 1]);
                ft_exit(data);
            }
            else
            {
                fprintf(stderr, "%s: invalid argument: '%s'\n", argv[0], argv[i + 1]);
                ft_exit(data);
            }
        }
        /*else if (!strcmp("-c", argv[i]))
        {
            if (i + 1 >= argc)
            {
                print_help(data);
            }
            else if (is_digits(argv[i + 1], data) == 1 && atof(argv[i + 1]) > 0 && atof(argv[i + 1]) < 9223372036854775807)
            {
                data->num_packets = atof(argv[i + 1]);
                i++;
            }
            else if (atof(argv[i + 1]) != 0)
            {
                fprintf(stderr, "%s: invalid argument: '%s': out of range: 1 <= value <= 65536\n", argv[0], argv[i + 1]);
                ft_exit(data);
            }
            else
            {
                fprintf(stderr, "%s: invalid argument: '%s'\n", argv[0], argv[i + 1]);
                ft_exit(data);
            }
        }*/
        else if (valid_ip(argv[i]) == 1)
        {
            if (data->ip_active == true)
            {
                printf("Cannot handle 'packetlen' cmdline arg '%s' on position 2 (argc 2)\n", argv[i]);
                ft_exit(data);
            }
            data->ip = argv[i];
            data->ip_active = true;
        }
        else if (valid_hostname(argv[i], data) == 1)
        {
            if (data->ip_active == true)
            {
                printf("Cannot handle 'packetlen' cmdline arg '%s' on position 2 (argc 2)\n", argv[i]);
                ft_exit(data);
            }
            data->ip_active = true;
        }
        else
        {
            fprintf(stderr, "%s: %s: Name or service not known\n", argv[0], argv[i]);
            ft_exit(data);
        }
        i++;            
    }
    if (!data->ip)
    {
        fprintf(stderr, "%s: usage error: Destination adress required\n", argv[0]);
        ft_exit(data);
    }
}