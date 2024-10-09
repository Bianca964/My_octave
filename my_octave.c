
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define MODULO 10007

int **alocare_memorie_matrice(int nr_linii, int nr_coloane)
{
	int **matrice = (int **)malloc(nr_linii * sizeof(int *));
	if (!matrice) {
		printf("malloc a esuat\n");
		free(matrice);
		exit(0);
	}

	for (int i = 0; i < nr_linii; i++) {
		matrice[i] = (int *)malloc(nr_coloane * sizeof(int));
		if (!matrice[i]) {
			printf("malloc a esuat\n");
			// eliberează memoria pentru liniile deja alocate
			for (int j = 0; j < i; j++)
				free(matrice[j]);
			// eliberează memoria pentru pointerul către linii
			free(matrice);
			exit(0);
		}
	}

	return matrice;
}

void free_matrice(int **matrix, int nr_linii)
{
	for (int i = 0; i < nr_linii; i++)
		free(matrix[i]);
	free(matrix);
}

void citire_si_inserare_matrice(int ****vector_matrici, int *k, int **linii,
								int **coloane, int *size, int m, int n)
{
	// pun liniile(m) si coloanele(n) in vectorii ce retin pt fiecare matrice
	(*linii)[*k] = m;
	(*coloane)[*k] = n;

	// aloc memorie pentru matrice
	int **matrice = alocare_memorie_matrice(m, n);

	// citesc matricea
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			scanf("%d", &matrice[i][j]);
	}

	/* verific daca indexul matricei ce urmeaza sa fie stocata in vector
	depaseste cata memorie am alocat pt el si realoc daca depaseste*/
	if ((*k) + 1 >= *size) {
		(*size) = (*size) * 2;
		(*vector_matrici) = (int ***)realloc((*vector_matrici),
											 (*size) * sizeof(int **));
		if (!(*vector_matrici)) {
			printf("realloc a esuat in citire_si_inserare_matrice\n");
			for (int i = 0; i < (*k) ; i++) {
				for (int j = 0; j < (*linii)[i]; j++)
					free((*vector_matrici)[i][j]);
				free((*vector_matrici)[i]);
			}
			free(*vector_matrici);
			exit(0);
		}
	}

	// introduc matricea in vectorul de matrici
	(*vector_matrici)[*k] = matrice;

	// trec la urmatorul index pt vectorul de matrici
	(*k)++;

	// aloc si vectorului pentru nr liniilor si coloanelor matricilor
	(*linii) = (int *)realloc((*linii), ((*k) + 1) * sizeof(int));
	if (!(*linii)) {
		printf("realloc a esuat in citire_si_inserare_matrice\n");
		free((*linii));
		exit(0);
	}
	(*coloane) = (int *)realloc((*coloane), ((*k) + 1) * sizeof(int));
	if (!(*coloane)) {
		printf("realloc a esuat in citire_si_inserare_matrice\n");
		free((*coloane));
		exit(0);
	}
}

void afisare_matrice(int ***vector_matrici, int index, int *linii,
					 int *coloane, int k)
{
	if (index >= k || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		for (int i = 0; i < linii[index]; i++) {
			for (int j = 0; j < coloane[index]; j++)
				printf("%d ", *(*(*(vector_matrici + index) + i) + j));
			printf("\n");
		}
	}
}

