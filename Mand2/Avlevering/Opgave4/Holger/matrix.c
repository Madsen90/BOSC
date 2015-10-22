#include "matrix.h"
#include <stdlib.h>
#include <stdio.h>

void allocate_matrix_int(int*** mat, int m, int n){
	//Memory allocation for matrix
	
	*mat = malloc(m * sizeof(int *));
	
	int i;
	for(i = 0; i < m; ++i){
		(*mat)[i] = malloc(n * sizeof(int));
	}

	return;
}

void free_matrix_int(int*** mat, int m){
	int i;
	for(i = 0; i < m; ++i){
		free(**mat);
	}

	free(*mat);
}

