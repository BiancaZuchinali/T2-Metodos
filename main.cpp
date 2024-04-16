#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void read_input( vector<vector<float >>& transitions, int& alunos_sem1 ) {
  ifstream entrada( "caso1.txt" );
  string lixo;
  entrada >> lixo >> lixo >> lixo >> alunos_sem1;
  int semestre_i, dest_semestre_i;
  string semestre, dest_semestre;
  float prob;
  int max_semestre = 0;
  while ( 1 ) {
    if ( entrada.eof() ) break;
    entrada >> semestre >> lixo >> prob >> lixo >> dest_semestre;
    //cout << "li:" << prob << " do " << semestre<< " para " <<  dest_semestre << endl;
    semestre_i = stoi( semestre.substr( 4, 2 ) );
    if ( semestre_i > max_semestre ) {
      max_semestre = semestre_i;
    }
    char caractere = dest_semestre.back();
    if ( caractere == 'a' ) { // Significa que é o diploma
      dest_semestre_i = max_semestre + 1;
    } else {
      dest_semestre_i = stoi( dest_semestre.substr( 4, 2 ) );
    }
    if ( transitions.size() <= semestre_i - 1 ) {
      transitions.resize( max_semestre + 1, vector<float>( max_semestre + 1, 0.0 ) );
    }
    if ( transitions[semestre_i - 1].size() <= dest_semestre_i ) {
      transitions[semestre_i - 1].resize( max_semestre + 1, 0.0 );
    }
    // printf("coloquei: %f na linha %d coluna %d\n",prob,semestre_i, dest_semestre_i);
    transitions[semestre_i - 1][dest_semestre_i - 1] = prob;
  }
  cout << "max semetre:" << max_semestre << endl;
  // Coloca os zeros para garantir que a matriz seja quadrada
  for ( int i = 0; i <= max_semestre; i++ ) {
    if ( transitions[i].size() < max_semestre + 1 ) {
      cout << "tamanho matriz" << transitions[i].size() << endl;
      transitions[i].resize( max_semestre + 1, 0.0 );
      cout << "tamanho matriz com resize" << transitions[i].size() << endl;
      //cout << "ver oq tem na matriz 2 e 2" << transitions[2][2] << endl;
    }
  }
}

void Gauss(vector<vector<float>>& coef_matrix, vector<float>& result) {
  int n = coef_matrix.size();
  //SEM PIVOTAMENTO
    for (int i = 0; i < n; ++i) {
      // Elimina os menores numeros na coluna i
      for (int j = i + 1; j < n; ++j) {
        float f = coef_matrix[j][i] / coef_matrix[i][i];
        for (int k = i; k <= n; ++k) {
          coef_matrix[j][k] -= coef_matrix[i][k] * f;
        }
        result[j] -= result[i] * f;
      }
    }

    // Back substitution
    for (int i = n - 1; i >= 0; --i) {
      for (int j = i + 1; j < n; ++j) {
        result[i] -= coef_matrix[i][j] * result[j];
      }
      result[i] /= coef_matrix[i][i];
    }
  


  /*//COM PIVOTAMENTO
  for (int i = 0; i < n; ++i) {
    // Acha a coluna de pivotamento i
    int max_idx = i;
    for (int j = i + 1; j < n; ++j) {
      if (abs(coef_matrix[j][i]) > abs(coef_matrix[max_idx][i])) {
        max_idx = j;
      }
    }
    // Swap linhas(colunas)
    swap(coef_matrix[i], coef_matrix[max_idx]);
    swap(result[i], result[max_idx]);

    // Eliminar elementos menores
    for (int j = i + 1; j < n; ++j) {
      float f = coef_matrix[j][i] / coef_matrix[i][i];
      for (int k = i + 1; k <= n; ++k) {
        coef_matrix[j][k] -= coef_matrix[i][k] * f;
      }
      result[j] -= result[i] * f;
    }
  }

  // Back substitution
  for (int i = n - 1; i >= 0; --i) {
    for (int j = i + 1; j < n; ++j) {
      result[i] -= coef_matrix[i][j] * result[j];
    }
    result[i] /= coef_matrix[i][i];
  }*/
}


int total_a(vector<vector<float>>& transitions, int& alunos_sem1, int total_alunos) {
  vector<vector<float>> coef_matrix(transitions.size() + 1, vector<float>(transitions.size() + 1, 0.0));
//tentativa de generalizar o que está ali embaixo:
/*for(int i = 0; i <= transitions.size(); i++){
  if(i == transitions.size() ){
    coef_matrix[i][i] = 1; // quer dizer que é a linha do diploma
  }
  coef_matrix[i][i] = 1 - transitions[i][i];
  coef_matrix[i+1][i] = -transitions[i][i+1];
  // problemas para achar uma forma geral pro 0, será que faço uma coisa parecida com a de cima? tipo ali na função que lê
}*/
  // para o S1 
  coef_matrix[0][0] = 1 - transitions[0][0];
  coef_matrix[1][0] = -transitions[0][1];
  coef_matrix[2][0] = 0;
  //para o S2
  coef_matrix[0][1] = 0;
  coef_matrix[1][1] = 1 - transitions[1][1];
  coef_matrix[2][1] = -transitions[1][2];
    // para o S3
  coef_matrix[0][2] = 0;
  coef_matrix[1][2] = 0;  
  coef_matrix[2][2] = 1;

  cout << "Matriz de coeficientes:" << endl;
  for ( int i = 0; i < coef_matrix.size(); i++ ) {
    cout << "Linha: " << i + 1 << ": ";
    for ( int j = 0; j < coef_matrix[i].size(); j++ ) {
      cout << coef_matrix[i][j] << " ";
    }
    cout << endl;
  }

  // metodo de gaus pra resolver a matriz
  vector<float> result(coef_matrix.size(), 0.0);
  result[0] = alunos_sem1;
  Gauss(coef_matrix, result);
  // Imprimindo o resultado
  cout << "Resultado:" << endl;
  for ( int i = 0; i < result.size(); i++ ) {
    cout << "Linha: " << i + 1 << ": " << result[i] << endl;
  }
  
  for( int i = 0; i < result.size(); i++){
    total_alunos += result[i];
  }
    
  return total_alunos;

}

int main() {
  vector<vector<float>> transitions;
  int alunos_sem1;
  float prob_formando = 1.0;
  float total_alunos = 0.0;

  read_input( transitions, alunos_sem1 );
  total_alunos = total_a(transitions,alunos_sem1,total_alunos);

  for( int i = 0; i < transitions.size(); i++ ) {
    prob_formando *= transitions[i][i + 1];
  }

  cout << "Matriz de transições:" << endl;
  for ( int i = 0; i < transitions.size(); i++ ) {
    cout << "Semestre " << i + 1 << ": ";
    for ( int j = 0; j < transitions[i].size(); j++ ) {
      cout << transitions[i][j] << " ";
    }
    cout << endl;
  }

  cout << "Total de alunos: " << total_alunos << endl;

  return 0;
}