void redimensionare(int ****vector_matrici, int **linii,
					int **coloane, int k)
{
	int l, c, *vec_linii, *vec_coloane, index;
	scanf("%d", &index);

	// citesc liniile
	scanf("%d", &l);
	vec_linii = (int *)malloc(l * sizeof(int));
	if (!vec_linii) {
		printf("malloc a esuat pentru vec_linii\n");
		free(vec_linii);
		exit(0);
	}
	for (int i = 0; i < l; i++)
		scanf("%d", &vec_linii[i]);

	// citesc coloanele
	scanf("%d", &c);
	vec_coloane = (int *)malloc(c * sizeof(int));
	if (!vec_coloane) {
		printf("malloc a esuat pentru vec_coloane\n");
		free(vec_coloane);
		exit(0);
	}
	for (int i = 0; i < c; i++)
		scanf("%d", &vec_coloane[i]);

	// afisare mesaj si eliberare memorie alocata
	if (index >= k || index < 0) {
		printf("No matrix with the given index\n");
		free(vec_linii);
		free(vec_coloane);
	} else {
		// aloc memorie pentru matricea pe care trebuie s o inserez
		int **mat = alocare_memorie_matrice(l, c);

		// construiesc matricea pe care trebuie s o inserez
		for (int i = 0; i < l; i++) {
			for (int j = 0; j < c; j++) {
				// pun intr-o variabila pentru a avea loc pe rand:)
				int x = (*vector_matrici)[index][vec_linii[i]][vec_coloane[j]];
				mat[i][j] = x;
			}
		}

		// eliberez memoria matricii initiale din vector
		free_matrice((*vector_matrici)[index], (*linii)[index]);

		// modific nr liniilor si coloanelor in vectorii aferenti
		(*linii)[index] = l;
		(*coloane)[index] = c;

		// introduc matricea in vectorul de matrici
		(*vector_matrici)[index] = mat;

		// eliberez memoria vectorilor temporari
		free(vec_linii);
		free(vec_coloane);
	}
}

void inmultire_matrici(int ****vector_matrici, int *k, int *size, int **linii,
					   int **coloane)
{
	int index1, index2;

	// citesc indecsii celor 2 matrici
	scanf("%d %d", &index1, &index2);

	// afisare mesaj
	if (index1 >= (*k) || index1 < 0 || index2 >= (*k) || index2 < 0) {
		printf("No matrix with the given index\n");
	} else if ((*coloane)[index1] != (*linii)[index2]) {
		// verific daca dimensiunile sunt nonconforme
		printf("Cannot perform matrix multiplication\n");
	} else {
		int i, j, y;
		// le pun in alte variabile ca sa fie mai usor cu notatiile
		int m = (*linii)[index1];
		int n = (*coloane)[index1];
		int p = (*coloane)[index2];
		int **a = (*vector_matrici)[index1];
		int **b = (*vector_matrici)[index2];

		// aloc memorie pt matricea c care va avea m linii si p coloane
		int **c = alocare_memorie_matrice(m, p);

		// calculez matricea c (rezultatul inmultirii matricilor a si b)
		for (i = 0; i < m; i++) {
			for (y = 0; y < p; y++) {
				long long s = 0;
				for (j = 0; j < n; j++)
					s = s + a[i][j] * b[j][y];
				s = s % MODULO;
				while (s < 0)
					s = s + MODULO;
				c[i][y] = s;
			}
		}
		/*verific daca indexul matricei ce urmeaza sa fie stocata in vector
		depaseste cata memorie am alocat pt el si realoc daca depaseste*/
		if ((*k) + 1 >= (*size)) {
			(*size) = (*size) * 2;
			(*vector_matrici) = (int ***)realloc((*vector_matrici),
												 (*size) * sizeof(int **));
			if (!(*vector_matrici)) {
				printf("realloc a esuat in inmultire_matrici\n");
				for (int i = 0; i < (*k) ; i++) {
					for (int j = 0; j < (*linii)[i]; j++)
						free((*vector_matrici)[i][j]);
					free((*vector_matrici)[i]);
				}
				free(*vector_matrici);
				exit(0);
			}
		}
		// insertez matricea c in vectorul de matrici la final
		(*vector_matrici)[*k] = c;

		// cresc indexul k
		(*k)++;

		//aloc o noua memorie pt vectorii de linii si coloane
		(*linii) = (int *)realloc((*linii), ((*k) + 1) * sizeof(int));
		if (!(*linii)) {
			printf("realloc a esuat in inmultire_matrici\n");
			free(*linii);
			exit(0);
		}
		(*coloane) = (int *)realloc((*coloane), ((*k) + 1) * sizeof(int));
		if (!(*coloane)) {
			printf("realloc a esuat in inmultire_matrici\n");
			free(*coloane);
			exit(0);
		}

		//pun in vectorii de linii si coloane noile valori
		(*linii)[(*k) - 1] = (*linii)[index1];
		(*coloane)[(*k) - 1] = (*coloane)[index2];
	}
}

