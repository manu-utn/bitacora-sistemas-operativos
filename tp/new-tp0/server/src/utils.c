#include "../include/utils.h"

/*
 * Paso (1): socket/3
 * Tenemos la explicación del lado del cliente
 *
 * Paso (2): Bind/3
 * 1. Para reservar un puerto a un socket
 * 2. Sólo tiene sentido si usaremos listen() osea si estamos escuchando como
 * servidor (el kernel se quedará esperando algún socket descriptor de un
 * proceso)
 *
 * sockfd: Es el socket file descriptor devuelto por socket()
 *
 * my_addr: es un *ptr contiene informacion de la dirección, dirección IP, puerto, ..
 *
 * addrlen: es el tamaño en bytes de esa dirección

 * Paso (3): Listen/2
 * 1. Se requiere previamente asociar un número de puerto a la dirección IP (con la
 * syscall bind() para asociarlo)
 * 2. Las conexiones entrantes (clientes) las acepta el servidor con la syscall accept()
 * si dicho servidor lo permite
 *
 * - sockfd: es el file descriptor de la syscall socket()
 *
 * - backlog: es el número de conexiones pendienes, el limite de conexiones entrantes a encolar
 */
int iniciar_servidor(void)
{
	int socket_servidor;

  struct addrinfo hints, *servinfo;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(IP, PUERTO, &hints, &servinfo);

    // Creamos el socket de escucha del servidor
    socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol); // --> (1)

    // Asociamos el socket a un puerto
    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen); // --> (2)

    // Escuchamos las conexiones entrantes
    listen(socket_servidor, SOMAXCONN); // --> (3)

    freeaddrinfo(servinfo);

    log_trace(logger, "Listo para escuchar a mi cliente");

    return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
	struct sockaddr dir_cliente;
  socklen_t tam_direccion = sizeof(struct sockaddr);

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, &dir_cliente, &tam_direccion); // --> (1)

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) != 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

//podemos usar la lista de valores para poder hablar del for y de como recorrer la lista
t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
	//return NULL; // <<-- fe de errata (error)
}
