#include <stdio.h>
#include <stdlib.h>
#include <commons/collections/list.h>

//------------------------------------------------------------------------------

#define DURACION_TAREA_FACIL 5

typedef struct{
  int numero;
  int duracion;
} tarea_t;

typedef enum {
  FELIZ = 'f',
  NORMAL = 'n',
  TRISTE ='t'
} estado_animo_t;

typedef struct{
  int edad;
  estado_animo_t estado_animo;
} persona_t;

static t_link_element* list_create_element(void* data);
static t_link_element* list_find_element(t_list *self, bool(*cutting_condition)(t_link_element*, int));
static void* list_fold_elements_by_value(t_link_element* element, void* seed, void*(*operation)(void*, void*, void*), void* data);
static void* list_get_maximum_by_value(t_list* self, void* (*operation)(void*, void*,void*), void*data);
static void* list_find_by_value(t_list *self, bool(*condition)(void*,void*), void* data);

tarea_t* crear_tarea(int numero, int duracion);
int get_duracion_tarea(tarea_t* tarea);
void* imprimir_tarea(tarea_t* tarea);
void* liberar_tarea(tarea_t* tarea);
bool es_una_tarea_facil(tarea_t* tarea);
t_list* primeras_tareas_faciles(t_list* tareas);
tarea_t* tarea_mas_facil_entre(tarea_t* tarea_1, tarea_t* tarea_2);
tarea_t* buscar_tarea(t_list* listaTareas, tarea_t* tarea);
/* tarea_t* tarea_mas_facil_entre(void *arg1, void* arg2); */

tarea_t* tarea_mas_facil_segun(tarea_t* tarea_1, tarea_t* tarea_2, int valor); // usa las commons modificadas

void imprimir_persona(persona_t persona){
  printf("edad: %d, estado_animo:%c\n",
         persona.edad, persona.estado_animo);
}

//------------------------------------------------------------------------------
t_list* tareasDinamicas;