int suma_elem_matrice(int **matrice_din_vector, int m, int n)
{
	long long suma = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			suma = suma + matrice_din_vector[i][j];
	}
	suma = suma % MODULO;
	while (suma < 0)
		suma = suma + MODULO;
	return suma;
}

void sortare_matrici(int *k, int ****vector_matrici, int **linii,
					 int **coloane)
{
	for (int i = 0; i < (*k) - 1; i++) {
		for (int j = i + 1; j < (*k); j++) {
			/* daca urmatoarea matrice are suma mai mica decat cea din spate,
			se interschimba*/
			int s1 = suma_elem_matrice((*vector_matrici)[j], (*linii)[j],
									   (*coloane)[j]);
			int s2 = suma_elem_matrice((*vector_matrici)[i], (*linii)[i],
									   (*coloane)[i]);
			if (s1 < s2) {
				// interschimb matricile in vectorul de matrici
				int **aux = (*vector_matrici)[i];
				(*vector_matrici)[i] = (*vector_matrici)[j];
				(*vector_matrici)[j] = aux;

				// interschimb liniile pt matricile i si j
				int aux_linii = (*linii)[i];
				(*linii)[i] = (*linii)[j];
				(*linii)[j] = aux_linii;

				// interschimb coloanele pt matricile i si j
				int aux_coloane = (*coloane)[i];
				(*coloane)[i] = (*coloane)[j];
				(*coloane)[j] = aux_coloane;
			}
		}
	}
}

void transpusa(int ****vector_matrici, int *index, int **linii, int **coloane,
			   int k)
{
	if ((*index) >= k || (*index) < 0) {
		printf("No matrix with the given index\n");
	} else {
		// aloc memorie pt matricea in care pun matricea transpusa
		int **matrice = alocare_memorie_matrice((*coloane)[*index],
												(*linii)[*index]);

		// construiesc transpusa in matricea auxiliara
		for (int i = 0; i < (*coloane)[*index]; i++) {
			for (int j = 0; j < (*linii)[*index]; j++)
				matrice[i][j] = (*vector_matrici)[*index][j][i];
		}

		// modific in vectorul de linii si coloane noile valori
		int aux = (*linii)[*index];
		(*linii)[*index] = (*coloane)[*index];
		(*coloane)[*index] = aux;

		// eliberez matricea veche de la indexul index
		free_matrice((*vector_matrici)[*index], (*coloane)[*index]);

		// adaug matricea in vectorul de matrici
		(*vector_matrici)[*index] = matrice;
	}
}

//inmultirea matricilor patratice a si b cu n linii si n coloane
void inmultire_matrice_pentru_ridicare_putere(int n, int ***a, int **b)
{
	// aloc memorie pentru matricea in care va fi rezulatutl
	int **rez_inm = alocare_memorie_matrice(n, n);

	// calculez matricea rezultata
	int i, j, k;
	for (i = 0; i < n; i++) {
		for (k = 0; k < n; k++) {
			long long s = 0;
			for (j = 0; j < n; j++)
				s = s + (*a)[i][j] * b[j][k];
			s = s % MODULO;
			while (s < 0)
				s = s + MODULO;
			rez_inm[i][k] = s;
		}
	}

	// pun rezulatul in matricea pe care o modific
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			(*a)[i][j] = rez_inm[i][j];
	}

	// eliberez memoria pentru matricea rez_inm
	free_matrice(rez_inm, n);
}

