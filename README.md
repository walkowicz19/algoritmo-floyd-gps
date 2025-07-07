Este projeto implementa um sistema GPS simples que calcula rotas entre cidades usando o algoritmo Floyd-Warshall. O programa permite adicionar localizações (cidades) e estradas (conexões entre cidades), calcular a rota mais curta entre dois pontos e visualizar a matriz de distâncias.

# Estruturas de Dados

Location: Armazena informações sobre uma cidade/localização (ID, nome, latitude, longitude).

GPS: Contém todas as localizações e matrizes de distâncias e próximos passos.

# Funções Principais

init_gps: Inicializa a estrutura GPS.

add_location: Adiciona uma nova localização ao sistema.

add_road: Adiciona uma estrada (conexão) entre duas localizações.

floyd_warshall: Implementa o algoritmo Floyd-Warshall para calcular as rotas mais curtas.

print_path: Exibe o caminho detalhado entre duas localizações.

list_locations: Lista todas as localizações disponíveis.

print_distance_matrix: Exibe a matriz de distâncias entre todas as localizações.

# Como Usar

Pré-requisitos - Compilador C (como GCC). Terminal ou prompt de comando.

Compilação - Salve o código em um arquivo chamado algoritmo-gps.c.

Compile o código usando o seguinte comando:

bash
gcc algoritmo-gps.c -o gps

Execute o programa:

bash
./gps

# Funcionalidades

Calcular Rota:

Digite o nome da cidade de origem e destino. O programa exibirá o caminho detalhado e a distância total.

Listar Localizações:

Exibe todas as cidades cadastradas no sistema, com suas coordenadas.

Mostrar Matriz de Distâncias:

Exibe a matriz de distâncias entre todas as cidades.

Sair:

Encerra o programa.

# Exemplo de Uso

Ao iniciar o programa, um mapa de exemplo com cidades brasileiras é carregado automaticamente.

Para calcular uma rota:

Escolha a opção 1 no menu. Digite a cidade de origem (ex: Sao_Paulo). Digite a cidade de destino (ex: Rio_de_Janeiro). O programa exibirá o caminho e a distância total.

Para visualizar todas as localizações disponíveis:

Escolha a opção 2 no menu.

Para visualizar a matriz de distâncias:

Escolha a opção 3 no menu.
