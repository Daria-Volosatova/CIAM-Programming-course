#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<malloc.h>
#include<errno.h>
#include<string.h>
extern int errno;

// функция, выводящая массив
void array_output(double** arr, size_t n, size_t m) {
	for(int i = 0; i < n; i++) {
		for(int j = 0 ; j < m; j++) {
			printf("%10.3lf ", arr[i][j]);
		}
		printf("\n");
	}
}

//значение аппроксимируещего полинома в точке
double ap_pol(double x, double* p, size_t n) {
	double result = 0;
	for(int i = 0; i < n; i++ ) {
		result += pow(x, i)*(p[i]);
	}
	return result;
}

//нахождение главного элемента столбца обмен с текущем элементом (для метода Гаусса)
double main_element_reverse_row(size_t k, double** arr, size_t n) {
	int i_max = k;
	double temp;
	for (int i = k; i < n; i++)
			if (fabs( arr[i_max][k] ) < fabs(arr[i] [k]))
			{
				i_max = i;
			}
	for (int j = k; j < n + 1; j++) {
		temp = arr[k][j];
		arr[k][j] = arr[i_max][j];
		arr[i_max][j] = temp;
    }
}

int main() {
	
//считываем данные (координаты точек) и записываем их в двумерный динамический массив	
FILE *data;
double *x;
double *y;
x = NULL;
y = NULL;
if((data = fopen("hs.txt", "r")) == NULL){
	printf("Can't open\n");
	return(0);
}

int count = 0;

for(int i = 0; ; i++) {
	count ++;
	x = (double*)realloc(x, count * sizeof(double));
	y = (double*)realloc(y, count * sizeof(double));
	if((fscanf(data, "%lf", &x[i]) == 1) && ((fscanf(data, "%lf", &y[i]) == 1))){
	} 
	else {
		break;
	}
}

fclose(data);

count -= 1;

//вводим порядок аппроксимации
printf(" \nEnter order of approximation:\n");
int n;
scanf("%d", &n);
double** arr = malloc((n + 1) * sizeof(double*));

double* t = malloc ((n + 1) * (n + 2) * sizeof(double));

for (size_t i = 0; i < n + 1; ++i) {
	arr[i] = t + i * (n + 2);
}

//заполняем матрицу коэффициентами линейной системы уравнений, сформированной в соответствии с формулами метода наименьших квадратов
for (int i = 0; i < n + 1; i++) {
	for (int j = 0; j < n + 2; j++) {
		if (j == n + 1) {
			arr[i][j] = 0;
			for (int k =0; k < count;  k++) {
				arr[i][j] += pow(x[k], i) * y[k];
			}
		} else{
			arr[i][j] = 0;
			for(int k = 0; k < count; k++) {
				arr[i][j] += pow(x[k], i + j);
			}
		}
	}
}


//решаем систему уравнений методом Гаусса
int max_n;
double max, a;
double mult;

for (int k = 0; k < n + 1; k++) {
	main_element_reverse_row(k, arr, n + 1);
	for (int i = k + 1; i < n + 1; i++) {
		mult = arr[i][k]/arr[k][k];
		for (int j = n + 2; j >= k; j--) {
			arr[i][j] -= arr[k][j] * mult;
		}
	}
}

double p[n + 1];

for (int i = n ; i > 0; i--) {
	for (int j = i - 1; j >= 0; j--) {
		mult = arr[j][i]/arr[i][i];
		for (int k = 0; k < n + 2; k++) {
			arr[j][k] -= arr[i][k] * mult;
		}
	}
}

for(int i = 0; i < n + 1; i ++) {
	p[i] = arr[i][n + 1]/arr[i][i];
}

if((data = fopen("results.csv", "w")) == NULL){
	printf("Can't open: %s\n", strerror(errno));
	return(0);
}

//записываем результаты в файл
for(int i = 0; i < count; i++) {
	fprintf(data, "%10.3lf,%10.3lf,%10.3lf\n", x[i], y[i], ap_pol(x[i], p, n + 1));
}

fclose(data);

free(arr);
free(t);

return 0;
}