void ridicare_la_putere(int ****vector_matrici, int *linii, int *coloane,
						int k)
{
	int index;
	long long power;

	// citesc index si puterea
	scanf("%d", &index);
	scanf("%lld", &power);

	// afisare mesaj
	if (index >= k || index < 0) {
		printf("No matrix with the given index\n");
	} else if (power < 0) {
		printf("Power should be positive\n");
	} else if (linii[index] != coloane[index]) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		// folosesc alte varibile pentru usurinta
		int nr_linii = linii[index];
		int **a = (*vector_matrici)[index];

		// aloc memorie pentru matricea noua
		int **matrice_rez = alocare_memorie_matrice(nr_linii, nr_linii);

		// initializez matricea noua cu In(element neutru la inmultire)
		for (int i = 0; i < nr_linii; i++) {
			for (int j = 0; j < nr_linii; j++) {
				if (i == j)
					matrice_rez[i][j] = 1;
				else
					matrice_rez[i][j] = 0;
			}
		}

		// algoritmul pentru ridicare la putere
		while (power > 0) {
			if (power % 2 == 1) {
				/* daca power este impar, matricea noua devine rezultatul
				inmultirii lui 'matrice' cu matricea pe care vreau s-o
				ridic la putere*/
				inmultire_matrice_pentru_ridicare_putere(nr_linii,
														 &matrice_rez, a);
			}
			// ridic matricea 'a' la a doua
			inmultire_matrice_pentru_ridicare_putere(nr_linii, &a, a);
			power = power / 2;
		}

		// eliberez memoria matricei vechi din vectorul de matrici
		free_matrice((*vector_matrici)[index], nr_linii);

		// pun matricea noua in locul celeilalte
		(*vector_matrici)[index] = matrice_rez;
	}
}

void eliminare_matrice(int ****vector_matrici, int *k, int index, int **linii,
					   int **coloane, int *size)
{
	if (index >= *k || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		// eliberez memoria pentru matricea index
		free_matrice((*vector_matrici)[index], (*linii)[index]);

		// mut fiecare matrice cu un index mai in spate
		for (int i = index; i < *k - 1; i++) {
			(*vector_matrici)[i] = (*vector_matrici)[i + 1];
			(*linii)[i] = (*linii)[i + 1];
			(*coloane)[i] = (*coloane)[i + 1];
		}

		// scad indexul cu 1
		(*k)--;

		// eliberez o parte din memorie daca e cazul
		if ((*k) <= ((*size) / 2)) {
			(*size) = (*size) / 2;

			//fortez ca in vectorul de matrici sa am cel putin un spatiu alocat
			if ((*size) < 1)
				(*size) = 1;

			// injumatatire memorie pentru vectorul de matrici
			(*vector_matrici) = (int ***)realloc(*vector_matrici,
												(*size) * sizeof(int **));
			if (!(*vector_matrici)) {
				printf("realloc a esuat in eliminare_matrice\n");
				for (int i = 0; i < (*k) ; i++) {
					for (int j = 0; j < (*linii)[i]; j++)
						free((*vector_matrici)[i][j]);
					free((*vector_matrici)[i]);
				}
				free(*vector_matrici);
				exit(0);
			}

			// injumatatire memorie pentru vectorul liniilor
			(*linii) = (int *)realloc((*linii), (*size) * sizeof(int));
			if (!(*linii)) {
				printf("realloc linii a esuat in eliminare_matrice\n");
				free((*linii));
				exit(0);
			}

			// injumatatire memorie pentru vectorul coloanelor
			(*coloane) = (int *)realloc((*coloane), (*size) * sizeof(int));
			if (!(*coloane)) {
				printf("realloc coloane a esuat in eliminare_matrice\n");
				free((*coloane));
				exit(0);
			}
		}
	}
}

void adunare_matrici(int n, int **a, int **b, int ***c)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			(*c)[i][j] = 0;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			(*c)[i][j] = a[i][j] + b[i][j];
			(*c)[i][j] = (*c)[i][j] % MODULO;
			while ((*c)[i][j] < 0)
				(*c)[i][j] = (*c)[i][j] + MODULO;
		}
	}
}