int main(){
  persona_t carlos;
  carlos.edad = 5;
  carlos.estado_animo = TRISTE;

  //imprimir_persona(carlos);
  //printf("\n");

  tarea_t tareaA = {.numero=0, .duracion=15};
  tarea_t tareaB = {.numero=1, .duracion=15};

  t_list* tareasEstaticas = list_create();
  // le agregamos el operador de dirección & porque la firma de list_add/2 necesita
  // un puntero a void como segundo parámetro
  list_add(tareasEstaticas, &tareaA);
  list_add(tareasEstaticas, &tareaB);

  // Nota: Esto fallará, porque no se hizo malloc() en cada elemento, son estructuras estáticas
  // list_destroy_and_destroy_elements(tareasDinamicas, liberar_tarea);

  // -----------------------------------------------------------------

  //t_list* tareasDinamicas = list_create();
  tareasDinamicas = list_create();
  list_add(tareasDinamicas, crear_tarea(10, 5));
  list_add(tareasDinamicas, crear_tarea(11, 30));
  list_add(tareasDinamicas, crear_tarea(12, 35));
  list_add(tareasDinamicas, crear_tarea(13, 3));
  list_add(tareasDinamicas, crear_tarea(14, 2));
  list_add(tareasDinamicas, crear_tarea(15, 45));
  list_add(tareasDinamicas, crear_tarea(16, 1));
  list_add(tareasDinamicas, crear_tarea(17, 1));
  list_add(tareasDinamicas, crear_tarea(18, 1));

  tarea_t* unaTarea;

  printf("\nuna tarea facil cualquiera del listado:\n");
  tarea_t* tareaFacil = crear_tarea(10, 5);
  unaTarea = list_find_by_value(tareasDinamicas, tareaFacil);

  if(unaTarea){
    imprimir_tarea(unaTarea);
  }else{
    printf("la tarea no existe?\n");
  }

  // probando cambiar fold1, pasandole un valor para usar en tarea_mas_facil_entre
  printf("\nla tarea más facil del listado:\n");
  int valor = 2;
  unaTarea = (tarea_t*) list_get_maximum_by_value(tareasDinamicas, (void*) tarea_mas_facil_segun, (void*) valor);
  imprimir_tarea(unaTarea);

  /*
  printf("imprimimos cada tarea, una por una\n");
  // obtenemos el primer elemento (index=0)
  unaTarea = (tarea_t*) list_get(tareasDinamicas, 0);
  //imprimir_tarea(unaTarea);

  // obtenemos el segundo elemento (index=1)
  unaTarea = (tarea_t*) list_get(tareasDinamicas, 1);
  //imprimir_tarea(unaTarea);

  printf("\nuna tarea facil cualquiera del listado:\n");
  unaTarea = (tarea_t*) list_find(tareasDinamicas, (void*) es_una_tarea_facil);
  imprimir_tarea(unaTarea);

  //-----------------------------------------------------------------------------------------------------
  printf("\nla tarea más facil del listado:\n");
  unaTarea = (tarea_t*) list_fold1(tareasDinamicas, (void*) tarea_mas_facil_entre);
  imprimir_tarea(unaTarea);

  printf("\nimprimimos algunas tareas faciles:\n");
  t_list* lasPrimerasTareasFaciles = primeras_tareas_faciles(tareasDinamicas);
  // list_iterate(lasPrimerasTareasFaciles, (void*) imprimir_tarea);
  //-----------------------------------------------------------------------------------------------------

  // removemos una tarea según una condición booleana
  //list_remove_and_destroy_by_condition(tareasDinamicas, (void*) es_una_tarea_facil, (void*) liberar_tarea);

  printf("\nimprimimos todas las tareas faciles:\n");
  t_list *tareasFaciles = list_filter(tareasDinamicas, (void*) es_una_tarea_facil);
  list_iterate(tareasFaciles, (void*) imprimir_tarea);

  printf("\nimprimimos la duración de todas las tareas faciles:\n");
  t_list* duracion_de_cada_tarea = list_map(tareasDinamicas, (void*) get_duracion_tarea);
    for(int i=0; i < list_size(duracion_de_cada_tarea); i++){
    int* duracion = (int*) list_get(duracion_de_cada_tarea, i);
    printf("%d ", duracion);
    }
  printf("\n");
  //list_iterate(duracion_de_cada_tarea, (void*) imprimir_tarea);

  // imprimimos todas las tareas
  printf("\nimprimimos todas las tareas:\n");
  // list_iterate(tareasDinamicas, (void*) imprimir_tarea);

  // 1. liberamos memoria de la lista creada porque usa list_create()
  // 2. liberamos memoria de cada elemento suponiendo que son punteros que también alocaron memoria
  list_destroy_and_destroy_elements(tareasDinamicas, (void*) liberar_tarea);

  // es normal que este imprima valores basura
  //imprimir_tarea(unaTarea);
  */
  return 0;
}

//------------------------------------------------------------------------------

tarea_t* crear_tarea(int _numero, int _duracion){
  tarea_t* tarea = malloc(sizeof(tarea_t));
  tarea->numero = _numero;
  tarea->duracion = _duracion;

  return tarea;
}

/*
tarea_t* tarea_mas_facil_entre(void *arg1, void* arg2){
  tarea_t* tarea_1 = (tarea_t*) arg1;
  tarea_t* tarea_2 = (tarea_t*) arg2;
  return tarea_1->duracion < tarea_2->duracion ? tarea_1 : tarea_2;
}
 */

tarea_t* tarea_mas_facil_entre(tarea_t* tarea_1, tarea_t* tarea_2){
  return tarea_1->duracion <= tarea_2->duracion ? tarea_1 : tarea_2;
}

