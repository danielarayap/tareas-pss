#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void consultar(char *nom_dic, char *pal) {	
	FILE *dic = fopen(nom_dic, "r");//abre el archivo en modo lectura
	if (dic == NULL){
		perror("\nfopen() error\n");
		exit(1);
	}//chequear que no haya errores en la apertura del archivo
	char linea[100];
	char *llave,*fin_llave, *def, *fin_def;
	int izq, der;
	size_t rc;
	while ((rc = fread(linea, 100, 1, dic)) == 1) {
		llave = fin_llave = linea + 20;
		def = fin_def = linea + 40;
		izq = atoi(linea);
		der = atoi(linea + 10);
		while (*fin_llave != ' ') fin_llave++; //encuentra el final del string que contiene la palabra 
		*fin_llave = 0; 
		while (*fin_def != ' ' || *(fin_def + 1) != ' ') fin_def++; //encuentra el final del string que contiene el significado
		*fin_def = 0;
		if (strcmp(pal, llave) == 0) { 
			printf("%s\n", def);
			fclose(dic);
			exit(0);
		}
		else if (strcmp(pal, llave) > 0) {
			if (der == -1) break; //si la palabra a buscar es mayor lexicograficamente que la palabra en la linea del archivo pero no existe un hijo der, se sale del ciclo
			fseek(dic, der * 100, SEEK_SET); //mueve el indicador de posicion a la linea der
		}
		else if (strcmp(pal, llave) < 0) {
			if (izq == -1) break; //si la palabra a buscar es menor lexicograficamente que la palabra en la linea del archivo pero no existe un hijo izq, se sale del ciclo
			fseek(dic, izq * 100, SEEK_SET);//mueve el indicador de posicion a la linea izq		
		}
	}
	printf("palabra no encontrada\n");
	fclose(dic);
	exit(0);
}


int main(int argc, char **argv) {
	if (argc!=3){
		fprintf(stderr, "uso: %s <nombre de archivo> <palabra a consultar en archivo>\n", argv[0]); //para indicar que se admiten solo dos parametros aparte de la llamada a consultar
		exit(1);
	}
	char *nom_dic = argv[1];
	char *pal = argv[2];
	consultar(nom_dic, pal);
	return 0;
}

//Alumno: Daniel Araya Poblete

