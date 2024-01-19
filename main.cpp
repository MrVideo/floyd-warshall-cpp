#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define INFINITY 1000
#define INFINITY_STRING "inf"
#define INFINITY_STRING_TERMINATED "inf\n"
#define DELIMITER ' '
#define BUF_SIZE 1024

void print_matrix(std::vector<std::vector<int>>& matrix, int dimensions);
std::vector<std::vector<int>> init_matrix(int dimensions);
bool floyd_warshall_algorithm(std::vector<std::vector<int>>& distances,
                             std::vector<std::vector<int>>& predecessors,
                             int dimensions);
std::vector<std::vector<int>> read_matrix_from_file(std::string& filename,
                                                    int dimensions);
int count_matrix_dimensions_from_file(std::string& filename);
void print_help_menu();

int main(int argc, char *argv[]) {
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<int>> predecessors;

    int node_num = 0;

    std::string filename;

    if (argc == 1) { // Interactive mode
        std::cout << "Enter the filename containing the matrix: ";

        std::cin >> filename;

        if (node_num == -1) {
            std::cout << "\nThere was an error while reading from file.\n";
            return 1;
        }
    } else if (argc == 2) {
        if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
			print_help_menu();
			return 0;
        } else {
			filename = argv[1];
		}
    } else {
		std::cout << "\nInvalid arguments. Type floyd-warshall -h for help.\n";
		return 1;
    }
	
	node_num = count_matrix_dimensions_from_file(filename);
	if (node_num == -1) {
		std::cout << "\nThere was an error while reading from file.\n";
		return 1;
	}

	distances = read_matrix_from_file(filename, node_num);
	if (distances.empty()) {
		std::cout << "\nUnable to initialise the distance matrix. Exiting.";
		return 1;
	}

	std::cout << "\nThe entered matrix is:\n";
	print_matrix(distances, node_num);

	predecessors = init_matrix(node_num);

	bool result = floyd_warshall_algorithm(distances, predecessors, node_num);
	
	if (!result) {
		std::cout << "\nA negative cost circuit was found: computation halted.";
	}

	std::cout << "\nThe final distance matrix is:\n";
	print_matrix(distances, node_num);

	if (result) {
		std::cout << "\nThe final predecessor matrix is:\n";
		print_matrix(predecessors, node_num);
	}

    return 0;
}

void print_matrix(std::vector<std::vector<int>> &matrix, int dimensions) {
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            if (matrix[i][j] == INFINITY)
                std::cout << "inf\t";
            else
                std::cout << matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

std::vector<std::vector<int>> init_matrix(int dimensions) {
    std::vector<std::vector<int>> matrix(dimensions);

    for (int i = 0; i < dimensions; i++)
        matrix[i] = std::vector<int>(dimensions, i);

    return std::move(matrix);
}

bool floyd_warshall_algorithm(std::vector<std::vector<int>> &distances,
                             std::vector<std::vector<int>> &predecessors,
                             int dimensions) {
    for (int u = 0; u < dimensions; u++) {
        for (int i = 0; i < dimensions; i++) {
            for (int j = 0; j < dimensions; j++) {
                if (i != u && j != u) {
                    if (distances[i][u] != INFINITY &&
                        distances[u][j] != INFINITY) {
                        if (distances[i][u] + distances[u][j] <
                            distances[i][j]) {
                            distances[i][j] = distances[i][u] + distances[u][j];

                            predecessors[i][j] = u;
                        }
                    }
                }
            }
        }

		for (int k = 0; k < dimensions; k++) {
			if (distances[k][k] < 0) {
				return false;
			}
		}
    }

    return true;
}

std::vector<std::vector<int>> read_matrix_from_file(std::string &filename,
                                                    int dimensions) {
    std::ifstream file(filename);

    std::string read_buf;
    std::vector<int> read_int;

    while (getline(file, read_buf)) {
        std::istringstream ss(read_buf);
        std::string token;
        while (ss >> token) {
            if (token == INFINITY_STRING) {
                read_int.push_back(INFINITY);
            } else {
                read_int.push_back(std::stoi(token));
            }
        }
    }

    std::vector<std::vector<int>> matrix(dimensions, std::vector<int>(dimensions));

    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            matrix[i][j] = read_int[dimensions * i + j];
        }
    }

    return matrix;
}

int count_matrix_dimensions_from_file(std::string& filename) {
    std::ifstream file(filename);

    std::string buffer, token;
    int element_counter = 0;

    if (getline(file, buffer)) {
        std::istringstream ss(buffer);
        std::string token;
        while (ss >> token) {
            element_counter++;
        }
    }

    return element_counter;
}

void print_help_menu() {
    std::cout << "Usage: floyd-warshall [filename.txt|OPTIONS]\n";
    std::cout << "\n\t-h, --help\tshows this prompt\n";
    std::cout << "\nIf no arguments are passed, the program will start in "
                 "interactive mode.\n";
}
