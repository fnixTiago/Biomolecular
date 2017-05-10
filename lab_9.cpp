#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;
typedef char T;

void generar_matriz(vector<vector<int> > &M, string cad1, string cad2,int match, int mismatch, int gap_penalty){
	M.resize(cad1.size()+1);
	for (int i = 0; i < M.size(); ++i){
		M[i].resize(cad2.size()+1);
	}
	//inicializar filas
	for (int i = 0; i < M.size(); ++i)
		M[i][0] =0;

	//inicializar columnas
	for (int i = 0; i < M[i].size(); ++i)
		M[0][i] =0;

	vector<int> v;
	//creamos la matriz
	for (int i = 1; i < M.size(); ++i){
		for (int j = 1; j < M[i].size(); ++j){
			v.push_back(0);
			int score =(cad1[i-1]==cad2[j-1])?match:mismatch;
			v.push_back(M[i-1][j-1] + score);//diagonal
			v.push_back(M[i-1][j] + gap_penalty);//derecho
			v.push_back(M[i][j-1] + gap_penalty);//izquierdo
			sort(v.begin(), v.end());//ordeno
			M[i][j] =v[v.size()-1];
			v.clear();
			score =0;
		}
	}
	cout<<"\nMOSTRAR\n";
	cout<<"\t";
	for (int i = 0; i < cad2.size(); ++i){
		printf("\t%c", cad2[i]);
	}
	cout<<endl;
	cout<<"\t";
	for (int i = 0; i < M.size(); ++i){
		for (int j = 0; j < M[i].size(); ++j){
			printf("%d\t", M[i][j]);
		}
		cout <<endl;
		cout<<cad1[i]<<"\t";
	}
	cout<<endl;
}

int position_max(vector<int>&v){
	int max =v[0];
	int position =0;
	for (int i = 1; i < v.size(); ++i){
		if(max <v[i]){
			max =v[i];
			position =i;
		}
	}
	return position;
}

void alineamiento(vector<vector<int> > &M, string cad1, string cad2, int match, int mismatch, int gap_penalty){
	//encontrar el maximo
	int position_mayor =position_max(M[M.size()-1]);
	
	vector<pair<char,char>> alineado;//va contener las alineaciones de las 2 cadenas
	int i =M.size()-1;//recuenda que se empieza desde 0
	int j =position_mayor;
	pair<char,char> pareja;
	while(i>0 && j>0){
		int score =(cad1[i-1]==cad2[j-1])?match:mismatch;
		if((M[i-1][j-1] + score)== M[i][j]){//diagonal
			pareja =make_pair(cad1[i-1],cad2[j-1]);
			alineado.push_back(pareja);
			//cout<<"\nDIAGONAL\n";
			i--;
			j--;
		}
		else if((M[i-1][j] + gap_penalty)== M[i][j]){//arriba
			pareja =make_pair(cad1[i-1],'_');
			alineado.push_back(pareja);
			//cout<<"\nARRIBA\n";
			i--;
		}
		else if((M[i][j-1] + gap_penalty)== M[i][j]){//izquierdo
			pareja =make_pair('_',cad2[j-1]);
			alineado.push_back(pareja);
			//cout<<"\nIZQUIERDO\n";
			j--;
		}
		else break;
	}

	for (int i = 0; i < alineado.size(); ++i){
		cout<<alineado[i].first<<" - "<<alineado[i].second<<endl;	
	}
	//limpieza de la matriz
	for (int i = 0; i < M.size(); ++i)
	{
		M[i].clear();
	}
}

int main(){
	vector<vector<int> > M;//matriz
	int match,mismatch,gap_penalty;
	string cad1,cad2;

	cout<<"\nEjemplo 1\n";
	cad1= "GAT";
	cad2= "GGAT";
	match = 1;
	mismatch = -1;
	gap_penalty = -1;
	generar_matriz(M,cad1,cad2,match,mismatch,gap_penalty);
	alineamiento(M,cad1,cad2,match,mismatch,gap_penalty);

	cout<<"\nEjemplo 2\n";
	cad1= "GGATCGA";
	cad2= "GAATTCAGTTA";
	match = 5;
	mismatch = -3;
	gap_penalty = -4;
	generar_matriz(M,cad1,cad2,match,mismatch,gap_penalty);
	alineamiento(M,cad1,cad2,match,mismatch,gap_penalty);

	cout<<"\nEjemplo 3\n";
	cad1= "GTACGTATC";
	cad2= "CAGTATCGT";
	match = 1;
	mismatch = -1;
	gap_penalty = -2;
	generar_matriz(M,cad1,cad2,match,mismatch,gap_penalty);
	alineamiento(M,cad1,cad2,match,mismatch,gap_penalty);
	return 0;
}


