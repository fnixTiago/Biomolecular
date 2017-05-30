#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int sumar_v(vector<int>&v){
	int tmp=0;
	for (int i = 0; i < v.size(); ++i){
		tmp +=v[i];
	}
	return tmp;
}

string mezclar(vector<string> &v){
	string tmp="";
	for (int i = 0; i < v[0].size(); ++i){
		char inicio= v[0][0];
		for (int j = 0; j < v.size(); ++j){
			if(v[j][i]=='-')
				tmp +='-';	
		}
	}
}



void llenar_datos(map<char,map<char, int> > &datos){
	datos['A']['A'] =10;	datos['A']['G'] =-1;	datos['A']['C'] =-3;	datos['A']['T'] =-4;
	datos['G']['A'] =-1;	datos['G']['G'] = 7;	datos['G']['C'] =-5;	datos['G']['T'] =-3;
	datos['C']['A'] =-3;	datos['C']['G'] =-5;	datos['C']['C'] = 9;	datos['C']['T'] = 0;
	datos['T']['A'] =-4;	datos['T']['G'] =-3;	datos['T']['C'] = 0;	datos['T']['T'] = 8;
}

string obtener_cadena(vector<pair<char, char> >&alineados,bool pos){
	if(alineados.size()==0)
		cout<<"\nERROR ALINEACIONES VACIAS\n";
	else{
		string cadena="";
		for (int i = 0; i < alineados.size(); ++i){
			if(pos==0)
				cadena +=alineados[i].first;
			else
				cadena +=alineados[i].second;
		}
		return cadena;
	}

}


void needleman_wunch(vector<pair<char, char> >&alineados, map<char,map<char, int> > &datos, string cad1, string cad2){
	
	alineados.clear();
	vector< vector<int> > M;
	M.resize(cad1.size()+1);
	for (int i = 0; i < cad1.size()+1; ++i){
		M[i].resize(cad2.size()+1);
	}
	//llenado de vacios fila
	for (int i = 0; i < M.size(); ++i){
		M[i][0] =(-5)*i;
	}
	//llenamos de vacios las columnas
	for (int i = 0; i < M[0].size(); ++i){
		M[0][i] =(-5)*i;
	}

	//llenado de la matriz
	vector<int> v;//vector que contendra los resultados

	for (int i = 1; i < M.size(); ++i){
		for (int j = 1; j < M[0].size(); ++j){
			//cout<<"\n\t"<<i<<" "<<j<<endl;
			v.push_back(datos[cad1[i-1]][cad2[j-1]] + M[i-1][j-1]);//diagonal
			//cout<<"Diagonal: "<<datos[cad1[i-1]][cad2[j-1]] + M[i-1][j-1]<<endl;
			v.push_back(M[i-1][j] -5);//superior
			//cout<<"Superior: "<<M[i-1][j] -5<<endl;
			v.push_back(M[i][j-1] -5);//izquierdo
			//cout<<"Izquierdo: "<<M[i][j-1] -5<<endl;
			sort(v.begin(),v.end());
			M[i][j] =v[v.size()-1];
			v.clear(); 
		}
	}

	//mostraremos la matriz
/*	cout<<"\t\t";
	for (int i = 0; i < cad2.size(); ++i){
		cout<<cad2[i]<<"\t";
	}
	cout<<endl;
	cout<<"\t";
	for (int i = 0; i < M.size(); ++i)
	{
		for (int j = 0; j < M[0].size(); ++j){
			cout<<M[i][j]<<"\t";
		}
		cout<<endl;
		cout<<cad1[i]<<"\t";
	}
*/
	int i=M.size()-1;
	int j=M[0].size()-1;
	while(i>0 && j>0){
		//cout<<"\n"<<i<<" : "<<j<<"\n";
		if(M[i][j] == (datos[cad1[i-1]][cad2[j-1]] +M[i-1][j-1])){//diagonal
			alineados.push_back(make_pair(cad1[i-1],cad2[j-1]));
			i--;
			j--;
		}
		else if(M[i][j] ==(M[i-1][j] -5)){//superior
			alineados.push_back(make_pair(cad1[i-1],'-'));
			i--;
		}
		else if(M[i][j] == (M[i][j-1] -5)){//izquierda
			alineados.push_back(make_pair('-',cad2[j-1]));
			j--;
		}
		else break;
	}
	if (i>0){
		//cout<<"\n\t i: "<<i<<endl;
		for (int k1 = i; k1 > 0; --k1)
		{
			alineados.push_back(make_pair(cad1[k1],'-'));
		}
	}
	if(j>0){
		//cout<<"\n\t j: "<<j<<endl;
		for (int k1 = j; k1 >0; --k1)
		{
			alineados.push_back(make_pair('-',cad2[k1-1]));
		}
	}
	/*
	cout<<"\nALINEAMIENTO: "<<alineados.size()<<endl;
	
	for (int i = 0; i < alineados.size(); ++i){
		cout<<alineados[i].first<<"\t"<<alineados[i].second<<endl;
	}
	*/


}
int obtner_peso(vector<pair<char,char>> &v, int match, int mismatch, int gap){
	int tmp=0;
	int resultado=0;
	for (int i= 0; i < v.size(); ++i){
		if(v[i].first==v[i].second)
			resultado +=match;
		else{
			if(v[i].first=='-'){
				if(v[i].second=='-')
					resultado += 0;
				else
					resultado +=gap;
			}
			else if(v[i].second=='-')
				resultado +=gap;
			else resultado +=mismatch;
		}
	}
	return resultado;
}

