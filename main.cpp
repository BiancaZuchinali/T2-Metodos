#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void read_input(vector<vector<float>>& transitions, int& alunos_sem1, int& max_semestre) {
    ifstream entrada("caso1.txt");
    string lixo;
    entrada >> lixo >> lixo >> lixo >> alunos_sem1;
    int semestre_i, dest_semestre_i;
    string semestre, dest_semestre;
    float prob;
    max_semestre = 0;
    while (1) {
        if (entrada.eof()) break;
        entrada >> semestre >> lixo >> prob >> lixo >> dest_semestre;
        semestre_i = stoi(semestre.substr(4, 2));
        if (semestre_i > max_semestre) {
            max_semestre = semestre_i;
        }
        char caractere = dest_semestre.back();
        if (caractere == 'a') { // Significa que é o diploma
            dest_semestre_i = max_semestre + 1;
        } else {
            dest_semestre_i = stoi(dest_semestre.substr(4, 2));
        }
        transitions[dest_semestre_i - 1][semestre_i - 1] = prob;
    }
}

void Gauss(vector<vector<float>> &coef_matrix, vector<float> &result) {
    // SEM PIVOTAMENTO
    /*for (int i = 0; i < coef_matrix.size(); ++i) { // linha
        // Elimina os menores numeros na coluna i
        for (int j = i + 1; j < coef_matrix.size(); ++j) { // colunas colunas partindo da seguinte
            float f = coef_matrix[j][i] / coef_matrix[i][i]; // calcuna o número para zerar
            for (int k = i; k < coef_matrix.size(); ++k) { // colunas partindo da atual
                coef_matrix[j][k] -= coef_matrix[i][k] * f; // zera
            }
            result[j] -= result[i] * f;
        }
    }

    // Back substitution
    for (int i = coef_matrix.size() - 1; i >= 0; --i) { // linha no ordem reversa
        for (int j = i + 1; j < coef_matrix.size(); ++j) { // colunas partindo da seguinte
            result[i] -= coef_matrix[i][j] * result[j];
        }
        result[i] /= coef_matrix[i][i];
    }*/

    // COM PIVOTAMENTO
    for (int i = 0; i < coef_matrix.size(); ++i) {
        // Acha a coluna de pivotamento i
        int max_idx = i;
        for (int j = i + 1; j < coef_matrix.size(); ++j) {
            if (abs(coef_matrix[j][i]) > abs(coef_matrix[max_idx][i])) {
                max_idx = j;
            }
        }
        // Swap linhas(colunas)
        swap(coef_matrix[i], coef_matrix[max_idx]);
        swap(result[i], result[max_idx]);

        // Eliminar elementos menores
        for (int j = i + 1; j < coef_matrix.size(); ++j) {
            float f = coef_matrix[j][i] / coef_matrix[i][i];
            for (int k = i; k < coef_matrix.size(); ++k) {
                coef_matrix[j][k] -= coef_matrix[i][k] * f;
            }
            result[j] -= result[i] * f;
        }
    }

    // Back substitution
    for (int i = coef_matrix.size() - 1; i >= 0; --i) {
        for (int j = i + 1; j < coef_matrix.size(); ++j) {
            result[i] -= coef_matrix[i][j] * result[j];
        }
        result[i] /= coef_matrix[i][i];
    }
}


vector<float>  total_a(vector<vector<float>>& transitions, int& alunos_sem1, int max_semestre, float total_alunos_comDiploma, float total_alunos_semDiploma) {
   vector<vector<float>> coef_matrix(max_semestre + 1,vector<float>(max_semestre + 1, 0.0));
    for (int i = 0; i < max_semestre + 1; i++) {
        for (int j = 0; j < max_semestre + 1; j++) {
            coef_matrix[i][j] = transitions[i][j];
        }
    }

    for (int i = 0; i < coef_matrix.size(); i++) {
        coef_matrix[i][i] -= 1;
    }
  
    /*cout << "Matriz de coeficientes:" << endl;
  for (int i = 0; i < coef_matrix.size(); i++) {
    cout << "Linha: " << i + 1 << ": ";
    for (int j = 0; j < coef_matrix[i].size(); j++) {
      cout << coef_matrix[i][j] << " ";
    }
    cout << endl;
  }*/

    vector<float> result(coef_matrix.size(), 0.0);
    result[0] = -alunos_sem1;
    Gauss(coef_matrix, result);

    return result;
}

int main() {
    vector<vector<float>> transitions(20, vector<float>(20, 0.0));
    int alunos_sem1, max_semestre;
    float total_alunos_comDiploma = 0.0;
    float total_alunos_semDiploma = 0.0;

    read_input(transitions, alunos_sem1, max_semestre);
    vector<float> result = total_a(transitions, alunos_sem1, max_semestre, total_alunos_comDiploma, total_alunos_semDiploma);

     /*cout << "Matriz de transições:" << endl;
  for (int i = 0; i <= max_semestre; i++) {
    cout << "Semestre " << i + 1 << ": ";
    for (int j = 0; j <= max_semestre; j++) {
      cout << transitions[i][j] << " ";
    }
    cout << endl;
  }*/

    for (int i = 0; i < max_semestre+1; i++) {
        total_alunos_comDiploma += result[i];
        cout << result[i]<< endl;
    }
  
    total_alunos_semDiploma = total_alunos_comDiploma - result[max_semestre]; 

    cout << "Total de alunos com os Diplomados: " << total_alunos_comDiploma << endl;
    cout << "Total de alunos sem os Diplomados: " << total_alunos_semDiploma << endl;
  
    return 0;
}
