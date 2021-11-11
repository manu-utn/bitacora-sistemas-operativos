bool es_este_dispositivo_io(t_recurso_io* recursoIO, t_recurso_io* otroRecursoIO){
  return string_equals_ignore_case(recursoIO->nombre, otroRecursoIO->nombre);
    }

void encolar_pcb_a_dispositivo_io(t_pcb* pcb, t_call_io* callIO) {
  t_recurso_io* recursoIO = list_find2(dispositivosIODelSistema->listaRecursos, (void*) es_este_dispositivo_io, callIO);
}

void* list_find2(t_list *self, bool(*condition)(void*,void*), void* data) {
  t_link_element *new_element = list_create_element(data);
	t_link_element* element;

	bool _find_by_condition(t_link_element* element, int i) {
		return element == NULL || condition(element->data, new_element->data);
	}
	element = list_find_element(self, _find_by_condition);

	return element != NULL ? element->data : NULL;
}

t_link_element* list_create_element(void* data) {
	t_link_element* element = malloc(sizeof(t_link_element));
	element->data = data;
	element->next = NULL;
	return element;
}

t_link_element* list_find_element(t_list *self, bool(*cutting_condition)(t_link_element*, int)) {
	t_link_element* element = self->head;
	int index = 0;

	while(!cutting_condition(element, index)) {
		element = element->next;
		index++;
	}

	return element;
}
