#include <iostream>
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <map>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>    // random_shuffle
 

#define p 0.99
#define alfa 1.0
#define beta 1.0
#define Q 1.0
#define fero_ini 0.1//feromona inicial
#define count_h 3 //cantidad de hormigas
#define itera_h 100 //cantiad de iteraciones


using namespace std;
typedef map<char,map<char, double> > M_X;

double random_0_1(){//numeros aleatorios entre 0 y 1
	return (random() / (double) RAND_MAX);
}

void mostrar_map(M_X& map){//muestra la matriz de map
	for( const auto &ptr :map){
	    for( const auto  &ptr2 :ptr.second){
	    	cout <<"M["<< ptr.first << "]["<< ptr2.first << "]: " << ptr2.second << endl;
	    }
	}
}
void crear_M_X_distancia(M_X &map_dist){
	map_dist['A']['A'] =0.0;		map_dist['A']['B'] =12.0;		map_dist['A']['C'] =3.0;		map_dist['A']['D'] =23.0;	map_dist['A']['E'] =1.0;
	map_dist['B']['A'] =12.0;		map_dist['B']['B'] =0.0;		map_dist['B']['C'] =9.0;		map_dist['B']['D'] =18.0;	map_dist['B']['E'] =3.0;
	map_dist['C']['A'] =3.0;		map_dist['C']['B'] =9.0;		map_dist['C']['C'] =0.0;		map_dist['C']['D'] =89.0;	map_dist['C']['E'] =56.0;
	map_dist['D']['A'] =23.0;		map_dist['D']['B'] =18.0;		map_dist['D']['C'] =89.0;		map_dist['D']['D'] =0.0;		map_dist['D']['E'] =87.0;
	map_dist['E']['A'] =1.0;		map_dist['E']['B'] =3.0;		map_dist['E']['C'] =56.0;		map_dist['E']['D'] =87.0;	map_dist['E']['E'] =0.0;
	
}

void crear_M_X_feromona(M_X &map_dist, M_X &mx_fero){//crea la matriz feromona 
	for( const auto &ptr :map_dist){
	    for( const auto  &ptr2 :ptr.second){
	    	if(ptr.first != ptr2.first)
	    		mx_fero[ptr.first][ptr2.first] =fero_ini;
	    	//cout <<"M["<< ptr.first << "]["<< ptr2.first << "]: " << ptr2.second << endl;
	    	else
	    		mx_fero[ptr.first][ptr2.first] =0.0;
	    }
	}
}
char obtener_posicion(M_X &v_proba, double aleatorio, char inicial){
	map<char, double> m =v_proba[inicial];
	double suma=0.0;
	double suma2=0.0;
	char posi ='-';
	for (auto &ptr2 : m){
		suma2 +=ptr2.second;
		//cout<<suma<<" < "<<aleatorio<<" <= "<<suma2<<endl;
		if(suma < aleatorio && aleatorio <= suma2){
			posi =ptr2.first;
			return posi;
		}
		else{
			suma +=ptr2.second;
		}
	}
}

bool find_ele(vector<char> &v, char value){
	vector<char>::iterator it =v.begin();
	it =find (v.begin(), v.end(), value);
	if (it != v.end())//encontrado
		return true;
	else//no encontrado
	    return false;	
}
void buscar_camino(M_X &map_dist, M_X &mx_fero, vector<char> &visitados){
	char visi_ini= '-';
	double t,n;
	double suma=0.0;
	M_X v_proba;//vector probabilidad
	double suma_cant;
	double n_random; 
	map<char, double> camino_ini;//camino inicial
	while(visitados.size() < map_dist.size()){
		visi_ini =visitados[visitados.size()-1];
		cout<<"\nCIUDAD: "<<visi_ini<<endl;
		camino_ini= map_dist[visi_ini];
		for (auto &ptr2: camino_ini){
			if(!find_ele(visitados, ptr2.first)){
			
				t =pow (mx_fero[visi_ini][ptr2.first],alfa);
				n =pow (1.0/map_dist[visi_ini][ptr2.first], beta);
				cout<<visi_ini<<"-"<<ptr2.first<<":\t t= "<<t<<"\tn = "<<n
					<<" \t t*n = "<<t*n;
				v_proba[visi_ini][ptr2.first] = t*n;
				suma +=t*n;
				cout<<endl;
			}
		}
		cout<<"Suma: "<<suma<<endl;
		suma_cant=0.0;
		for( const auto &ptr :v_proba){
		    for( const auto  &ptr2 :ptr.second){
		    	v_proba[ptr.first][ptr2.first] /= suma;
		    	//ptr2.second /=suma;
		    	cout << ptr.first << "-"<< ptr2.first << ": prob = " << ptr2.second <<
		    	 "\t("<<suma_cant<<" - "<<suma_cant + ptr2.second<<")"<<endl;
				suma_cant += ptr2.second;
		    }
		}

		n_random =random_0_1();
		cout<<"Randon: "<<n_random<<endl;
		visi_ini =obtener_posicion(v_proba, n_random,visi_ini);
		cout<<"\tCiudad Siguiente: "<<visi_ini<<endl;
		cout<<"________________________________________\n";
		visitados.push_back(visi_ini);
		camino_ini.clear();
		v_proba.clear();
		suma=t=n=0.0;
	}

}
double costo_h(vector<char> &visitados, M_X &map_dist){
	 double suma=0.0;
	 for (int i = 0; i < visitados.size()-1; ++i){
		suma += map_dist[visitados[i] ][visitados[i+1] ];
	}
	return suma;
}