void scadere_matrici(int n, int **a, int **b, int ***c)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			(*c)[i][j] = 0;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			(*c)[i][j] = a[i][j] - b[i][j];
			(*c)[i][j] = (*c)[i][j] % MODULO;
			while ((*c)[i][j] < 0)
				(*c)[i][j] = (*c)[i][j] + MODULO;
		}
	}
}

void eliberare_matrici_strassen(int **a11, int **a12, int **a21, int **a22,
								int **b11, int **b12, int **b21, int **b22,
								int **c11, int **c12, int **c21, int **c22,
								int **p1, int **p2, int **p3, int **p4,
								int **p5, int **p6, int **p7, int **temp1,
								int **temp2, int newsize)
{
	//dealocare memorie pentru submatricile a
	free_matrice(a11, newsize);
	free_matrice(a12, newsize);
	free_matrice(a21, newsize);
	free_matrice(a22, newsize);

	//dealocare memorie pentru submatricile b
	free_matrice(b11, newsize);
	free_matrice(b12, newsize);
	free_matrice(b21, newsize);
	free_matrice(b22, newsize);

	//dealocare memorie pentru submatricile c
	free_matrice(c11, newsize);
	free_matrice(c12, newsize);
	free_matrice(c21, newsize);
	free_matrice(c22, newsize);

	//dealocare pentru temp1, temp2
	free_matrice(temp1, newsize);
	free_matrice(temp2, newsize);

	//dealocare mmeorie pentru matricile auxiliare p
	free_matrice(p1, newsize);
	free_matrice(p2, newsize);
	free_matrice(p3, newsize);
	free_matrice(p4, newsize);
	free_matrice(p5, newsize);
	free_matrice(p6, newsize);
	free_matrice(p7, newsize);
}

void alocare_mem_matrici_strassen(int ***a11, int ***a12, int ***a21,
								  int ***a22, int ***b11, int ***b12,
								  int ***b21, int ***b22, int ***c11,
								  int ***c12, int ***c21, int ***c22,
								  int ***temp1, int ***temp2, int newsize)
{
	(*a11) = alocare_memorie_matrice(newsize, newsize);
	(*a12) = alocare_memorie_matrice(newsize, newsize);
	(*a21) = alocare_memorie_matrice(newsize, newsize);
	(*a22) = alocare_memorie_matrice(newsize, newsize);
	(*b11) = alocare_memorie_matrice(newsize, newsize);
	(*b12) = alocare_memorie_matrice(newsize, newsize);
	(*b21) = alocare_memorie_matrice(newsize, newsize);
	(*b22) = alocare_memorie_matrice(newsize, newsize);
	(*c11) = alocare_memorie_matrice(newsize, newsize);
	(*c12) = alocare_memorie_matrice(newsize, newsize);
	(*c21) = alocare_memorie_matrice(newsize, newsize);
	(*c22) = alocare_memorie_matrice(newsize, newsize);
	(*temp1) = alocare_memorie_matrice(newsize, newsize);
	(*temp2) = alocare_memorie_matrice(newsize, newsize);
}

