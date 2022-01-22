#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------

typedef struct {
  int numero;
  int duracion;
} tarea_t;

// basado en el original, sólo le agregamos el index
struct link_element_with_index {
  int index;
  void *data;
  struct link_element_with_index *next;
};

typedef struct link_element_with_index t_link_element_with_index;

static t_link_element *list_create_element(void *data);
static t_link_element * list_find_element(t_list *self, bool (*cutting_condition)(t_link_element *, int));
static t_link_element_with_index *list_find_element_with_index(t_list *self, bool (*cutting_condition)(t_link_element *, int));
int list_get_element_index(t_list *self, bool (*cutting_condition)(void*, void*), void* target);

static void *list_find_by_value(t_list *self, bool (*condition)(void *, void *), void *data);

tarea_t *crear_tarea(int numero, int duracion);
tarea_t *tarea_mas_facil_entre(tarea_t *tarea_1, tarea_t *tarea_2);
void *imprimir_tarea(tarea_t *tarea);
//------------------------------------------------------------------------------

t_list *tareasDinamicas;

int main() {
  tareasDinamicas = list_create();
  list_add(tareasDinamicas, crear_tarea(10, 5));
  list_add(tareasDinamicas, crear_tarea(13, 3));
  list_add(tareasDinamicas, crear_tarea(11, 30));
  list_add(tareasDinamicas, crear_tarea(12, 35));
  list_add(tareasDinamicas, crear_tarea(14, 2));
  list_add(tareasDinamicas, crear_tarea(15, 45));
  list_add(tareasDinamicas, crear_tarea(16, 1));
  list_add(tareasDinamicas, crear_tarea(17, 1));
  list_add(tareasDinamicas, crear_tarea(18, 1));

  // Tarea_A mas fácil que Tarea_B, si Duracion_A < Duracion_B
  tarea_t *tareaFacil = crear_tarea(11, 4); // el numero de tarea no importa, va a traer la primera menor a esta

  int posicion = list_get_element_index(tareasDinamicas, (void*) tarea_mas_facil_entre, tareaFacil);

  tarea_t *unaTarea;
  unaTarea = (tarea_t *)list_find_by_value(tareasDinamicas, (void *)tarea_mas_facil_entre, tareaFacil);

  if (unaTarea) {
    imprimir_tarea(unaTarea);
  } else {
    printf("la tarea no existe?\n");
  }


  return 0;
}

// --------------------------------------------------
// funciones
tarea_t *crear_tarea(int _numero, int _duracion) {
  tarea_t *tarea = malloc(sizeof(tarea_t));
  tarea->numero = _numero;
  tarea->duracion = _duracion;

  return tarea;
}

tarea_t *tarea_mas_facil_entre(tarea_t *tarea_1, tarea_t *tarea_2) {
  return tarea_1->duracion <= tarea_2->duracion ? tarea_1 : tarea_2;
}

void *imprimir_tarea(tarea_t *tarea) {
  // - si tratamos de imprimir lo que retorna list_find() y éste no encontró
  // nada lanzará un error
  // - si no lo validamos lo anterior, el compilador nos dará el siguiente
  // warning Violación de segmento (core dumped) [Núcleo vaciado a un archivo]
  // if(unaTarea)
  printf("numero:%d, duracion:%d\n", tarea->numero, tarea->duracion);
}

// --------------------------------------------------
// funciones de las commons personalizadas

// función de la commons modificada, se le agregó 2do parámetro

static void* list_find_by_value(t_list *self, bool(*condition)(void*,void*), void* data) {
  t_link_element *new_element = list_create_element(data);
	t_link_element* element;
  t_link_element_with_index *element_with_index;

  int valor = 0;
  bool _find_by_condition(t_link_element* element, int index) {
    valor = index;
		return element == NULL || condition(element->data, new_element->data);
	}

	element = list_find_element(self, _find_by_condition);

  free(new_element); // importante

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

  while (!cutting_condition(element, index)) {
    element = element->next;
    index++;
  }

	return element;
}

// nuevo
static t_link_element_with_index *list_find_element_with_index(t_list *self, bool (*cutting_condition)(t_link_element *, int)) {
  t_link_element *element = self->head;
  t_link_element_with_index *element_with_index;
  int index = 0;

  while (!cutting_condition(element, index)) {
    element = element->next;
    index++;
  }

  element_with_index->index = index;
  element_with_index->data = element->data;

  return element_with_index;
}

int list_get_element_index(t_list *self, bool (*cutting_condition)(void*, void*), void* target) {
  /* t_link_element *element = self->head; */
  t_link_element *element = list_create_element(self->head);
  int index = 0;

  while (!cutting_condition(element, target)) {
    element = element->next;
    index++;
  }

  free(element);

  return index;
}

int list_get_index(t_list *list, bool (*cutting_condition)(void *, void *), void *target) {
  for (int i = 0; i < list_size(list); i++) {
    void *temp = list_get(list, i);
    if (cutting_condition(temp, target)) {
      return i;
    }
  }

  return -1;
}