//string obtener_cadena(vector<pair<char, char> >&alineados,bool pos){
int buscar_guion(string &cad,int pos_ini){
	for (int i =pos_ini; i < cad.size(); ++i){
		if(cad[i]=='-')
			return i;
	}
	return -1;
}
void agregar_guion(string &cad, int pos){
	string tmp="";
	for (int i = 0; i < cad.size(); ++i){
		if(i==(pos)){
			tmp +='-';
			tmp +=cad[i];
		}
		else tmp +=cad[i];
	}
	cad =tmp;

}

void alterar(vector<string> &v, int pos, int final){
	string tmp="";
	for (int i = 0; i <=final; ++i){
		agregar_guion(v[i], pos);
	}
}
void alterar_ultimo(vector<string> &v,vector<int> v_posi){
	int tam_i =v[0].size();//tamanio de cadena inicial
	int tam_f =v[v.size()-1].size();//ultimo tamanio
	cout<<"\ntam_i: "<<tam_i<<endl;
	cout<<"\ntam_f: "<<tam_f<<endl;
	string tmp="";
	string cad= v[v.size()-1];
	for (int i = 0, j=0; i < cad.size() ; ++i){
		if(i ==v_posi[j] && j <(tam_i- tam_f) ){
			cout<<"\nENTRO \n";
			//tmp +='-';
			tmp +=cad[i];
			j++;
		}
		tmp +=cad[i];
		

	}
	v[v.size()-1] = tmp;
}

