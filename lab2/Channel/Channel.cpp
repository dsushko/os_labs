#include <iostream>
#include <fstream>
#include <thread>
#include "buffered_channel.h"

double** multiplicate_matricies(double** matrix1,
	double** matrix2,
	int n1,
	int m1,
	int n2,
	int m2,
	int offset_x_1,
	int offset_x_2,
	int offset_y_1,
	int offset_y_2
) {
	if (n1 == m2) {
		double** result = new double* [m1];
		for (int i = 0; i < m1; i++) {
			result[i] = new double[n2];
		}
		for (int i = 0; i < m1; i++) {
			for (int j = 0; j < n2; j++) {
				result[i][j] = 0;
				for (int k = 0; k < n1; k++) {
					result[i][j] += matrix1[i + offset_x_1][k + offset_y_1] * matrix2[k + offset_x_2][j + offset_y_2];
				}
			}
		}
		return result;
	}
	else return matrix1;
}

void make_matrix_sq_zero(double** matrix, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix[i][j] = 0;
		}
	}
}
void add_sec_matrix_to_first(double** matrix1, double** matrix2, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			matrix1[i][j] += matrix2[i][j];
		}
	}
}

void mult1(double** matrix_result, double** matrix_1, double** matrix_2, int block_size, int n, int iteration) {
	double** block = new double* [block_size];
	for (int i = 0; i < block_size; i++) {
		block[i] = new double[block_size];
		for (int j = 0; j < block_size; j++) {
			block[i][j] = 0;
		}
	}
	make_matrix_sq_zero(block, block_size);
	for (int i = 0; i < n / block_size; i++) {
		block = multiplicate_matricies(matrix_1, matrix_2, n, block_size, block_size, n,
			iteration * block_size, 0, 0, i * block_size);
		for (int i1 = 0; i1 < block_size; i1++) {
			for (int j1 = 0; j1 < block_size; j1++) {
				matrix_result[i1 + iteration * block_size][j1 + block_size * i] = block[i1][j1];
			}
		}
	}


}

void mult2(double** matrix_result, double** matrix_1, double** matrix_2, int block_size, int n, int iteration) {
	if (iteration == 0)
		make_matrix_sq_zero(matrix_result, n);

	double** block = new double* [block_size];
	for (int i = 0; i < block_size; i++) {
		block[i] = new double[block_size];
		for (int j = 0; j < block_size; j++) {
			block[i][j] = 0;
		}
	}
	make_matrix_sq_zero(block, block_size);
	for (int i = 0; i < n / block_size; i++) {
		block = multiplicate_matricies(matrix_1, matrix_2, block_size, n, n, block_size,
			0, iteration * block_size, i * block_size, 0);
		for (int i1 = 0; i1 < block_size; i1++) {
			for (int j1 = 0; j1 < block_size; j1++) {
				matrix_result[i1 + iteration * block_size][j1 + block_size * i] = block[i1][j1];
			}
		}
	}
	add_sec_matrix_to_first(matrix_result, block, n);
}

void mult_3(double** matrix_result, double** matrix_1, double** matrix_2, int block_size, int n, int it_i, int it_j) {
	double** block = new double* [block_size];
	for (int i = 0; i < block_size; i++) {
		block[i] = new double[block_size];
		for (int j = 0; j < block_size; j++) {
			block[i][j] = 0;
		}
	}
	for (int i = 0; i < n / block_size; i++) {
		for (int j = 0; j < n / block_size; j++) {
			make_matrix_sq_zero(block, block_size);
			for (int k = 0; k < n / block_size; k++) {
				add_sec_matrix_to_first(block,
					multiplicate_matricies
					(matrix_1, matrix_2, n / block_size, n / block_size, n / block_size, n / block_size,
						it_j * block_size + j * block_size, k * n / block_size, k * n / block_size, it_i * block_size + i * block_size),
					n);
			}
		}
	}
	for (int i1 = 0; i1 < block_size; i1++) {
		for (int j1 = 0; j1 < block_size; j1++) {
			matrix_result[i1 + it_i * block_size][j1 + block_size * it_j] = block[i1][j1];
		}
	}
}

void print_matrix(double** matrix, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << "\n";
	}
}

int main() {
	int n;
	std::ifstream fin("input.txt");
	fin >> n;

	double** a = new double* [n];
	double** b = new double* [n];
	double** c = new double* [n];
	for (int i = 0; i < n; i++) {
		a[i] = new double[n];
		b[i] = new double[n];
		c[i] = new double[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fin >> a[i][j];
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fin >> b[i][j];
		}
	}

	double** res = multiplicate_matricies(a, b, n, n, n, n, 0, 0, 0, 0);

	print_matrix(res, n);

	int columns_1 = 2, rows_1 = 2, columns_2 = 2, rows_2 = 2;

	//случай - первая матрица разбита на строки, вторая на столбцы
	for (int i = 0; i < n / columns_1; i++) {
		std::thread curr_t(mult1, c, a, b, n / columns_1, n, i);
		curr_t.join();
	}

	//случай - вторая матрица разбита на строки, первая на столбцы
	for (int i = 0; i < n / rows_1; i++) {
		std::thread curr_t(mult2, c, a, b, n / columns_1, n, i);
		curr_t.join();
	}

	for (int i = 0; i < n / rows_1; i++) {
		for (int j = 0; j < n / columns_1; j++) {
			mult_3(c, a, b, n / columns_1, n, i, j);
		}
		//std::thread curr_t(mult2, c,a,b,n/columns_1, n, i);
		//curr_t.join();
	}

	//buffered_channel<std::thread> stream_channel(n/columns_1);


	print_matrix(c, n);

	return 0;
}