#include "ft_traceroute.h"

void sender(t_traceroute *data)
{
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sock < 0)
    {
        fprintf(stderr, "Failed to create DGRAM socket\n");
        ft_exit(data);
    }
    int icmp_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmp_sock < 0)
    {
        fprintf(stderr, "Failed to create RAW socket\n");
        ft_exit(data);
    }

    struct sockaddr_in dest_addr;
    ft_memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(33434); //Puerto UDP
    if (inet_pton(AF_INET, data->ip, &dest_addr.sin_addr) != 1)
    {
        fprintf(stderr, "Invalid IP address\n");
        ft_exit(data);
    }

    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    char recv_buf[1024];

    while (data->ttl <= data->ttl_max)
    {
        //MODIFICAMOS EL TTL DEL SOCKET
        setsockopt(udp_sock, IPPROTO_IP, IP_TTL, &data->ttl, sizeof(data->ttl));

        //CALCULAMOS EL TIEMPO
        struct timeval start, end;
        gettimeofday(&start, NULL); //antes de enviar

        //ENVIAMOS UDP VACIO
        if (sendto(udp_sock, "", 1, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
        {
            fprintf(stderr, "Failed to send UDP packet\n");
            continue;
        }

        //ESPERAR RESPUESTA ICMP
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(icmp_sock, &fds);
        struct timeval timeout = {1, 0};
        int ready = select(icmp_sock + 1, &fds, NULL, NULL, &timeout);
        //printf("READY: %d\n", ready);
        if (ready > 0)
        { 
            if (recvfrom(icmp_sock, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&sender_addr, &addr_len) > 0)
            {
                gettimeofday(&end, NULL);
                double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

                printf("%2d  %s  %.3f ms\n", data->ttl, inet_ntoa(sender_addr.sin_addr), time_ms);

                // Si llegamos al destino, terminamos
                if (sender_addr.sin_addr.s_addr == dest_addr.sin_addr.s_addr)
                    break;
            }
        }
        else
            printf("%2d  * * *\n", data->ttl);
        data->ttl++;
    }
    close(udp_sock);
    close(icmp_sock);
}