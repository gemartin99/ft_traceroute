#include "ft_traceroute.h"

void sender(t_traceroute *data)
{
    //CREAMOS SOCKETS



    (void)data;
    while (data->ttl <= data->ttl_max)
    {
        //MODIFICAMOS EL TTL DEL SOCKET

        //CALCULAMOS EL TIEMPO

        //ESPERAR RESPUESTA ICMP

        //SI ES EL DESTINO TERMINAR O TIMEOUT
        data->ttl++;
    }
}