tarea_t* tarea_mas_facil_segun(tarea_t* tarea_1, tarea_t* tarea_2, int valor){
  printf("%d/%d <= %d/%d = %d \n", tarea_1->duracion,valor, tarea_2->duracion,valor, (tarea_1->duracion)/valor <= (tarea_2->duracion)/valor);
  return (tarea_1->duracion)/valor <= (tarea_2->duracion)/valor ? tarea_1 : tarea_2;
  //return (tarea_1->duracion) <= valor+(tarea_2->duracion) ? tarea_1 : tarea_2;
}

int get_duracion_tarea(tarea_t* tarea){
  return tarea->duracion;
}

t_list* primeras_tareas_faciles(t_list* tareas){
  // esta función tiene por objetivo,
  // filtrar por un criterio como el list_filter
  // pero que también compare como el list_fold1 entre dos elementos

  tarea_t* laTareaMasFacil = (tarea_t*) list_fold1(tareas, (void*) tarea_mas_facil_entre);

  bool _es_tarea_facil(tarea_t* tarea) {
    //return tarea_mas_facil_entre(tarea, laTareaMasFacil);
    return tarea->duracion <= laTareaMasFacil->duracion;
  }

  t_list* variasTareasFaciles = list_filter(tareas, (void*) _es_tarea_facil);

  return variasTareasFaciles;
}

bool es_esta_tarea(tarea_t* unaTarea, tarea_t* otraTarea){
  return unaTarea->numero == otraTarea->numero;
}

tarea_t* buscar_tarea(t_list* listaTareas, tarea_t* tarea){
  //bool es_esta_tarea(tarea_t* unaTarea){ return unaTarea->numero == tarea->numero; }
  //tarea_t* unaTarea = (tarea_t*) list_find(listaTareas, (void*) es_esta_tarea);
  tarea_t* unaTarea = (tarea_t*) list_buscar(listaTareas, (void*)es_esta_tarea, tarea);

  return unaTarea;
}

bool es_una_tarea_facil(tarea_t* tarea){
  return tarea->duracion <= DURACION_TAREA_FACIL;
}



void* liberar_tarea(tarea_t* tarea){
  //printf("numero:%d, duracion:%d\n", tarea->numero, tarea->duracion);
  free(tarea);
}

void* imprimir_tarea(tarea_t* tarea){
  // - si tratamos de imprimir lo que retorna list_find() y éste no encontró nada lanzará un error
  // - si no lo validamos lo anterior, el compilador nos dará el siguiente warning
  // Violación de segmento (core dumped) [Núcleo vaciado a un archivo]
  // if(unaTarea)
    printf("numero:%d, duracion:%d\n",
           tarea->numero, tarea->duracion);
}

// --------------------------------------------------
// funciones de las commons personalizadas

// función de la commons modificada, se le agregó 2do parámetro
static void* list_find_by_value(t_list *self, bool(*condition)(void*,void*), void* data) {
  t_link_element *new_element = list_create_element(data);
	t_link_element* element;

	bool _find_by_condition(t_link_element* element, int i) {
		return element == NULL || condition(element->data, new_element->data);
	}
	element = list_find_element(self, _find_by_condition);

	return element != NULL ? element->data : NULL;
}

static t_link_element* list_create_element(void* data) {
	t_link_element* element = malloc(sizeof(t_link_element));
	element->data = data;
	element->next = NULL;
	return element;
}

static t_link_element* list_find_element(t_list *self, bool(*cutting_condition)(t_link_element*, int)) {
	t_link_element* element = self->head;
	int index = 0;

	while(!cutting_condition(element, index)) {
		element = element->next;
		index++;
	}

	return element;
}

// commons modificadas
static void* list_get_maximum_by_value(t_list* self, void* (*operation)(void*, void*,void*), void*data) {
  return list_fold_elements_by_value(self->head->next, self->head->data, operation, data);
}

static void* list_fold_elements_by_value(t_link_element* element, void* seed, void*(*operation)(void*, void*, void*), void* data) {
	void* result = seed;
	while(element != NULL) {
		result = operation(result, element->data, data);
		element = element->next;
	}

	return result;
}
