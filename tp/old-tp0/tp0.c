/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	// creamos una instancia de log
	logger = iniciar_logger();
	// agregamos en el registro de la instancia de logs un mensaje
	log_info(logger, "soy un log");

	// creamos el archivo de configuracion
	config = leer_config();
	// obtenemos el valor de variable "clave"
	valor = config_get_string_value(config, "CLAVE");
	// agregamos al registro de logs el valor
	log_info(logger, valor);

	leer_consola(logger);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	//antes de continuar, tenemos que asegurarnos que el servidor esté corriendo porque lo necesitaremos para lo que sigue.

	//crear conexion
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "IP");

	conexion = crear_conexion(ip, puerto);

	// enviamos la clave (que está en valor) al sevidor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	//
	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	// utilizamos funciones de la biblioteca commons
	// creamos una instancia de logger, indicamos en que archivo se agregarán los logs y de que tipo serán
	return log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(void)
{
	return config_create("tp0.config");
}

void leer_consola(t_log* logger)
{
	// leemos el primer caracter, el típico símbolo ">" de la terminal
	char* leido = readline(">");

	// alternativa sería comparando cel resultado de strcmp con != 0
	while(strncmp(leido, "", 1) != 0){
		// agregamos al registro de logs los caracteres ingresados en la consola
		log_info(logger, leido);
		// liberamos memoria por cada lectura
		free(leido);
		// leemos el resto de los caracteres ingresados, y los guardamos en "leido"
		leido = readline(">");
	}

	// liberamos memoria
	free(leido);
}

void paquete(int conexion)
{
	t_paquete* paquete = crear_paquete();
	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0){
		// agregamos al paquete el contenido,
		// y le sumamos 1 a la longitud del contenido a mandar por el centinela (fin de cadena)
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		free(leido);
		leido = readline(">");
	}

	free(leido);

	// mandamos el paquete al servidor
	enviar_paquete(paquete, conexion);
	// eliminamos porque ya no se necesita
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config)
	// con las funciones de las commons y del TP mencionadas en el enunciado

	// destruimos la instancia de logger que creamos al principio
	log_destroy(logger);
	// destruimos el config (es un puntero)
	config_destroy(config);

	liberar_conexion(conexion);
}
