#include <iostream>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>    // random_shuffle
 
using namespace std;
typedef vector<vector<double> > M_X;

void mostrar_matrix(M_X &matrix,std::vector<string>&cad){
	cout<<"\t";
	for (int i = 0; i < cad.size(); ++i){
		cout<<cad[i]<<"\t\t";
		//printf("%s\t",cad[i] );
	}
	cout<<endl;
	for (int i = 0; i < matrix.size(); ++i){
		cout<<cad[i]<<"\t";
		//printf("%s\t",cad[i]);
		for (int j = 0; j < matrix[0].size() ; ++j){
			printf("%f\t",matrix[i][j] );
		}
		cout<<endl;
	}
}

void crear_M_X_distancia(M_X &matrix){
	matrix[0][0] =0.0;		matrix[0][1] =10.0;		matrix[0][2] =12.0;		matrix[0][3] =10.0;	matrix[0][4] =7.0;	//	matrix[0][5] =8.0;
	matrix[1][0] =10.0;		matrix[1][1] =0.0;		matrix[1][2] =4.0;		matrix[1][3] =4.0;	matrix[1][4] =13.0;	//	matrix[1][5] =14.0;
	matrix[2][0] =12.0;		matrix[2][1] =4.0;		matrix[2][2] =0.0;		matrix[2][3] =6.0;	matrix[2][4] =15.0;	//	matrix[2][5] =16.0;
	matrix[3][0] =10.0;		matrix[3][1] =4.0;		matrix[3][2] =6.0;		matrix[3][3] =0.0;	matrix[3][4] =13.0;	//	matrix[3][5] =14.0;
	matrix[4][0] =7.0;		matrix[4][1] =13.0;		matrix[4][2] =15.0;		matrix[4][3] =13.0;	matrix[4][4] =0.0;	//	matrix[4][5] =4.0;
//	matrix[5][0] =8.0;		matrix[5][1] =14.0;		matrix[5][2] =16.0;		matrix[5][3] =14.0;	matrix[5][4] =4.0;		matrix[5][5] =0.0;
}

pair<int,int> get_minimo(M_X &matrix){
	if(matrix.size()<=1){
		cout<<"\nERROR matrix pequeña\n";
		//return;
	}

	double menor =matrix[0][1];
	pair<int,int> posi =make_pair(0,1);
	for (int i = 0; i < matrix.size(); ++i){
		for (int j = 0; j < matrix[0].size() ; ++j){
			if(matrix[i][j]!=0){
				if(menor > matrix[i][j]){
					menor =matrix[i][j];
					posi =make_pair(i,j);
				}

			}
		}
	}
	return posi;
}
void actualizar(std::vector<double> v, M_X&matrix, int pos){
	for (int i = 0; i < matrix.size(); ++i){//actualiza en clumna
		for (int j = 0; j < matrix[0].size(); ++j){
			if(j==pos)
				matrix[i][j] =v[i];
		}
	}
	for (int i = 0; i < matrix[pos].size(); ++i){
		matrix[pos][i] =v[i];
	}
}
void unir_vec( M_X &matrix, pair<int, int >minimo){
	std::vector<double> v3;
	v3.resize(matrix.size()-1);
	


	for (int i = 0; i < matrix.size(); ++i){
		 matrix[i].erase (matrix[i].begin() + minimo.second);
	}
	for (int i = 0; i < v3.size(); ++i){
		if(minimo.first != i)	
			v3[i] = (matrix[minimo.first][i] + matrix[minimo.second][i])/2.0;
	}
	v3[minimo.first]=0.0;
	cout<<endl;
	matrix.erase (matrix.begin()+minimo.second);
	actualizar(v3,matrix, minimo.first);
	//mostrar_matrix(matrix);

}

void UPGMA(M_X& matrix, vector<string> &cad){
	pair <int,int> minimo ;
	string unido="";
	while(matrix.size() > 1){
		minimo =get_minimo(matrix);
		cout<<"\nSe une : "<<cad[minimo.first]<<","<<cad[minimo.second]<<endl;
		unido =cad[minimo.second];
		cad.erase (cad.begin()+minimo.second);
		unir_vec(matrix,minimo);
		cad[minimo.first] +=unido;
		mostrar_matrix(matrix,cad);
		unido ="";
	}

}


int main(int argc, char const *argv[])
{
	M_X matrix;
	vector<string> cad;// ={"A","B","C","D","E"};
	cad.push_back("A");
	cad.push_back("B");
	cad.push_back("C");
	cad.push_back("D");
	cad.push_back("E");
	//cad.push_back("F");
	int tam =cad.size();
	matrix.resize(tam);
	for (int i = 0; i < matrix.size(); ++i){
		matrix[i].resize(tam);
	}
	
	crear_M_X_distancia(matrix);
	mostrar_matrix(matrix,cad);
	UPGMA(matrix,cad);


	cout<<"\nGOOD\n";
	return 0;
}
