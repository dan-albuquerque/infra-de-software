
/*
  executavel tem as instacias dos recursos
  exe: ./banker 10 3 5
  tenho 10 instancias do recurso 1, 3 do recurso 2 e 5 do recurso 3
*/
/*
  costumer tem o numero maximo de instancias 
  ex: 7,5,3
  no cliente 0 temos ate no maximo 7 instancias do recurso 1, 5 do recurso 2 e 3 do recurso 3
*/
/*
  commands tem os pedidos dos clientes
  ex: RQ 0 0 2 1
  primeiro 0 eh o cliente - cliente 0 nesse caso
  o cliente 0 quer 0 do recurso 1, quer 2 do recurso 2, quer 1 do recurso 3
  RQ eh pedidno pra alocar recurso, RL eh release ta liberando recursos
  * imprime informações do cenario atual -> imprime a matriz
*/
/*
  result diz linha a linha do commands e as mensagens respectivas a cada comando, se pode alocar, se pode liberar, e se pode ter deadlock ou nao
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void readMaximumFromFile(FILE *customer_file, int num_resources, int num_customers,int maximum[][num_resources],
int allocation[][num_resources],int need[][num_resources]);

int isBankerSafe(int num_resources, int num_customers, int available[], 
    int allocation[][num_resources], int need[][num_resources]);
int requestResources(int customer, int request[], int num_resources, int num_customers, 
    int available[],int need[][num_resources], int allocation[][num_resources]);

int releaseResources(int customer, int release[], int num_resources, int allocation[][num_resources], 
    int need[][num_resources], int available[]);

void printState(int num_resources, int num_customers, FILE *result_file, int available[], 
    int maximum[][num_resources], int allocation[][num_resources], int need[][num_resources]);

void printCustomer(int customer, int num_resources, FILE *result_file, int maximum[][num_resources],
    int allocation[][num_resources], int need[][num_resources]);

void executeComandos(char command[], int num_resources, int num_customers, FILE *command_file, FILE *result_file, 
    int available[], int allocation[][num_resources], int need[][num_resources], int maximum[][num_resources]);
FILE * getNumCustomersAndResources(const char *filename, int *num_customers, int *num_resources);

int main(int argc, char *argv[]) {
    int num_resources = argc - 1, num_customers = 0 , num_resources_file = 0;
    FILE * customer_file = getNumCustomersAndResources("customer.txt", &num_customers, &num_resources_file);
    int available[argc - 1];
    int maximum[num_customers][argc - 1];
    int allocation[num_customers][argc - 1];
    int need[num_customers][argc - 1];

    if(num_resources != num_resources_file){
        printf("Incompatibility between customer.txt and command line\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_resources; i++) {
        available[i] = atoi(argv[i+1]);
    }
    readMaximumFromFile(customer_file, num_resources, num_customers, maximum, allocation, need);
    FILE *command_file = fopen("commands.txt", "r");
    if (command_file == NULL) {
        printf("Fail to read commands.txt\n");
        exit(EXIT_FAILURE);
    }

    FILE *result_file = fopen("result.txt", "w");
    if (result_file == NULL) {
        printf("Fail to write result.txt\n");
        exit(EXIT_FAILURE);
    }

    char command[10]; //ALTERAR
    while (fscanf(command_file, "%s", command) == 1) {
        executeComandos(command, num_resources, num_customers, command_file, result_file, available, allocation, need, maximum);
    }

    fclose(result_file);
    fclose(command_file);
    return 0;
}


void readMaximumFromFile(FILE *customer_file, int num_resources, int num_customers,int maximum[][num_resources],
int allocation[][num_resources],int need[][num_resources]) {
    if (customer_file == NULL) {
        printf("Fail to read customer.txt\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < num_customers; i++) {
        for (int j = 0; j < num_resources; j++) {
            if (fscanf(customer_file, "%d", &maximum[i][j]) != 1) {
                fprintf(stderr, "Error reading number from file\n");
                fclose(customer_file);
                exit(EXIT_FAILURE);
            }
            if (j < num_resources - 1) {
                if (fgetc(customer_file) != ',') {
                    fprintf(stderr, "Error reading comma from file\n");
                    fclose(customer_file);
                    exit(EXIT_FAILURE);
                }
            }
            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
    }
    fclose(customer_file);
}

int isBankerSafe(int num_resources, int num_customers, int available[], 
int allocation[][num_resources], int need[][num_resources]) {

    int work[num_resources];
    int finish[num_customers];

    // Initialize work and finish arrays
    for (int i = 0; i < num_resources; ++i) {
        work[i] = available[i];
    }
    for (int i = 0; i < num_customers; ++i) {
        finish[i] = 0;
    }
    int found = 1;
    while (found) {
        found = 0;
        for (int i = 0; i < num_customers; ++i) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < num_resources; ++j) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == num_resources) {
                    // Customer can be satisfied
                    found = 1;
                    finish[i] = 1;
                    for (int k = 0; k < num_resources; ++k) {
                        work[k] += allocation[i][k];
                    }
                }
            }
        }
    }
    // Check if all customers were satisfied
    for (int i = 0; i < num_customers; ++i) {
        if (!finish[i]) {
            return 0; // Unsafe state
        }
    }
    return 1; // Safe state
}
//ver com erico a ordem
int requestResources(int customer, int request[], int num_resources, int num_customers, 
    int available[],int need[][num_resources], int allocation[][num_resources]) {
    for (int i = 0; i < num_resources; ++i) {
        if (request[i] > available[i]) {
            return 0; // Request denied
        }
        if (request[i] > need[customer][i]) {
            return -1; // Request denied
        }
    }
    for (int i = 0; i < num_resources; ++i) {
        available[i] -= request[i];
        allocation[customer][i] += request[i];
        need[customer][i] -= request[i];
    }
    if (!isBankerSafe(num_resources, num_customers, available, allocation, need)) {
        // Request would cause an unsafe state, so undo allocation
        for (int i = 0; i < num_resources; ++i) {
            available[i] += request[i];
            allocation[customer][i] -= request[i];
            need[customer][i] += request[i];
        }
        return 2; // unsafe, pode ter deadlock
    }

    return 1; // Request granted
}

int releaseResources(int customer, int release[], int num_resources, int allocation[][num_resources], int need[][num_resources], int available[]) {
    for (int i = 0; i < num_resources; ++i) {
        if (release[i] > allocation[customer][i]) {
            return 0; //quantidade de recursos a serem liberados for maior do que a quantidade de recursos alocados para o cliente
        }
        if (release[i] > need[customer][i]) {
            return -1; //quantidade de recursos a serem liberados for maior do que a quantidade de recursos necessários para o cliente
        }
    }

    // Release resources
    for (int i = 0; i < num_resources; ++i) {
        allocation[customer][i] -= release[i];
        need[customer][i] += release[i];
        available[i] += release[i];
    }

    return 1; 
}

void printState(int num_resources, int num_customers, FILE *result_file, int available[], 
    int maximum[][num_resources], int allocation[][num_resources], int need[][num_resources]) {
    fprintf(result_file, "MAXIMUM | ALLOCATION | NEED\n");
    for (int i = 0; i < num_customers; ++i) {
        printCustomer(i, num_resources, result_file, maximum, allocation, need);
    }
    fprintf(result_file, "AVAILABLE ");
    for (int i = 0; i < num_resources; ++i) {
        fprintf(result_file, "%d ", available[i]);
    }
    fprintf(result_file, "\n");
}

void printCustomer(int customer, int num_resources, FILE *result_file, int maximum[][num_resources],
    int allocation[][num_resources], int need[][num_resources]) {
    for (int j = 0; j < num_resources; ++j) {
        fprintf(result_file, "%d ", maximum[customer][j]);
    }
    fprintf(result_file, "  | ");
    for (int j = 0; j < num_resources; ++j) {
        fprintf(result_file, "%d ", allocation[customer][j]);
    }
    fprintf(result_file, "     | ");
    for (int j = 0; j < num_resources; ++j) {
        fprintf(result_file, "%d ", need[customer][j]);
    }
    fprintf(result_file, "\n");
}

void executeComandos(char command[], int num_resources, int num_customers, FILE *command_file, FILE *result_file, 
    int available[], int allocation[][num_resources], int need[][num_resources], int maximum[][num_resources]){

    int customer, request[num_resources], release[num_resources];
    if (strcmp(command, "RQ") == 0) {
        fscanf(command_file, "%d", &customer);
        for (int i = 0; i < num_resources; ++i) {
            fscanf(command_file, "%d", &request[i]);
        }
        int banker_safe = requestResources(customer, request, num_resources, num_customers, available, need, allocation);
        if (banker_safe == 1) {
            fprintf(result_file, "Allocate to customer %d the resources ", customer);
            for (int i = 0; i < num_resources; ++i) {
                fprintf(result_file, "%d ", request[i]);
            }
            fprintf(result_file, "\n");
        } else if (banker_safe == 2){
            fprintf(result_file, "The customer %d request ", customer);
            for (int i = 0; i < num_resources; ++i) {
                fprintf(result_file, "%d ", request[i]);
            }
            fprintf(result_file, "was denied because result in an unsafe state \n");

        }else if(banker_safe == 0){
            fprintf(result_file, "The resources ");
            for (int i = 0; i < num_resources; ++i) {
                fprintf(result_file, "%d ", available[i]);
            }
            fprintf(result_file, "are not enough to customer %d request ", customer);
            for(int i = 0; i < num_resources; ++i){
                fprintf(result_file, "%d ", request[i]);
            }
            fprintf(result_file, "\n");
        } else {
            fprintf(result_file, "The customer %d request ", customer);
            for (int i = 0; i < num_resources; ++i) {
                fprintf(result_file, "%d ", request[i]);
            }
            fprintf(result_file, "was denied because exceed its maximum need \n");
        }
    } else if (strcmp(command, "RL") == 0) {
        fscanf(command_file, "%d", &customer);
        for (int i = 0; i < num_resources; ++i) {
            fscanf(command_file, "%d", &release[i]);
        }
        int release_request = releaseResources(customer, release, num_resources, allocation, need, available); 
        if (release_request) {
            fprintf(result_file, "Release from customer %d the resources ", customer);
            for (int i = 0; i < num_resources; i++) {
                fprintf(result_file, "%d ", release[i]);
            }
            fprintf(result_file, "\n");
        } else{
            fprintf(result_file, "The customer %d released ", customer);
            for (int i = 0; i < num_resources; i++) {
                fprintf(result_file, "%d ", release[i]);
            }
            fprintf(result_file, "was denied because exceed its maximum allocation \n", customer);
        }
    } else if (strcmp(command, "*") == 0) {
        printState(num_resources, num_customers, result_file, available, maximum, allocation, need);
    } else {
        fprintf(result_file, "Invalid command. Please enter RQ, RL, Status, or Exit. \n");
    }
}
FILE * getNumCustomersAndResources(const char *filename, int *num_customers, int *num_resources) {
    FILE *customer_file = fopen(filename, "r");
    if (customer_file == NULL) {
        printf("Fail to read %s\n", filename);
        exit(EXIT_FAILURE);
    }
    char c;
    while ((c = fgetc(customer_file)) != EOF) {
        if (c == '\n') {
            (*num_customers)++;
            // se for a primeira linha, conta o número de recursos
            if (*num_customers == 1) {
                (*num_resources) = 1;
                while ((c = fgetc(customer_file)) != '\n') {
                    if (c == ',') {
                        (*num_resources)++;
                    }
                }
            }
        } else if (c != ',' && (c < '0' || c > '9')) {
            printf("Fail to read %s\n", filename);
            exit(EXIT_FAILURE);
        }
    }
    (*num_customers) +=2 ; // última linha não tem \n e ta com 1 a menos
    rewind(customer_file);
    return customer_file;
}
