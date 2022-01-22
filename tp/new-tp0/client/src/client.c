#include "client.h"

int main(void)
{
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();

	// agregamos en el registro un mensaje
	log_info(logger, "Hola! Soy un log");
	// creamos el archivo de config
	config = iniciar_config();
	// obtenemos el valor de la variable del 2do parámetro
	valor = config_get_string_value(config, CLIENT_CLAVE);
	// agregamos al registro de logs el valor
	log_info(logger, valor);

  // El leer_consola() lo comentamos, porque sería si no interactuamos
  // con un servidor, porque la idea es mandarle paquetes.
  //
  /* leer_consola(logger); */

  // Obtenemos valores de la config que son string y los guardamos
  ip = config_get_string_value(config, CLIENT_IP);
  puerto = config_get_string_value(config, CLIENT_PUERTO);

  // Creamos un socket (fd) y establecemos una conexión hacia el servidor
  // (con connect) este debe estar escuchando el puerto que tiene asociado
  // a su ip nosotros cliente debemos esperar que él acepte nuestra
  // conexión (con accept)
  conexion = crear_conexion(ip, puerto);

        // enviamos la clave, que está asignada en "valor" al servidor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

t_log* iniciar_logger(void)
{
	// creamos una instancia de logger
	return log_create("cfg/tp0.log", "tp0", 1, LOG_LEVEL_INFO);
}

t_config* iniciar_config(void)
{
	return config_create(CLIENT_CONFIG);
}

/*
 * Paso (1):
 * Iteramos mientras hayan caracteres, la condición de corte es string vacío
 *
 * Paso (2):
 * Agregamos al registro de logs los caracteres ingresado via terminal
 *
 * Paso (3):
 * liberamos memoria por c/caracter leído, porque por cada readline
 * se aloca espacio en memoria y ésta debe ser liberada luego de usada
 *
 * -> Citamos lo que dice "man readline 2"
 * "The line returned is allocated with malloc(3); the  caller  must  free  it
 * when finished."
 *
 * Paso (4):
 * Leemos el siguiente caracter, misma implementación que leer archivos de AyED
 */
void leer_consola(t_log* logger)
{
	// leemos e primer caracter
	char* leido = leido = readline(">");

	while(strncmp(leido, "", 1) != 0){ // -> (1)
		log_info(logger, leido); // -> (2)
		free(leido); // --> (3)
		leido = readline(">"); // -> (4)
	}

	free(leido); // -> (4) "pero de la primera lectura"
}

/*
 * Paso (1):
 * Agregamos al paquete el contenido, y le sumamos 1 a la longitud del contenido
 * a mandar por el centinela (fin de cadena)
 *
 * Paso (2): Liberamos memoria por cada lectura
 *
 * Paso (3) Leemos el resto de los caracteres ingresados, y los guardamos en "leido"
 *
 * Paso (4): Mandamos el paquete al servidor
 *
 * Paso (5): Eliminamos porque ya no se necesita
 */
void paquete(int conexion)
{
	t_paquete* paquete = crear_paquete();
	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0){
		agregar_a_paquete(paquete, leido, strlen(leido) + 1); // --> (1)
		free(leido); // (2)
		leido = readline(">"); // --> (3)
	}

	free(leido); // --> (2) "pero con la primera lectura"

	enviar_paquete(paquete, conexion); // --> (4)

	eliminar_paquete(paquete); // --> (5)

}

/*
 * Paso (1) (2):
 * Destruimos ambos porque son punteros, y su implementación reserva memoria
 * que debe ser liberada
 *
 * Paso (3):
 * Se cierra la conexión
 */
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_destroy(logger); // --> (1)
	config_destroy(config); // --> (2)
	liberar_conexion(conexion); // --> (3)
}
