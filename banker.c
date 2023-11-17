
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
//mudar dps
#define MAX_CUSTOMERS 10
#define MAX_RESOURCES 10

int available[MAX_RESOURCES];
int maximum[MAX_CUSTOMERS][MAX_RESOURCES];
int allocation[MAX_CUSTOMERS][MAX_RESOURCES];
int need[MAX_CUSTOMERS][MAX_RESOURCES];

void readMaximumFromFile(const char *filename, int num_resources, int num_customers);
int requestResources(int customer, int request[MAX_RESOURCES], int num_resources);
int releaseResources(int customer, int release[MAX_RESOURCES], int num_resources);
void printState(int num_resources);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num_resource_1> <num_resource_2> ... <num_resource_n>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int num_resources = argc - 1, num_customers = 5;
    readMaximumFromFile("customer.txt", num_resources, num_customers);
    
    while (1) {
        char command[10];
        int customer, request[MAX_RESOURCES], release[MAX_RESOURCES];

        printf("Enter command (RQ/RL/Status/Exit): ");
        scanf("%s", command);

        if (strcmp(command, "RQ") == 0) {
            scanf("%d", &customer);
            for (int i = 0; i < num_resources; ++i) {
                scanf("%d", &request[i]);
            }

            if (requestResources(customer, request, num_resources)) {
                printf("Request granted.\n");
            } else {
                printf("Request denied.\n");
            }
        } else if (strcmp(command, "RL") == 0) {
            scanf("%d", &customer);
            for (int i = 0; i < num_resources; ++i) {
                scanf("%d", &release[i]);
            }

            if (releaseResources(customer, release, num_resources)) {
                printf("Resources released.\n");
            } else {
                printf("Invalid release request.\n");
            }
        } else if (strcmp(command, "Status") == 0) {
            printState(num_resources);
        } else if (strcmp(command, "Exit") == 0) {
            break;
        } else {
            printf("Invalid command. Please enter RQ, RL, Status, or Exit.\n");
        }
    }

    return 0;
}

void readMaximumFromFile(const char *filename, int num_resources, int num_customers) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    printf("File contents:\n");
    for (int i = 0; i < num_customers; i++) {
        for (int j = 0; j < num_resources; j++) {
            int value;
            if (fscanf(file, "%d", &value) != 1) {
                fprintf(stderr, "Error reading number from file\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            printf("%d", value);

            if (j < num_resources - 1) {
                if (fgetc(file) != ',') {
                    fprintf(stderr, "Error reading comma from file\n");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
                printf(", ");
            }

            allocation[i][j] = 0;
            need[i][j] = maximum[i][j];
        }
        printf("\n");
    }

    fclose(file);
}


int requestResources(int customer, int request[MAX_RESOURCES], int num_resources) {
    for (int i = 0; i < num_resources; ++i) {
        if (request[i] > need[customer][i] || request[i] > available[i]) {
            return 0; // Request denied
        }
    }

    // Pretend to allocate resources (not implemented here)
    return 1; // Request granted
}

int releaseResources(int customer, int release[MAX_RESOURCES], int num_resources) {
    for (int i = 0; i < num_resources; ++i) {
        if (release[i] > allocation[customer][i]) {
            return 0; // Invalid release request
        }
    }

    // Pretend to release resources (not implemented here)
    return 1; // Resources released
}

void printState(int num_resources) {
    printf("Available resources: ");
    for (int i = 0; i < num_resources; ++i) {
        printf("%d ", available[i]);
    }
    printf("\n");

    printf("Maximum resources:\n");
    for (int i = 0; i < MAX_CUSTOMERS; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            printf("%d ", maximum[i][j]);
        }
        printf("\n");
    }

    printf("Allocation resources:\n");
    for (int i = 0; i < MAX_CUSTOMERS; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("Need resources:\n");
    for (int i = 0; i < MAX_CUSTOMERS; ++i) {
        for (int j = 0; j < num_resources; ++j) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
}
#if(0)
void lerArquivo(const char *fileName, int customers, int resources) {
    FILE *file = fopen(fileName, "r");
    char linha[20];

    if (file == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < customers; i++) {
        for (int j = 0; j < resources; j++) {
            if (fscanf(file, "%d", &maximum[i][j]) != 1) {
                printf("Error reading from file\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
            
            if (j < resources - 1) {
                if (fgetc(file) != ',') {
                    printf("Error reading from file\n");
                    fclose(file);
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    fclose(file);
}
#endif
