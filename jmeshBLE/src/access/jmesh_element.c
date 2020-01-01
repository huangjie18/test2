#include"jmesh_element.h"
#include"jmesh_model.h"
#include"stdio.h"
#include"../jmesh/jmesh_print.h"

void jmesh_element_add_model(jmesh_element_t *element,jmesh_model_t* model)
{
    int i=0;
    jmesh_model_t* t;
    if(element==NULL||model==NULL){
        JMESH_LOGE(1,"add model","para cannot be null\n");
        return;
    }
    t=element->model_list;
    while(t!=NULL&&t!=model){
        t=t->next;
        i++;
    }
    if(t==NULL){
        if(i>=JMESH_ELEMENT_REGISTER_MODEL_MAX_NUM){
            JMESH_LOGE(1,"add model","the element add too many models\n");
        }
        model->next=element->model_list;
        element->model_list=model;
        model->element=element;
    }
}

int jmesh_list_model_id(jmesh_element_t* element,unsigned char* buffer)
{
		jmesh_model_t* model = element->model_list;
		unsigned char len = 0;
		while(model != NULL)
		{
				if(model->info->model_id.model_id == JMESH_MODEL_INVALID_ID)
				{
						JMESH_LITTLE_ENDIAN_FILL2(model->info->model_id.sig_id,buffer+len);
				}else{
						JMESH_LITTLE_ENDIAN_FILL2(model->info->model_id.model_id,buffer+len);
				}
				len+=2;
				model = model->next;
		}
		return len;
}
