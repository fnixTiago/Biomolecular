#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

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
pair<string, string> needleman_wunch(string cad1, string cad2){
	map<char,map<char, int> > datos;
	llenar_datos(datos);
	vector<pair<char, char> > alineados;
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
	reverse(alineados.begin(), alineados.end());
	pair<string,string> ali;
	ali =make_pair(obtener_cadena(alineados,0),obtener_cadena(alineados,1));
	return ali;


}

int get_peso(string cad1,string cad2,int gap, int mismatch){
	if(cad1.size()!=cad2.size())
		cout<<"\nERROR CADENAS DE DIFERENTE TAM\n";
	else{
		int tmp=0;
		for (int i = 0; i < cad1.size(); ++i){
			if(cad1[i]==cad2[i])
				tmp += 0;
			else{
				if(cad1[i]=='-'){
					if(cad2[i] != '-')
						tmp +=gap;
				}
				else if(cad2[i] =='-'){
					if(cad1[i] != '-')
						tmp +=gap;
				}
				else tmp+=mismatch;

			}
		}
		return tmp;
	}

}
int get_peso2(char c1,char c2, int gap, int mismatch){
	int tmp=0;
	//primero -ultimo
	if(c1==c2)
		tmp += 0;
	else{
		if(c1=='-'){
			if(c2 != '-')
				tmp +=gap;
		}
		else if(c2 =='-'){
			if(c1 != '-')
				tmp +=gap;
		}
		else tmp+=mismatch;

	}
	return tmp;
}
int get_peso3(char c1,char c2,char c3, int g,int m){
	int tmp =0;
	tmp +=get_peso2(c1,c3,g,m);
	tmp +=get_peso2(c2,c3,g,m);
	return tmp;
}

void crear_matriz(vector<string>&data,  pair<pair<string, string>, pair<int,int> > &par_menor,int gap,int mismatch){
	int pos_fil=-1;
	int i_0 = (par_menor.second).first;//posicion de la 1ra cadena
	int i_1 = (par_menor.second).second;//posicion de la 2da cadena
	for (int i = 0; i < data.size(); ++i){
		if(i != i_0 && i != i_1)
		pos_fil =i; 
	}
	cout<<"\npos_fil: "<<pos_fil<<endl;

	vector<vector<int> > M;
	M.resize(data[pos_fil].size()+1);
	for (int i = 0; i < M.size(); ++i){
		M[i].resize((par_menor.first).first.size()+1);
	}
	vector<string> data_tmp =data;
	data[i_0]  ="-" +(par_menor.first).first;
	data[i_1] ="-" +(par_menor.first).second;
	data[pos_fil] ="-" +data[pos_fil];

	//llenado de la matriz
	//llenado de filas
	int anterior=0;
	for (int i = 0; i < M.size(); ++i){
		M[i][0] =anterior + get_peso3('-','-',data[pos_fil][i],gap,mismatch);
		anterior =M[i][0];

	}
	anterior =0;	
	//llenado de columnas
	for (int i = 0; i < M[0].size(); ++i){
		M[0][i] =anterior + get_peso3(data[i_0][i],data[i_1][i],'-',gap,mismatch);
		anterior =M[0][i];	
	}
	int vacio=M[0][1];

	//llenado total de la matriz
	int suma=0;
	vector<int> res;//resulado de diagonal,superior,izquierdo
	for (int i = 1; i < M.size(); ++i){
		for (int j = 1; j < M[0].size(); ++j){
			

			//int anterior_ =get_peso3(data[i_0][j-1],data[i_1][j-1],data[pos_fil][i-1],gap,mismatch);
			int actual  =get_peso3(data[i_0][j],data[i_1][j],data[pos_fil][i],gap,mismatch);
			//suma = M[i-1][j-1] + get_peso3(data[i_0][j],data[i_1][j],data[pos_fil][i-1],gap,mismatch);
			//cout<<"\nsuma ="<<data[i_0][j]<<","<<data[pos_fil][i]<<" + "<<data[i_1][j]<<","<<data[pos_fil][i-1]<<" = "<<suma<<endl;
			
			//cout<<"\nAnteior: "<<anterior_<<endl;
			//cout<<"\nActual: "<<actual_<<endl;
			//suma =anterior_ + actual_;
			res.push_back( actual+ M[i-1][j-1]);//diagonal
			cout<<"\n\tDiagonal: "<<actual + M[i-1][j-1]<<endl;
			
			res.push_back(vacio + M[i-1][j]);//superior
			cout<<"\tSuperior: "<<vacio + M[i-1][j]<<endl;
			
			res.push_back(vacio + M[i][j-1]);//izquierdo
			cout<<"\tIzquierdo: "<<vacio + M[i][j-1]<<endl;
			
			sort(res.begin(), res.end());
			for (int i = 0; i < res.size(); ++i){
				cout<<res[i]<<"\t";
			}
			cout<<endl;
			M[i][j] = res[0];
			res.clear();		
		}
	}


/*

	M[1][1]= 0;	M[1][2]= 4;	M[1][3]= 8;	M[1][4]=12;	M[1][5]=16;	M[1][6]=20;
	M[2][1]= 4;	M[2][2]= 3;	M[2][3]= 7;	M[2][4]= 8;	M[2][5]=12;	M[2][6]=16;
	M[3][1]= 8;	M[3][2]= 7;	M[3][3]=9;	M[3][4]=12;	M[3][5]=14;	M[3][6]=18;
	M[4][1]=12;	M[4][2]=11;	M[4][3]=7;	M[4][4]=11;	M[4][5]=15;	M[4][6]=14;
	M[5][1]=16;	M[5][2]=15;	M[5][3]=11;	M[5][4]= 7;	M[5][5]=11;	M[5][6]=15;
	M[6][1]=20;	M[6][2]=19;	M[6][3]=15;	M[6][4]=11;	M[6][5]=13;	M[6][6]=17;
	M[7][1]=24;	M[7][2]=23;	M[7][3]=19;	M[7][4]=15;	M[7][5]=17;	M[7][6]=13;
*/
	for (int i = 0; i < M.size(); ++i){
		for (int j = 0; j < M[i].size(); ++j){
			cout<<M[i][j]<<"\t";	
		}
		cout<<endl;
	}
	vacio =M[2][1];
	vector<tuple<char, char, char>> progresive;
	int i=M.size()-1;
	int j=M[0].size()-1;
	while(i>0 && j>0){
		char c1,c2,c3;
		
		int pesoT =get_peso3(data[i_0][j],data[i_1][j],data[pos_fil][i],gap,mismatch);
		if(M[i][j]== (pesoT+ M[i-1][j-1]) && (M[i][j]!= (M[i][j-1] + vacio)) && (M[i][j]!= (M[i-1][j] + vacio)) ){ //diagonal
			c1 =data[i_0][j];
			c2 =data[i_1][j];
			c3 =data[pos_fil][i];
			tuple<char,char,char> foo (c1,c2,c3);
			progresive.push_back(foo);
			cout<<"Diagonal: ("<<i<<","<<j<<")-> ";
			i--;
			j--;
			cout<<"("<<i<<","<<j<<")\n";
		}
		else if(M[i][j]== (M[i][j-1] + vacio)){//izquierdo
			c1 =data[i_0][j];
			c2 =data[i_1][j];
			c3 =data[pos_fil][i];
			tuple<char,char,char> foo(c1,c2,'-');
			progresive.push_back(foo);
			cout<<"Izquierdo: ("<<i<<","<<j<<")-> ";
			//alineados.push_back(make_pair('-',cad2[j-1]));
			j--;
			cout<<"("<<i<<","<<j<<")\n";
		}
		else if(M[i][j]== (M[i-1][j]+vacio)){//superior
			c1 =data[i_0][j];
			c2 =data[i_1][j];
			c3 =data[pos_fil][i];
			tuple<char,char,char> foo('-','-',c3);
			progresive.push_back(foo);
			cout<<"Superior: ("<<i<<","<<j<<")-> ";
			i--;
			cout<<"("<<i<<","<<j<<")\n";
		}
		cout<<endl;
	}
	reverse(progresive.begin(), progresive.end());
	for(tuple<char, char, char> t: progresive)
	{
	    std::cout << std::get<2>(t) << ' ';
	};
	cout<<endl;
	for(tuple<char, char, char> t: progresive)
	{ 
	    std::cout << std::get<1>(t) << ' '; 
	};
	cout<<endl;
	for(tuple<char, char, char> t: progresive)
	{
	    std::cout << std::get<0>(t) << ' ';
	} 
	cout<<endl;


}


