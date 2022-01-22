#include "utils.h"


void* serializar_paquete(t_paquete* paquete, int bytes)
{
	void * magic = malloc(bytes);
	int desplazamiento = 0;

	memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return magic;
}

/*
 *
 * addrinfo:
 * 1. Devuelve un puntero a una lista linkeada que pueden ser una o más
 * estructuras del tipo addrinfo
 * 2. Si tuvo éxito retorna 0 (osea cero)
 * 3. Es importante luego usar freeaddrinfo(res) (donde el parámetro res
 * representa la estructura de dirección)
 *
 * Nota (1): Hints
 * 1. apunta a una estructura "addrinfo" con información que usaremos
 * 2. contiene la familia de protocolos y el protocolo a utilizar

 * Paso (2): Socket/3
 * Creamos el socket (que es un archivo conocido como File descriptor)
 *
 * ai_family: define la familia de protocolos
 * ai_socktype: define el tipo de socket
 * ai_protocol: define el protocolo de la familia a utilizar
 *
 * Paso (3) Connect/2
 * Establecemos la conxión con el socket

 * ai_addr: es donde se almacena la dirección
 * ai_addrlen: define el tamaño de la dirección
 */
int crear_conexion(char *ip, char* puerto)
{
  struct addrinfo hints; // <-- (1)
  struct addrinfo *server_info;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // permite obviar entre IPV4 y IPV6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // nos asigna el IP del localhost actual

  getaddrinfo(ip, puerto, &hints, &server_info);

  // --> (2)
  int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

  // --> (3)
  connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

	freeaddrinfo(server_info);

	return socket_cliente;
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}


void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
}

/* t_paquete* crear_super_paquete(void) */
/* { */
/* 	//me falta un malloc! */
/* 	/\* t_paquete* paquete; *\/ */

/* 	//descomentar despues de arreglar */
/* 	//paquete->codigo_operacion = PAQUETE; */
/* 	//crear_buffer(paquete); */
/* 	/\* return paquete; *\/ */
/* } */

t_paquete* crear_paquete(void)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = PAQUETE;
	crear_buffer(paquete);
	return paquete;
}

void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio)
{
	paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

	memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
	memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

	paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
}

void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
