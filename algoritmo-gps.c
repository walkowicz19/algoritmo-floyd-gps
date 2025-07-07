#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define INF 99999
#define MAX_NAME_LENGTH 50

// Estrutura para representar uma cidade/localização
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    double latitude;
    double longitude;
} Location;

// Estrutura principal do GPS
typedef struct {
    int num_locations;
    Location locations[MAX_NODES];
    int distance_matrix[MAX_NODES][MAX_NODES];
    int next_matrix[MAX_NODES][MAX_NODES];
} GPS;

// Função para limpar o buffer de entrada
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para ler string com espaços
void read_string(char *str, int max_len) {
    fflush(stdout);  // Força a exibição do prompt
    if (fgets(str, max_len, stdin) != NULL) {
        // Remove a quebra de linha se presente
        str[strcspn(str, "\n")] = 0;
    }
}

// Função para inicializar o GPS
void init_gps(GPS *gps) {
    gps->num_locations = 0;
    
    // Inicializar matrizes
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            if (i == j) {
                gps->distance_matrix[i][j] = 0;
            } else {
                gps->distance_matrix[i][j] = INF;
            }
            gps->next_matrix[i][j] = -1;
        }
    }
}

// Função para adicionar uma localização
int add_location(GPS *gps, const char *name, double lat, double lon) {
    if (gps->num_locations >= MAX_NODES) {
        printf("Erro: Número máximo de localizações atingido!\n");
        fflush(stdout);
        return -1;
    }
    
    int id = gps->num_locations;
    gps->locations[id].id = id;
    strncpy(gps->locations[id].name, name, MAX_NAME_LENGTH - 1);
    gps->locations[id].name[MAX_NAME_LENGTH - 1] = '\0';
    gps->locations[id].latitude = lat;
    gps->locations[id].longitude = lon;
    
    gps->num_locations++;
    return id;
}

// Função para adicionar uma estrada (conexão entre duas localizações)
void add_road(GPS *gps, int from, int to, int distance) {
    if (from >= 0 && from < gps->num_locations && 
        to >= 0 && to < gps->num_locations) {
        gps->distance_matrix[from][to] = distance;
        gps->distance_matrix[to][from] = distance; // Estrada bidirecional
        gps->next_matrix[from][to] = to;
        gps->next_matrix[to][from] = from;
    }
}

// Implementação do algoritmo Floyd-Warshall
void floyd_warshall(GPS *gps) {
    int n = gps->num_locations;
    
    printf("Calculando rotas mais curtas...\n");
    fflush(stdout);
    
    // Algoritmo Floyd-Warshall
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (gps->distance_matrix[i][k] != INF && 
                    gps->distance_matrix[k][j] != INF &&
                    gps->distance_matrix[i][k] + gps->distance_matrix[k][j] < gps->distance_matrix[i][j]) {
                    
                    gps->distance_matrix[i][j] = gps->distance_matrix[i][k] + gps->distance_matrix[k][j];
                    gps->next_matrix[i][j] = gps->next_matrix[i][k];
                }
            }
        }
    }
    
    printf("Cálculo concluído!\n\n");
    fflush(stdout);
}

