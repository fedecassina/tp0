#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */
	
	logger = iniciar_logger();

	log_info(logger,"Hola! Soy un log");
	
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	char *current_dir = getcwd(NULL, 0);
    printf("El directorio actual es %s\n", current_dir);
    free(current_dir);

	config = iniciar_config();

	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "IP: %s, PUERTO: %s, VALOR: %s \n", ip,puerto,valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	printf("\n CLIENTE CERRADO");
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","TP0",1,LOG_LEVEL_INFO);
	
	if(nuevo_logger == NULL){
		perror("No se pudo crear el logger!");
		exit(EXIT_FAILURE);
	}
	
	return nuevo_logger;
	
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");

	if(nuevo_config == NULL){
		perror("No pude leer la config\n");
		exit(EXIT_FAILURE);
	}
	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, ">> %s", leido);
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	
	while(strcmp(leido, "") != 0){
		free(leido);
		leido = readline("> ");
		log_info(logger, ">> %s", leido);
		
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete

	while(strlen(leido) != 0){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free(leido);
		leido = readline("> ");	
	}
	
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	//Enviar paquete al servidor
	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
	free(leido);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