void strassen(int n, int **a, int **b, int ***c)
{
	if (n == 2) {
		// cazul de baza cand inmultesc matricile normal
		(*c) = alocare_memorie_matrice(n, n);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				(*c)[i][j] = 0;
				for (int k = 0; k < n; k++)
					(*c)[i][j] = (*c)[i][j] + a[i][k] * b[k][j];
				(*c)[i][j] = (*c)[i][j] % MODULO;
				while ((*c)[i][j] < 0)
					(*c)[i][j] = (*c)[i][j] + MODULO;
			}
		}
	} else {
		int newsize = n / 2;
		int **a11, **a12, **a21, **a22, **b11, **b12, **b21, **b22, **c11,
			**c12, **c21, **c22, **p1, **p2, **p3, **p4, **p5, **p6, **p7,
			**temp1, **temp2;
		// alocare memorie pentru submatrici
		alocare_mem_matrici_strassen(&a11, &a12, &a21, &a22, &b11, &b12, &b21,
									 &b22, &c11, &c12, &c21, &c22, &temp1,
									 &temp2, newsize);
		// impart cele 2 matrici in submatrici
		for (int i = 0; i < newsize; i++) {
			for (int j = 0; j < newsize; j++) {
				a11[i][j] = a[i][j];
				a12[i][j] = a[i][j + newsize];
				a21[i][j] = a[i + newsize][j];
				a22[i][j] = a[i + newsize][j + newsize];
				b11[i][j] = b[i][j];
				b12[i][j] = b[i][j + newsize];
				b21[i][j] = b[i + newsize][j];
				b22[i][j] = b[i + newsize][j + newsize];
			}
		}
		// calculez submatricile p1 -> p7
		adunare_matrici(newsize, a11, a22, &temp1);
		adunare_matrici(newsize, b11, b22, &temp2);
		strassen(newsize, temp1, temp2, &p1);
		adunare_matrici(newsize, a21, a22, &temp1);
		strassen(newsize, temp1, b11, &p2);
		scadere_matrici(newsize, b12, b22, &temp1);
		strassen(newsize, a11, temp1, &p3);
		scadere_matrici(newsize, b21, b11, &temp1);
		strassen(newsize, a22, temp1, &p4);
		adunare_matrici(newsize, a11, a12, &temp1);
		strassen(newsize, temp1, b22, &p5);
		scadere_matrici(newsize, a21, a11, &temp1);
		adunare_matrici(newsize, b11, b12, &temp2);
		strassen(newsize, temp1, temp2, &p6);
		scadere_matrici(newsize, a12, a22, &temp1);
		adunare_matrici(newsize, b21, b22, &temp2);
		strassen(newsize, temp1, temp2, &p7);
		// calculez submatricile lui c
		adunare_matrici(newsize, p1, p4, &temp1);
		scadere_matrici(newsize, temp1, p5, &temp2);
		adunare_matrici(newsize, temp2, p7, &c11);
		adunare_matrici(newsize, p3, p5, &c12);
		adunare_matrici(newsize, p2, p4, &c21);
		adunare_matrici(newsize, p1, p3, &temp1);
		scadere_matrici(newsize, temp1, p2, &temp2);
		adunare_matrici(newsize, temp2, p6, &c22);
		// combin matricile in matricea rezultata c
		(*c) = alocare_memorie_matrice(n, n);
		for (int i = 0; i < newsize; i++) {
			for (int j = 0; j < newsize; j++) {
				(*c)[i][j] = c11[i][j];
				(*c)[i][j + newsize] = c12[i][j];
				(*c)[i + newsize][j] = c21[i][j];
				(*c)[i + newsize][j + newsize] = c22[i][j];
			}
		}
		// dealoc memoria pentru toate matricile auxiliare
		eliberare_matrici_strassen(a11, a12, a21, a22, b11, b12, b21, b22,
								   c11, c12, c21, c22, p1, p2, p3, p4, p5,
								   p6, p7, temp1, temp2, newsize);
	}
}

void inserare_matrice_in_vector(int ****vector_matrici, int **rez_strassen,
								int *size, int *k, int **linii, int **coloane,
								int index1, int index2)
{
	/* verific daca indexul matricei ce urmeaza sa fie stocata in vector
	depaseste cata memorie am alocat pt el si realoc daca depaseste*/
	if ((*k) + 1 >= (*size)) {
		(*size) = (*size) * 2;
		(*vector_matrici) = (int ***)realloc((*vector_matrici),
											 (*size) * sizeof(int **));
		if (!(*vector_matrici)) {
			printf("realloc a esuat in inserare_matrice_in_vector\n");
			for (int i = 0; i < (*k) ; i++) {
				for (int j = 0; j < (*linii)[i]; j++)
					free((*vector_matrici)[i][j]);
				free((*vector_matrici)[i]);
			}
			free(*vector_matrici);
			exit(0);
		}
	}

	// inserez matricea rez_strassen in vectorul de matrici la final
	(*vector_matrici)[*k] = rez_strassen;

	// aloc o noua memorie pentru vectorii de linii si coloane
	(*linii) = (int *)realloc((*linii), ((*k) + 1) * sizeof(int));
	if (!(*linii)) {
		printf("realloc a esuat in inserare_matrice_in_vector\n");
		free((*linii));
		exit(0);
	}
	(*coloane) = (int *)realloc((*coloane), ((*k) + 1) * sizeof(int));
	if (!(*coloane)) {
		printf("realloc a esuat in inserare_matrice_in_vector\n");
		free((*coloane));
		exit(0);
	}

	// pun in vectorul de linii si coloane noile valori
	(*linii)[*k] = (*linii)[index1];
	(*coloane)[*k] = (*coloane)[index2];

	// cresc indexul k
	(*k)++;
}