// Função para encontrar o ID de uma localização pelo nome (case insensitive)
int find_location_by_name(GPS *gps, const char *name) {
    for (int i = 0; i < gps->num_locations; i++) {
        if (strcasecmp(gps->locations[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Função para reconstruir e exibir o caminho
void print_path(GPS *gps, int start, int end) {
    if (gps->next_matrix[start][end] == -1) {
        printf("Não há rota disponível entre %s e %s\n", 
               gps->locations[start].name, gps->locations[end].name);
        fflush(stdout);
        return;
    }
    
    printf("\n=== ROTA CALCULADA ===\n");
    printf("De: %s\n", gps->locations[start].name);
    printf("Para: %s\n", gps->locations[end].name);
    printf("Distância total: %d km\n\n", gps->distance_matrix[start][end]);
    
    printf("Caminho detalhado:\n");
    int current = start;
    int step = 1;
    
    printf("%d. %s", step++, gps->locations[current].name);
    
    while (current != end) {
        current = gps->next_matrix[current][end];
        printf(" → %s", gps->locations[current].name);
        if (current != end) {
            printf("\n%d. ", step++);
        }
    }
    printf("\n\n");
    fflush(stdout);
}

// Função para exibir todas as localizações
void list_locations(GPS *gps) {
    printf("\n=== LOCALIZAÇÕES DISPONÍVEIS ===\n");
    for (int i = 0; i < gps->num_locations; i++) {
        printf("%d. %s (%.2f, %.2f)\n", 
               i + 1, gps->locations[i].name, 
               gps->locations[i].latitude, gps->locations[i].longitude);
    }
    printf("\n");
    fflush(stdout);
}

// Função para exibir a matriz de distâncias
void print_distance_matrix(GPS *gps) {
    printf("\n=== MATRIZ DE DISTÂNCIAS ===\n");
    printf("%-15s", "");
    
    for (int i = 0; i < gps->num_locations; i++) {
        printf("%-12s", gps->locations[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < gps->num_locations; i++) {
        printf("%-15s", gps->locations[i].name);
        for (int j = 0; j < gps->num_locations; j++) {
            if (gps->distance_matrix[i][j] == INF) {
                printf("%-12s", "∞");
            } else {
                printf("%-12d", gps->distance_matrix[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
    fflush(stdout);
}

// Função principal com menu interativo
int main() {
    GPS gps;
    init_gps(&gps);
    
    printf("=== GPS SIMPLES ===\n\n");
    fflush(stdout);
    
    // Dados de exemplo (mapa fictício de cidades)
    printf("Carregando mapa de exemplo...\n");
    fflush(stdout);
    
    // Adicionando cidades
    int sp = add_location(&gps, "Sao_Paulo", -23.5505, -46.6333);
    int rj = add_location(&gps, "Rio_de_Janeiro", -22.9068, -43.1729);
    int bh = add_location(&gps, "Belo_Horizonte", -19.9167, -43.9345);
    int bs = add_location(&gps, "Brasilia", -15.7939, -47.8828);
    int sal = add_location(&gps, "Salvador", -12.9714, -38.5014);
    int rec = add_location(&gps, "Recife", -8.0476, -34.8770);
    
    // Adicionando estradas (distâncias aproximadas em km)
    add_road(&gps, sp, rj, 430);
    add_road(&gps, sp, bh, 586);
    add_road(&gps, sp, bs, 1015);
    add_road(&gps, rj, bh, 434);
    add_road(&gps, bh, bs, 716);
    add_road(&gps, bh, sal, 1372);
    add_road(&gps, bs, sal, 1440);
    add_road(&gps, sal, rec, 800);
    
    // Executar Floyd-Warshall
    floyd_warshall(&gps);
    
    // Menu interativo
    int opcao;
    char origem[MAX_NAME_LENGTH], destino[MAX_NAME_LENGTH];
    
    while (1) {
        printf("=== MENU ===\n");
        printf("1. Calcular rota\n");
        printf("2. Listar localizações\n");
        printf("3. Mostrar matriz de distâncias\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        fflush(stdout);
        
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Digite um número.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer(); // Limpa o buffer após scanf
        
        switch (opcao) {
            case 1: {
                printf("\nDigite a cidade de origem (ex: Sao_Paulo): ");
                fflush(stdout);
                read_string(origem, MAX_NAME_LENGTH);
                
                printf("Digite a cidade de destino (ex: Rio_de_Janeiro): ");
                fflush(stdout);
                read_string(destino, MAX_NAME_LENGTH);
                
                int id_origem = find_location_by_name(&gps, origem);
                int id_destino = find_location_by_name(&gps, destino);
                
                if (id_origem == -1) {
                    printf("Cidade de origem '%s' não encontrada!\n\n", origem);
                    printf("Dica: Use nomes como Sao_Paulo, Rio_de_Janeiro, etc.\n\n");
                } else if (id_destino == -1) {
                    printf("Cidade de destino '%s' não encontrada!\n\n", destino);
                    printf("Dica: Use nomes como Sao_Paulo, Rio_de_Janeiro, etc.\n\n");
                } else {
                    print_path(&gps, id_origem, id_destino);
                }
                fflush(stdout);
                break;
            }
                
            case 2: {
                list_locations(&gps);
                break;
            }
                
            case 3: {
                print_distance_matrix(&gps);
                break;
            }
                
            case 4: {
                printf("Obrigado por usar o GPS Simples!\n");
                fflush(stdout);
                return 0;
            }
                
            default: {
                printf("Opção inválida! Escolha entre 1-4.\n\n");
                fflush(stdout);
                break;
            }
        }
    }
    
    return 0;
}