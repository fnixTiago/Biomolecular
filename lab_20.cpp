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

void crear_M_X_distancia(M_X &matrix, int nxm){
	matrix.resize(nxm);
	for (int i = 0; i < matrix.size(); ++i){
		matrix[i].resize(nxm);
	}

	matrix[0][0] =0.0;		matrix[0][1] =5.0;		matrix[0][2] =4.0;		matrix[0][3] =7.0;	matrix[0][4] =6.0;		matrix[0][5] =8.0;
	matrix[1][0] =5.0;		matrix[1][1] =0.0;		matrix[1][2] =7.0;		matrix[1][3] =10.0;	matrix[1][4] =9.0;		matrix[1][5] =11.0;
	matrix[2][0] =4.0;		matrix[2][1] =7.0;		matrix[2][2] =0.0;		matrix[2][3] =7.0;	matrix[2][4] =6.0;		matrix[2][5] =8.0;
	matrix[3][0] =7.0;		matrix[3][1] =10.0;		matrix[3][2] =7.0;		matrix[3][3] =0.0;	matrix[3][4] =5.0;		matrix[3][5] =9.0;
	matrix[4][0] =6.0;		matrix[4][1] =9.0;		matrix[4][2] =6.0;		matrix[4][3] =5.0;	matrix[4][4] =0.0;		matrix[4][5] =8.0;
	matrix[5][0] =8.0;		matrix[5][1] =11.0;		matrix[5][2] =8.0;		matrix[5][3] =9.0;	matrix[5][4] =8.0;		matrix[5][5] =0.0;
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

pair<int,int> get_menor(M_X&matrix, std::vector<double> v_sumatoria, vector<string>&cad){
	M_X matrix2;
	matrix2.resize(matrix.size());
	for (int i = 0; i <matrix2.size() ; ++i){
		matrix2[i].resize(matrix.size());
	}

	for (int i = 0; i < matrix.size(); ++i){
		for (int j = 0; j < matrix[0].size(); ++j){
			if(i!=j)
				matrix2[i][j] =matrix[i][j] -(v_sumatoria[i] + v_sumatoria[j]);
		}
	}
	pair<int,int> minimo =get_minimo(matrix2);
	cout<<"\nMATRIX2\n";
	mostrar_matrix(matrix2,cad);
	cout<<"\n\tminimos: "<<minimo.first<<","<<minimo.second<<endl;

	/*multimap<double, pair<int,int> >map_menores;
	double result=0.0;
	for (int i = 0; i < matrix.size()-1; ++i){
		result = matrix[i][i+1] - (v_sumatoria[i] + v_sumatoria[i+1]);
		map_menores.insert(make_pair(result, make_pair(i,i+1)));
	}
	for (auto &it:map_menores){
		cout<<it.first<<" : ("<<(it.second).first<<","<<(it.second).second<<")\n";
	}
	cout<<"\nEscogemos al primero\n";
	for (auto &it:map_menores){
		cout<<it.first<<" : ("<<(it.second).first<<","<<(it.second).second<<")\n";
		return it.second;
	}
*/
	//destruimos la matriz creada
	for (int i = 0; i < matrix2.size(); ++i){
		matrix2[i].clear();
	}

	matrix2.clear();

	return minimo;
	

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
	

	int ik =minimo.first;
	int jk =minimo.second;
	double ele =matrix[ik][jk];

	for (int i = 0; i < matrix.size(); ++i){
		 matrix[i].erase (matrix[i].begin()+minimo.second);
	}
	for (int i = 0; i < v3.size(); ++i){
		if(i!=minimo.first){
			v3[i] = (matrix[ik][i] + matrix[jk][i] - ele)/2.0;
	//			cout<< matrix[ik][i]<<" + "<<matrix[jk][i]<<" - "<<ele<<"/2.0\n";
		}
	}
	/*

	cout<<"\n*************+\n";
	for (int i = 0; i < v3.size(); ++i)
	{
		cout<<v3[i]<<endl;
	}

	cout<<endl;
	*/matrix.erase (matrix.begin()+minimo.second);
	actualizar(v3,matrix, minimo.first);
	//mostrar_matrix(matrix);

}


void NJ(M_X &matrix , std::vector<string>&cad){
	std::vector<double> v_sumatoria;
	
	pair<int,int> minimo;

	string unido="";

	while(matrix.size() > 2){
		
		v_sumatoria.resize(matrix.size());
		for (int i = 0; i < matrix.size(); ++i){
			for (int j = 0; j < matrix[0].size() ; ++j){
				v_sumatoria[i] +=matrix[i][j]/(matrix.size()-2);
			}
		}
		cout<<"\nMostrar vector divergencia: \n";
		for (int i = 0; i < v_sumatoria.size(); ++i){
			cout<<"\tr("<<cad[i]<<") : "<<v_sumatoria[i]<<endl;
		}
		cout<<"\nElementos menores\n";
		minimo =get_menor(matrix,v_sumatoria,cad);
		int ik =minimo.first;
		int jk =minimo.second;
		
		cout<<"S("<<cad[ik]<<"U) ="<< matrix[ik][jk]/2.0 + (v_sumatoria[ik] - v_sumatoria[jk])/2.0<<endl;
		cout<<"S("<<cad[jk]<<"U) ="<< matrix[ik][jk]/2.0 +  (v_sumatoria[jk] - v_sumatoria[ik])/2.0<<endl;
		

		
		//mostrar_matrix(matrix,cad);
		unir_vec(matrix,minimo);

		unido =cad[minimo.second];
		cad.erase (cad.begin()+minimo.second);
		//unir_vec(matrix,minimo);
		cad[minimo.first] +=unido;
		unido="";
		mostrar_matrix(matrix, cad);
		v_sumatoria.clear();
		//break;

	}

}

int main(int argc, char const *argv[])
{
	M_X matrix;
	crear_M_X_distancia(matrix,6);
	vector<string> cad;
	cad.push_back("A");
	cad.push_back("B");
	cad.push_back("C");
	cad.push_back("D");
	cad.push_back("E");
	cad.push_back("F");
	mostrar_matrix(matrix, cad);
	NJ(matrix,cad);

	cout<<"\nGOOD\n";	
	return 0;
}