void eliberare_finala(int ****vector_matrici, int **linii, int **coloane,
					  int k)
{
	for (int i = 0; i < k ; i++) {
		for (int j = 0; j < (*linii)[i]; j++)
			free((*vector_matrici)[i][j]);
		free((*vector_matrici)[i]);
	}
	free(*vector_matrici);
	free(*linii);
	free(*coloane);
	exit(0);
}

int main(void)
{
	int m, n, k = 0, index, index1, index2, size = 1, **rez_strassen;
	int *linii, *coloane, ***vector_matrici;
	char litera;

	scanf("%s", &litera);

	// aloc memorie pt vector_matrici si vectorii liniilor si coloanelor
	vector_matrici = (int ***)malloc(size * sizeof(int **));
	linii = (int *)malloc((k + 1) * sizeof(int));
	coloane = (int *)malloc((k + 1) * sizeof(int));

	// daca citeste o litera diferita de Q fac comanda aferenta literei
	while (litera != 'Q') {
		switch (litera) {
		case 'L':
			scanf("%d %d", &m, &n);
			citire_si_inserare_matrice(&vector_matrici, &k, &linii, &coloane,
									   &size, m, n);
			break;
		case 'D':
			scanf("%d", &index);
			if (index >= k || index < 0) {
				printf("No matrix with the given index\n");
				break;
			}
			printf("%d %d\n", linii[index], coloane[index]);
			break;
		case 'P':
			scanf("%d", &index);
			afisare_matrice(vector_matrici, index, linii, coloane, k);
			break;
		case 'C':
			redimensionare(&vector_matrici, &linii, &coloane, k);
			break;
		case 'M':
			inmultire_matrici(&vector_matrici, &k, &size, &linii, &coloane);
			break;
		case 'O':
			sortare_matrici(&k, &vector_matrici, &linii, &coloane);
			break;
		case 'T':
			scanf("%d", &index);
			transpusa(&vector_matrici, &index, &linii, &coloane, k);
			break;
		case 'R':
			ridicare_la_putere(&vector_matrici, linii, coloane, k);
			break;
		case 'F':
			scanf("%d", &index);
			eliminare_matrice(&vector_matrici, &k, index, &linii, &coloane,
							  &size);
			break;
		case 'S':
			scanf("%d %d", &index1, &index2);
			if (index1 >= k || index1 < 0 || index2 >= k || index2 < 0) {
				printf("No matrix with the given index\n");
				break;
			}
			// verific daca dimensiunile sunt nonconforme
			if (coloane[index1] != linii[index2]) {
				printf("Cannot perform matrix multiplication\n");
				break;
			}
			strassen(linii[index1], vector_matrici[index1],
					 vector_matrici[index2], &rez_strassen);
			inserare_matrice_in_vector(&vector_matrici, rez_strassen, &size,
									   &k, &linii, &coloane, index1, index2);
			break;
		default:
			printf("Unrecognized command\n");
		}
		scanf("%s", &litera);
	}
	// cand citeste Q, eliberez toata memoria
	if (litera == 'Q')
		eliberare_finala(&vector_matrici, &linii, &coloane, k);

	return 0;
}
