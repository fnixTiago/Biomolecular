#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
//ALGORITMO BLAST
using namespace std;
#define W 3//Word  cantidad de letras en la que será divido

void dividir(vector<string> &v, string cad){
	string tmp="";
	for (int i = 0; i < cad.size() -W +1; ++i){
		for (int j = 0; j < W; ++j){
			tmp +=cad[i+j];
		}
		v.push_back(tmp);
		tmp="";
	}
	/*
	for (int i = 0; i < v.size(); ++i){
		cout<<v[i]<<"\n";
	}*/
}
string search_word(vector<string> v1, vector<string> v2){
	vector<string> ::iterator it;
	for (int i = 0; i < v1.size(); ++i){
		it =find(v2.begin(), v2.end(), v1[i]);
		if(it != v2.end())
			return v1[i];
	}
	return "";
}

void posicion_word(vector<string> &v,string word, int &ini, int &fin){
	int posicion=-1;
	for (int i = 0; i < v.size(); ++i){
		if(v[i]==word){
			posicion =i+W;
			break;
		}
	}
	if(posicion >0){
		ini =posicion-W;
		fin =posicion-1;
		
	}
	else{
		cout<<"\nERROR letra no encontrada\n";
	}
}

void expansion(vector<pair<char,char> > &v){
	for (int i = 0; i < v.size(); ++i){
		if(v[i].first == v[i].second)
			cout<<v[i].first<<" _ "<<v[i].second<<endl;
		else
			cout<<v[i].first<<"   "<<v[i].second<<endl;
	}

}
void alineamiento(string cad1,string cad2, int match,int mismatch,int U){
	vector<string> v1;
	vector<string> v2;
	string word_find ="";
	int ini1,ini2,fin1,fin2;

	dividir(v1,cad1);
	cout<<"\nNuevo\n";
	dividir(v2,cad2);
	word_find =search_word(v1,v2);
	if(word_find=="") cout<<"\nERROR palabra no encontrada\n";
	cout<<"\nPalabra encontrada: "<<word_find<<endl;
	posicion_word(v1,word_find,ini1,fin1);
	//cout<<"\nIni1:"<<ini1<<"\nFin1:"<<fin1<<endl;
	posicion_word(v2,word_find,ini2,fin2);
	//cout<<"\nIni2:"<<ini2<<"\nFin2:"<<fin2<<endl;

	vector<pair<char,char> > v_hsp;
	multimap<vector<pair<char,char> > ,int> all;
	pair<char,char> pareja;
	int HSP=0;
	while(ini1>=0 && ini2>=0 && fin1 <= (cad1.size()) && fin2<=(cad2.size()) ){
		for (int i = ini1, j=ini2; i <=fin1 && j<=fin2 ; ++i, ++j){
			HSP +=(cad1[i]==cad2[j])?match:mismatch;
			pareja =make_pair(cad1[i],cad2[j]);
			v_hsp.push_back(pareja);
		}/*
		cout<<"\nIni1:"<<ini1<<"\nFin1:"<<fin1<<endl;
		cout<<"\nIni2:"<<ini2<<"\nFin2:"<<fin2<<endl;
		cout<<"\nHSP: "<<HSP<<endl;
		*/all.insert(pair<vector<pair<char,char> > ,int>(v_hsp, HSP));
		if(ini1>0 && ini2>0){
			ini1--;
			ini2--;
		}

		if(fin1 <= (cad1.size()-1) && fin2<=(cad2.size()-1)){
			fin1++;
			fin2++;
		}
		if(HSP != match*W && U>=HSP)
			break;
		if (fin1 >= (cad1.size()) || fin2 >=(cad2.size()))
			break;

		v_hsp.clear();
		HSP =0;

	}

	//mostraremos el alineamiento
	cout<<"\nTamanio: "<<all.size()<<endl;
	for (auto& x: all){
    	cout << "[" << x.second<<"]";
    	v_hsp =x.first;
    	for (int i = 0; i < v_hsp.size(); ++i){
    		cout<<"\n\t"<<v_hsp[i].first<<" _ "<<v_hsp[i].second;
    	}
    	cout<<"\n";
	}
	
	//buscamos al menor
	v_hsp.clear();
	bool salir=false;
	for (int i = U,itera =0 ;itera <all.size(); ++i,itera++){
		for (auto& x: all){
			if(i==x.second){
				//cout<<"\n\t::"<<i<<endl;
				v_hsp =x.first;
				salir=true;
				break;
			}
		}
		if (salir)
			break;
	}
	if(v_hsp.size() !=0)
		expansion(v_hsp);


}


int main(int argc, char const *argv[])
{
	string cad1,cad2;
	int match,mismatch;
	int HSP;//Son como los scores
	int U;//umbral: indica hasta cuantas posiciones se mueve
	cad1 ="TCAGATCACGG";
	cad2 ="GTATCGCTC";
	match = +1;
	mismatch = -2;
	U =-3;
	alineamiento(cad1,cad2,match,mismatch,U);
	cout<<"\nEXITO\n";
	return 0;
}
