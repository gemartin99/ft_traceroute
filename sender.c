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

    // Resolvemos el nombre de dominio a una dirección IP
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    int status = getaddrinfo(data->ip, NULL, &hints, &res);
    if (status != 0)
    {
        fprintf(stderr, "Failed to resolve IP for domain: %s\n", data->ip);
        ft_exit(data);
    }

    dest_addr.sin_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr;
    freeaddrinfo(res);

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(dest_addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    if (!data->hostname)
        data->hostname = data->ip;
    printf("Traceroute to %s (%s), %d hops max, 29 byte packets\n", data->hostname, ip_str, data->jumps);
    while (data->ttl <= data->jumps)
    {
        //MODIFICAMOS EL TTL DEL SOCKET
        setsockopt(udp_sock, IPPROTO_IP, IP_TTL, &data->ttl, sizeof(data->ttl));

        double total_time = 0;
        int received_responses = 0;
        char last_ip[INET_ADDRSTRLEN] = "*";
        int i = 0;
        while (i < data->num_packets)
        {
            //CALCULAMOS EL TIEMPO
            struct timeval start, end;
            gettimeofday(&start, NULL); //antes de enviar

            //ENVIAMOS UDP VACIO (1byte)
            if (sendto(udp_sock, "", 1, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
            {
                fprintf(stderr, "Failed to send UDP packet\n");
                i++;
                continue;
            }

            //ESPERAR RESPUESTA ICMP
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(icmp_sock, &fds);
            struct timeval timeout = {data->timeout, 0};
            int ready = select(icmp_sock + 1, &fds, NULL, NULL, &timeout);
            //printf("READY: %d\n", ready);
            if (ready > 0)
            { 
                if (recvfrom(icmp_sock, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&sender_addr, &addr_len) > 0)
                {
                    gettimeofday(&end, NULL);
                    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

                    if (i == 0)
                    {
                        printf("%2d  %s  %.3f ms ", data->ttl, inet_ntoa(sender_addr.sin_addr), time_ms);
                    }
                    else
                    {
                        // Para los siguientes paquetes, solo mostramos el tiempo
                        printf("%.3f ms ", time_ms);
                    }
                    if (i + 1 == data->num_packets)
                        printf("\n");

                    total_time += time_ms;
                    received_responses++;
                    strncpy(last_ip, inet_ntoa(sender_addr.sin_addr), INET_ADDRSTRLEN);
                    //printf("%2d  %s  %.3f ms\n", data->ttl, inet_ntoa(sender_addr.sin_addr), time_ms);

                    // Si llegamos al destino, terminamos
                    if (sender_addr.sin_addr.s_addr == dest_addr.sin_addr.s_addr)
                        break;
                }
            }
            i++;
        }
        if (received_responses > 0)
            ;//printf("%2d  %s  %.3f ms\n", data->ttl, last_ip, total_time / received_responses);
        else
            printf("%2d  * * *\n", data->ttl);
        
        // Si llegamos al destino, terminamos
        if (strcmp(last_ip, inet_ntoa(dest_addr.sin_addr)) == 0)
            break;

        data->ttl++;
    }
    close(udp_sock);
    close(icmp_sock);
}