void TPS(vector<string>&M, int gap,int mismatch){
	cout<<"\nCADENAS INGRESADAS\n";
	for (int i = 0; i < M.size(); ++i){
		cout<<M[i]<<endl;
	}
	std::vector<pair<pair<string, string>, pair<int,int> > > v;//cadenas<>,posiciones<>
	//int cant =M.size()*(M.size()-1)/2;
	//v.resize(cant);
	pair<string,string> tmp;
	for (int i = 0; i < M.size(); ++i){
		for (int j = i+1; j < M.size(); ++j){
			tmp = needleman_wunch(M[i],M[j]);
			v.push_back(make_pair(tmp,make_pair(i,j)) );
		}
	}
	for (int i = 0; i < v.size(); ++i){
		cout<<i<<endl;
		cout<<"\t"<<(v[i].first).first<<endl;
		cout<<"\t"<<(v[i].first).second<<endl;
	}

	pair<pair<string, string>, pair<int,int> > par_menor =v[0];
	pair<int, int >posiciones =v[0].second;
	int min=get_peso((v[0].first).first, (v[0].first).second ,gap, mismatch);
	cout<<"\nPESOS RESPECTIVOS\n";
	for (int i = 0; i < v.size(); ++i){
		int peso =get_peso((v[i].first).first, (v[i].first).second ,gap, mismatch);
		cout<<peso<<endl;
		if(min > peso){
			par_menor =v[i];
			min = peso;
			posiciones =v[i].second;
		}
	}
	cout<<"\nPESO MENOR: "<<min<<" :: < "<<posiciones.first<<","<<posiciones.second<<" >\n";
	crear_matriz(M,par_menor,gap,mismatch);	

}



int main(int argc, char const *argv[])
{
	//vector<pair<char,char> >alineados;
	vector<string> M;
	int gap,mismatch;
	M.push_back("ACTCAT");
	M.push_back("AGTCAT");
	M.push_back("ACGTCCT");
	mismatch =3;
	gap =2;



	TPS(M,gap,mismatch);

/*
	needleman_wunch(alineados,cad1,cad3);
	reverse(alineados.begin(),alineados.end());
	cout<<"\nALINEAMIENTO: "<<alineados.size()<<endl;
	
	for (int i = 0; i < alineados.size(); ++i)
		cout<<alineados[i].first;
	cout<<endl;
	for (int i = 0; i < alineados.size(); ++i)
		cout<<alineados[i].second;
	
*/


	cout<<"\nEXITO\n";
	return 0;
}

