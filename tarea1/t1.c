#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "t1.h"

/* función bitMasSignificativo visto en Auxiliar 2: Operaciones con bits (P4) */
/* Retorna la posicion del bit mas significativo de un entero de 32 bits sin
 * signo, utilizando búsqueda binaria.
 */
int bitMasSignificativo(uint x){
	int i = 31, j = 0;
	int resp = -1;
	while (i < j){
		int k = (i + j + 1) / 2;
		int mask = 0xffffffff << k;
		if (x & mask)
			j = k;
		else
			i = k - 1;
	}
	if (x & (1 << i))
		return i;
	return resp;
}

uint insertar_bits(uint x, int pos, uint y, int len) {
	uint aux = x << len;
	uint aux2 = y << pos;
	uint aux3;
	if (pos == 0) 
		return aux + y;
	if (bitMasSignificativo(x) < pos) 
		return x + aux2;
	uint mask_1 = (-1) << (pos + len);
	uint mask_2 = ~(-1 << pos);
	aux = aux & mask_1;
	aux3 = x & mask_2;
	return aux + aux2 + aux3;
}

void eliminar_espacios(char *s) {
	if (!*s) return;
	char * dup = s;
	char * nodup = s;
	dup++;
	nodup++;
	while (*dup){
		if (*dup != ' ') 
			*nodup++ = *dup;
		else if (*(nodup-1) != *dup) 
			*nodup++ = *dup;
		dup++;
	}
	*nodup = 0;
}