void alinemaiento_all(vector<pair<string,string> >&tmp){
	vector<string> v;
	int pos=0;
	/*
	for (int i = 0; i < tmp.size(); ++i){
		cout<<i<<" : \n"<<"\t"<<tmp[i].first<<"\n\t"<<tmp[i].second<<endl;
	}
	*/
	//Los primeros 
	int guiones=0;
	v.push_back(tmp[0].first);
	v.push_back(tmp[0].second);

	for (int i = 1; i < tmp.size(); ++i){
		if(i==0)
			v.push_back(tmp[i].first);
		v.push_back(tmp[i].second);
	}


	vector<int> v_posi;
	for (int i = 1; i < tmp.size(); ++i){
		if(v[0]==tmp[i].first);
			//v.push_back(tmp[i].second);
		else{
			int pos_c =buscar_guion(tmp[i].first, pos);
			if(pos<pos_c){
				pos =pos_c;
				cout<<"\tPOSI: "<<pos_c<<endl;
				alterar(v,pos+guiones,i);
				//v.push_back(tmp[i].second);
				v_posi.push_back(pos);
				guiones++;
			}
		}/*
			cout<<"\n\t"<<i<<endl;
			for (int i = 0; i < v.size(); ++i){
				cout<<"\t"<<v[i]<<endl;
			*/
	}
	alterar_ultimo(v,v_posi);

	for (int i = 0; i < v.size(); ++i){
		cout<<i<<": \t"<<v[i]<<endl;
	}
}

	
void alineamiento_etrella(vector<pair<char, char> >&alineados, map<char,map<char, int> > &datos,vector<string> cadenas, int match,int mismatch,int gap){
	//creamos la matriz
	vector<vector<int> >M;
	M.resize(cadenas.size());
	for (int i = 0; i < M.size(); ++i){
		M[i].resize(cadenas.size());
	}
	vector<pair<char, char> >tmp;
	//Lllenamos la matriz
	for (int i = 0; i < M.size(); ++i){
		for (int j = 0; j < M[0].size(); ++j){
			if(i!=j){
				needleman_wunch(tmp,datos,cadenas[i],cadenas[j]);
				M[i][j] = obtner_peso(tmp,match,mismatch,gap);
				tmp.clear();
			}
			else
				M[i][j]=0;
		}
	}
	cout<<"\nMOSTRAR MATRIZ\n";
	for (int i = 0; i < M.size(); ++i)
	{
		for (int j = 0; j < M[0].size(); ++j){
			cout<<M[i][j]<<"\t";
		}
		cout<<endl;
	}

	int pos=0;
	int max =sumar_v(M[0]);//primera posicion
	cout<<"\n SUMAR filas\n";
	for (int i = 0	; i < M.size(); ++i){
		cout<<sumar_v(M[i])<<endl;
		if(max < sumar_v(M[i]) ){
			pos =i;
			max =sumar_v(M[i]);
		}
	}
	cout<<"Mejor resultado: M["<<pos<<"]: "<<max<<endl;
	vector<string> cadenas_result;
	vector<string> cadenas_result1;
	vector<pair<string,string> >step_3;
	//needleman_wunch(tmp,datos,cadenas[pos],cadenas[]);
	bool primera_vez= true;		
	for (int i = 0; i < cadenas.size(); ++i){
		if(i!=pos){
			vector<pair<char,char> >tmp;
			needleman_wunch(tmp,datos,cadenas[pos],cadenas[i]);
			reverse(tmp.begin(),tmp.end());
			if(primera_vez){
				cadenas_result.push_back(obtener_cadena(tmp,0));
				primera_vez =false;	
			}
			cadenas_result1.push_back(obtener_cadena(tmp,0));
			cadenas_result.push_back(obtener_cadena(tmp,1));
			step_3.push_back(make_pair(obtener_cadena(tmp,0), obtener_cadena(tmp,1)));
			cout<<"\nS["<<pos<<"] :\t";
			for (int i = 0; i < tmp.size(); ++i){
				cout<<tmp[i].first;
			}
			cout<<"\nS["<<i<<"] :\t";
			for (int i = 0; i < tmp.size(); ++i){
				cout<<tmp[i].second;
			}
			cout<<endl;
		}
	}
	cout<<"\n****************************\n";
	alinemaiento_all(step_3);




	/*
	cout<<"\nCADENAS RESULTANTES\n";
	for (int i = 0; i < cadenas_result.size(); ++i){
		cout<<"S"<<i<<": "<<cadenas_result[i]<<endl;
	}
	string cadena_tmp =cadenas_result1[0];
	for (int i = 1; i < cadenas_result1.size(); ++i){
		needleman_wunch(tmp,datos,cadena_tmp,cadenas_result1[i]);
		reverse(tmp.begin(),tmp.end());
		cadena_tmp = obtener_cadena(tmp,0);
	//	cout<<"\n\t"<<cadena_tmp<<endl;	
	}
	*/
//	cout<<"******************\n\t"<<cadena_tmp<<endl;
/*
	for (int i = 0; i < cadenas_result.size(); ++i){
		//for (int j = 0; j < cadenas_result.size(); ++j)
		//{
			needleman_wunch(tmp,datos,cadena_tmp,cadenas_result[i]);
			reverse(tmp.begin(),tmp.end());
			cadena_tmp = obtener_cadena(tmp,1);
			//cout<<"\n\t"<<cadena_tmp<<endl;	
		//}
		cout<<"\n\t"<<cadena_tmp;	
	}
*/
}


int main(int argc, char const *argv[])
{
	map<char,map<char, int> > datos;
	vector<pair<char,char> >alineados;
	llenar_datos(datos);
	string cad1,cad2;
	cad2 ="ACCGTCTT";
	cad1 ="CGTCTT";
	needleman_wunch(alineados, datos, cad1,cad2);
	
	//_____________________________________________

	int match,mismatch,gap;
	vector<string> cadenas,cadenas1;
	match =1;
	mismatch =-1;
	gap =-2;
	
	cadenas.push_back("CCTGCTGCAGGGG");
	cadenas.push_back("GATGTGCCG");
	cadenas.push_back("GATGTGCAG");
	cadenas.push_back("CCGCTAGCAGGGG");
	cadenas.push_back("CCTGTAGG");
	
	/*
	cadenas.push_back("ATTGCCATT");
	cadenas.push_back("ATGGCCATT");
	cadenas.push_back("ATCCAATTTT");
	cadenas.push_back("ATCTTCTT");
	cadenas.push_back("ACTGACC");
	*/
	alineamiento_etrella(alineados, datos,cadenas,match,mismatch,gap);
	//alineamiento_etrella(alineados, datos,cadenas1,match,mismatch,gap);
	cout<<"\nEXITO\n";
	return 0;
}