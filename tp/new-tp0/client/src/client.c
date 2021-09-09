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

	// Usando el config creado previamente
	// Lee las variables de IP, Puerto y Valor
	// leer_consola(logger); // <-- NOPE, haces doble enter amewo

	// Creamos una conexión hacia el servidor
	ip = config_get_string_value(config, CLIENT_IP);
	puerto = config_get_string_value(config, CLIENT_PUERTO);
	conexion = crear_conexion(ip, puerto);
  //configurar_conexion(ip, puerto, conexion);

	// enviamos la clave, que está asignada en "valor" al servidor
	enviar_mensaje(valor, conexion);

	paquete(conexion);

	terminar_programa(conexion, logger, config);
}

void configuar_conexion(char* ip, char* puerto, int conexion){
}

t_log* iniciar_logger(void)
{
	// creamos una instancia de logger
	return log_create("tp0.log", "tp0", 1, LOG_LEVEL_INFO);
}

t_config* iniciar_config(void)
{
	return config_create(CLIENT_CONFIG);
}

void leer_consola(t_log* logger)
{
	// leemos e primer caracter
	char* leido = leido = readline(">");

	// -> iteramos mientras hayan caracteres
	// -> la condición de corte es string vacío
	while(strncmp(leido, "", 1) != 0){
		// agregamos al registro de logs los caracteres ingresado via terminal
		log_info(logger, leido);
		// liberamos memoria por c/caracter leído
		free(leido);
		// leemos el siguiente caracter, misma implementación que leer archivos de ayed
		leido = readline(">");
	}

	// liberamos memoria de primer caracter leído
	free(leido);
}

void paquete(int conexion)
{
	t_paquete* paquete = crear_paquete();
	char* leido = readline(">");

	while(strncmp(leido, "", 1) != 0){
		// agregamos al paquete el contenido,
		// y le sumamos 1 a la longitud del contenido a
    // mandar por el centinela (fin de cadena)
		agregar_a_paquete(paquete, leido, strlen(leido) + 1);
		// liberamos memoria por cada lectura
		free(leido);
		// leemos el resto de los caracteres ingresados, y los guardamos en "leido"
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
	// destruimos la instancia de logger que creamos al principio
	log_destroy(logger);
	// destruimos el config (es un puntero)
	config_destroy(config);

	liberar_conexion(conexion);
}
