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
    printf("Usage: ./ft_traceroute <hostname> [options]\n");

    printf("Options:\n");
    printf("  -h                Display this help message and exit.\n");
    printf("  -j <num>          Maximum number of hops (TTL). Must be between 1 and 30.\n");
    printf("  -p <num>          Number of packets to send per TTL. Must be between 1 and 10.\n");
    printf("  -t <num>          Maximum wait time for a reply (in seconds). Must be between 1 and 5.\n");
    printf("  -f <num>          Set the starting TTL\n");
    printf("  -q                Quiet mode (less verbose output)\n");
    printf("\n");
    if (data)
        free(data);

    exit(0);
}

void handle_sigint(int sig)
{
    (void)sig;
    exit(1);
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}