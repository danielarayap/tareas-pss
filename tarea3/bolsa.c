#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "bolsa.h"
#define true 1
#define false 0
typedef int boolean;

pthread_mutex_t accion= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond= PTHREAD_COND_INITIALIZER;
char vendedor_actual[10];
char comprador_actual[10];
int num_vend, precio_actual;
boolean compra;

int vendo(int precio, char *vendedor, char *comprador){
	pthread_mutex_lock(&accion); 
	if(!num_vend){ //si no hay vendedores 
		precio_actual = precio; //el nuevo precio es el que introduce este vendedor
		strcpy(vendedor_actual,vendedor); 
		num_vend += 1; 
		while(!compra){ //mientras no haya un comprador, debe esperar
			if(precio > precio_actual){ //si hay un vendedor con un precio menor retorna false
				num_vend -= 1;
				pthread_mutex_unlock(&accion);
				return false;
			}
			pthread_cond_wait(&cond, &accion);
		}
		if(strcmp(vendedor_actual,vendedor) != 0){ //si cambia el nombre del vendedor actual
			strcpy(comprador,comprador_actual); //copiar el nombre del comprador 
			compra = false; 
			pthread_mutex_unlock(&accion); 
			return true;
		}
		num_vend -= 1;
		pthread_mutex_unlock(&accion);
		return false;
	}
	else if(precio_actual > precio){ //si hay vendedores y el precio es mayor al que ofrezco
		precio_actual = precio; //se establece el precio oficial como el mio
		num_vend += 1; 
		strcpy(vendedor_actual, vendedor); 
		pthread_cond_broadcast(&cond); 
		while(!compra){ //mientras no haya un comprador debe esperar
			if(precio > precio_actual){ //si hay un vendedor con precio menor retorna false
				num_vend -= 1;
				pthread_mutex_unlock(&accion);
				return false; 
			}
			pthread_cond_wait(&cond, &accion);
		}
		strcpy(comprador,comprador_actual); //copiar el nombre del comprador en el comprador oficial
		compra = false;
		num_vend = 0;
		pthread_mutex_unlock(&accion);//libera mutex
		return true;
	}
	pthread_mutex_unlock(&accion);
	return false;
}

int compro(char *comprador, char *vendedor){
	pthread_mutex_lock(&accion); 
	if (num_vend != 1) {//si no hay un solo vendedor definido ofreciendo
		pthread_mutex_unlock(&accion);
		return 0;
	}
	compra = true;
	strcpy(comprador_actual,comprador);
	strcpy(vendedor,vendedor_actual);
	int p = precio_actual;
	strcpy(vendedor_actual,"");
	num_vend -= 1;
	pthread_cond_broadcast(&cond); 
	pthread_mutex_unlock(&accion); 
	return p;
}
