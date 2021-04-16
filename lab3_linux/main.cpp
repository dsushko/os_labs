#include <iostream>
#include <fstream>
#include <thread>
#include <queue>
#include <mutex>
#include <ctime>
#include <string>
#include <pthread.h>


int current_streams = 0;


double **multiplicate_matricies(
        double **matrix1,
        double **matrix2,
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
        double **result = new double *[m1];
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
    } else return matrix1;
}

void make_matrix_sq_zero(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }
}

void add_sec_matrix_to_first(double **matrix1, double **matrix2, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix1[i][j] += matrix2[i][j];
        }
    }
}

class parameters_mult1 {
public:
    double **matrix_result;
    double **matrix_1;
    double **matrix_2;
    int block_size;
    int iteration;
    int n;

    parameters_mult1(double **matrix_result,
                     double **matrix_1,
                     double **matrix_2,
                     int block_size,
                     int iteration,
                     int n) {
        this->matrix_result = matrix_result;
        this->matrix_1 = matrix_1;
        this->matrix_2 = matrix_2;
        this->block_size = block_size;
        this->iteration = iteration;
        this->n = n;

    }
};

class parameters_mult2 {
public:
    double **matrix_result;
    double **matrix_1;
    double **matrix_2;
    int block_size;
    int iteration;
    int n;

    parameters_mult2(double **matrix_result,
                     double **matrix_1,
                     double **matrix_2,
                     int block_size,
                     int iteration,
                     int n) {
        this->matrix_result = matrix_result;
        this->matrix_1 = matrix_1;
        this->matrix_2 = matrix_2;
        this->block_size = block_size;
        this->iteration = iteration;
        this->n = n;

    }
};

class parameters_mult3 {
public:
    double **matrix_result;
    double **matrix_1;
    double **matrix_2;
    int block_size;
    int it_i;
    int it_j;
    int n;

    parameters_mult3(double **matrix_result,
                     double **matrix_1,
                     double **matrix_2,
                     int block_size,
                     int it_i,
                     int it_j,
                     int n) {
        this->matrix_result = matrix_result;
        this->matrix_1 = matrix_1;
        this->matrix_2 = matrix_2;
        this->block_size = block_size;
        this->it_i = it_i;
        this->it_j = it_j;
        this->n = n;

    }
};

void *mult1(void *param) {
    current_streams++;
    parameters_mult1 *p = (parameters_mult1 *) param;
    double **block = new double *[p->block_size];
    for (int i = 0; i < p->block_size; i++) {
        block[i] = new double[p->block_size];
        for (int j = 0; j < p->block_size; j++) {
            block[i][j] = 0;
        }
    }
    make_matrix_sq_zero(block, p->block_size);
    for (int i = 0; i < p->n / p->block_size; i++) {
        block = multiplicate_matricies(p->matrix_1, p->matrix_2, p->n, p->block_size, p->block_size, p->n,
                                       p->iteration * p->block_size, 0, 0, i * p->block_size);
        for (int i1 = 0; i1 < p->block_size; i1++) {
            for (int j1 = 0; j1 < p->block_size; j1++) {
                p->matrix_result[i1 + p->iteration * p->block_size][j1 + p->block_size * i] = block[i1][j1];
            }
        }
    }
    current_streams--;
}

std::mutex locker_2;

void *mult2(void *param) {
    current_streams++;
    parameters_mult2 *p = (parameters_mult2 *) param;
    double **block = new double *[p->block_size];
    for (int i = 0; i < p->block_size; i++) {
        block[i] = new double[p->block_size];
        for (int j = 0; j < p->block_size; j++) {
            block[i][j] = 0;
        }
    }

    make_matrix_sq_zero(block, p->block_size);
    block = multiplicate_matricies(p->matrix_1, p->matrix_2,
                                   p->block_size, p->n, p->n, p->block_size,
                                   0, p->iteration * p->block_size, p->iteration * p->block_size, 0);

    locker_2.lock();
    add_sec_matrix_to_first(p->matrix_result, block, p->n);
    locker_2.unlock();
    current_streams--;
}

std::mutex **locker_3;
std::mutex queue_mutex;

