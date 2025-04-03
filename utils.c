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