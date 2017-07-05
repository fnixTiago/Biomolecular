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

void mostrar_matrix(M_X &matrix){
	for (int i = 0; i < matrix.size(); ++i){
		for (int j = 0; j < matrix[0].size() ; ++j){
			printf("%f\t",matrix[i][j] );
		}
		cout<<endl;
	}
}

void crear_M_X_distancia(M_X &matrix){
	matrix[0][0] =0.0;		matrix[0][1] =10.0;		matrix[0][2] =12.0;		matrix[0][3] =10.0;	matrix[0][4] =7.0;
	matrix[1][0] =10.0;		matrix[1][1] =0.0;		matrix[1][2] =4.0;		matrix[1][3] =4.0;	matrix[1][4] =13.0;
	matrix[2][0] =12.0;		matrix[2][1] =4.0;		matrix[2][2] =0.0;		matrix[2][3] =6.0;	matrix[2][4] =15.0;
	matrix[3][0] =10.0;		matrix[3][1] =4.0;		matrix[3][2] =6.0;		matrix[3][3] =0.0;	matrix[3][4] =13.0;
	matrix[4][0] =7.0;		matrix[4][1] =13.0;		matrix[4][2] =15.0;		matrix[4][3] =13.0;	matrix[4][4] =0.0;
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
	cout<<"\n1ro: "<<posi.first<<","<<posi.second<<endl;
	return posi;
}

vector<double> unir_vec(std::vector<double> &v1, std::vector<double> &v2){
	std::vector<double> v3;
	v3.resize(v1.size()-2);
	for (int i = 0; i < v1.size(); ++i){
		if(v1[i]==0 || v2[i]==0)
			v3[i] =0;
		else
			v3[i] =(v1[i] + v2[i])/2.0;
	}
	return v3;
}

void unir(M_X&matrix, pair<int,int> minimo){
	M_X matrix2;
	matrix2.resize(matrix.size()-1);
	for (int i = 0; i < matrix2.size(); ++i){
		matrix2[i].resize(matrix.size()-1);
	}
	
	std::vector<double> v= unir_vec(matrix[minimo.first], matrix[minimo.second]);
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<"\t";
	}
	cout<<endl;

	
/*
	for (int i = 0; i < matrix.size(); ++i){
		for (int j = 0; j < matrix[0].size(); ++j){
			//if(i==minimo.first || j==minimo.first)
		}
	}
*/
}

int main(int argc, char const *argv[])
{
	M_X matrix;
	matrix.resize(5);
	for (int i = 0; i < matrix.size(); ++i){
		matrix[i].resize(5);
	}
	
	crear_M_X_distancia(matrix);
	mostrar_matrix(matrix);
	pair <int,int> minimo =get_minimo(matrix);
	unir(matrix,minimo);


	cout<<"\nGOOD\n";
	return 0;
}