void *mult3(void *param) {
    current_streams++;
    std::queue<parameters_mult3> *q = (std::queue<parameters_mult3> *) param;
    while (true) {
        queue_mutex.lock();
        if (q->empty()) {
            queue_mutex.unlock();
            current_streams--;
            break;
        }
        parameters_mult3 p = q->front();
        q->pop();
        queue_mutex.unlock();

        double **block = new double *[p.block_size];
        for (int i = 0; i < p.block_size; i++) {
            block[i] = new double[p.block_size];
            for (int j = 0; j < p.block_size; j++) {
                block[i][j] = 0;
            }
        }

        make_matrix_sq_zero(block, p.block_size);
        for (int k = 0; k < p.n / p.block_size; k++) {
            double **tmp = multiplicate_matricies
                    (p.matrix_1, p.matrix_2,
                     p.block_size, p.block_size, p.block_size, p.block_size,
                     p.it_j * p.block_size, k * p.block_size, k * p.block_size, p.it_i * p.block_size);
            add_sec_matrix_to_first(block, tmp, p.block_size);
        }
        locker_3[p.it_i][p.it_j].lock();
        for (int i = 0; i < p.block_size; i++) {
            for (int j = 0; j < p.block_size; j++) {
                p.matrix_result[j + p.block_size * p.it_j][i + p.it_i * p.block_size] = block[j][i];
            }
        }
        locker_3[p.it_i][p.it_j].unlock();
    }
}

void print_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void await(){
    while(current_streams > 0){

    }
}

int main() {
    //если чо всё квадратное, и матрицы и блоки
    int n;
    int streams;
    bool print_matricies = false;
    std::string answer;
    std::ifstream fin("input.txt");
    fin >> n;
    fin >> streams;
    std::cout << "print matricies? yes/no\n";
    std::cin >> answer;
    if (answer == "yes") {
        print_matricies = true;
    }

    double **a = new double *[n];
    double **b = new double *[n];
    double **c = new double *[n];
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



    int start = clock();
    double **res = multiplicate_matricies(a, b, n, n, n, n, 0, 0, 0, 0);
    int end = clock();
    if (print_matricies)
        print_matrix(res, n);
    std::cout << (double) (end - start) * 1000 / (double) CLOCKS_PER_SEC << "ms\n\n";

    make_matrix_sq_zero(c, n);

    pthread_t stream_ids1[streams];

    //случай - первая матрица разбита на строки, вторая на столбцы
    make_matrix_sq_zero(c, n);

    start = clock();
    for (int i = 0; i < streams; i++) {
        void *p = new parameters_mult1(c, a, b, n / streams, i, n);
        pthread_create(&stream_ids1[i], nullptr, mult1, p);
    }
    for(int i = 0; i < streams; i++){
        pthread_join(stream_ids1[i], nullptr);
    }
    //await
    end = clock();
    if (print_matricies)
        print_matrix(c, n);
    std::cout << (double) (end - start) * 1000 / (double) CLOCKS_PER_SEC << "ms\n\n";

    //случай - вторая матрица разбита на строки, первая на столбцы
    pthread_t stream_ids2[streams];

    make_matrix_sq_zero(c, n);
    start = clock();
    for (int i = 0; i < streams; i++) {
        void *p = new parameters_mult2(c, a, b, n / streams, i, n);
        pthread_create(&stream_ids2[i], NULL, &mult2, p);
    }
    for(int i = 0; i < streams; i++){
        pthread_join(stream_ids2[i], nullptr);
    }
    //await
    end = clock();
    if (print_matricies)
        print_matrix(c, n);
    std::cout << (double) (end - start) * 1000 / (double) CLOCKS_PER_SEC << "ms\n\n";


    //случай номер 3 когда все разбито на квадратненькие блоки
    pthread_t stream_ids3[streams];
    locker_3 = new std::mutex *[streams];
    for (int i = 0; i < streams; i++) {
        locker_3[i] = new std::mutex[streams];
    }
    make_matrix_sq_zero(c, n);
    std::queue<parameters_mult3> operations;
    void* operations_ptr = &operations;
    for (int i = 0; i < streams; i++) {
        for (int j = 0; j < streams; j++) {
            operations.push(parameters_mult3(c, a, b, n / streams, i, j, n));
        }
    }
    start = clock();
    for (int i = 0; i < streams; i++) {
        pthread_create(&stream_ids3[i], nullptr, &mult3,operations_ptr);
    }
    for(int i = 0; i < streams; i++){
        pthread_join(stream_ids3[i], nullptr);
    }
    //await
    end = clock();

    if (print_matricies)
        print_matrix(c, n);
    std::cout << (double) (end - start) * 1000 / (double) CLOCKS_PER_SEC << "ms\n\n";

    std::cin.get();
    std::cin.get();

    return 0;
}