void mostrar_caminos(vector<vector<char> > &all_visitados, M_X &map_dist){
	for (int i = 0; i < all_visitados.size(); ++i){
		cout<<"HOrmiga: "<<i<<"\t";
		for (int j = 0; j < all_visitados[0].size(); ++j){
			cout<<all_visitados[i][j]<<"-";
		}
		cout<<"\t Costo: "<<costo_h(all_visitados[i],map_dist)<<endl;
	}
}

bool buscar_arista(std::vector<char> v, char c1, char c2){
	for (int i = 0; i < v.size()-1; ++i){
		if(v[i]==c1 && v[i+1]==c2 || v[i]==c2 && v[i+1]==c1)
			return true;
	}
	return false;
}
void actualizar_feromonas(M_X &mx_fero, vector<vector<char> > &all_visitados, M_X &map_dist){
	double suma=0.0;
	for( const auto &ptr :mx_fero){
	    for( const auto  &ptr2 :ptr.second){
	    	for (int i = 0; i < all_visitados.size(); ++i){
	    		if(buscar_arista(all_visitados[i],ptr.first , ptr2.first) && ptr.first != ptr2.first)
	    			suma += Q*1.0/costo_h(all_visitados[i], map_dist);
	    	}
	    	if(ptr.first != ptr2.first )
	    		mx_fero[ptr.first][ptr2.first] =mx_fero[ptr.first][ptr2.first]*p + suma;
	    	//ptr2.second =ptr2.second*p + suma;
	    	cout << ptr.first << "-"<< ptr2.first << ": feromona = " << ptr2.second << endl;
	    }
	    suma=0.0;
	}
}

void get_global( vector<vector<char> > &all_visitados,std::vector<char> &global,M_X &map_dist){
	int posi_menor=-1;
	double costo_menor =costo_h(all_visitados[0],map_dist);
	double costo_actual;

	if (global.size()==0){
		global.resize(all_visitados[0].size());
		for (int i = 0; i < all_visitados.size(); ++i){
			costo_actual = costo_h(all_visitados[i],map_dist);
			if(costo_menor > costo_actual ){
				posi_menor =i;
				costo_menor =costo_actual; 
			}
		}
	}
	else{
		costo_menor =costo_h(global,map_dist);
		for (int i = 0; i < all_visitados.size(); ++i){
			costo_actual = costo_h(all_visitados[i],map_dist);
			if(costo_menor > costo_actual ){
				posi_menor =i;
				costo_menor =costo_actual; 
			}
		}

	}
	if(posi_menor >= 0){//cuidado si no entra a los bucles es -1
		for (int i = 0; i < all_visitados[0].size(); ++i){
			global[i] =all_visitados[posi_menor][i];
		}
	}
}

void AS(M_X &map_dist, M_X &mx_fero,char inicial){
	int i=0;
	vector<char> visitados;
	vector<vector<char> > all_visitados;
	std::vector<char> global;
	while(i <= 100){//iteraciones de las hormigas
		cout<<"\nIteracion: "<<i<<endl;
		for (int i = 0; i < count_h ; ++i){//todos los caminos de las hormigas
			cout<<"Hormiga "<<i<<endl;
			cout<<"Ciudad inicial: "<<inicial<<endl;
			visitados.push_back(inicial);
			buscar_camino(map_dist, mx_fero,visitados);
			all_visitados.push_back(visitados);
			visitados.clear();
			cout<<"\n****************************\n";
		}
		mostrar_caminos(all_visitados, map_dist);
		actualizar_feromonas(mx_fero, all_visitados, map_dist);
		
		get_global(all_visitados, global,map_dist);
		cout<<"\nMejor Hormiga Global: ";
		for (int i = 0; i < global.size(); ++i){
			cout<<global[i]<<"-";
		}
		cout<<"\tCosto: "<<costo_h(global,map_dist)<< endl;


		i++;
		for (int i = 0; i < all_visitados.size(); ++i){
			all_visitados[i].clear();
		}
		all_visitados.clear();
	}

	//cout<<"\ntamnio: "<<map_dist.size()<<endl;
}


int main(int argc, char const *argv[])
{
	M_X map_dist;
	M_X mx_fero;
	crear_M_X_distancia(map_dist);
	crear_M_X_feromona(map_dist, mx_fero);
	AS(map_dist,mx_fero,'D');

	




	//mostrar_map(mx_fero);
	cout<<"\nGOOD\n";
	return 0;
}
