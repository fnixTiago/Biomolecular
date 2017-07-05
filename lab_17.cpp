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
#define q0 0.7
#define phi 0.05
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
	    	cout <<"M["<< ptr.first << "]["<< ptr2.first << "]: ";printf("%f\n",ptr2.second);
	    }
	}
}
void crear_M_X_distancia(M_X &map_dist){
	map_dist['A']['A'] =0.0;		map_dist['A']['B'] =12.0;		map_dist['A']['C'] =3.0;		map_dist['A']['D'] =23.0;	map_dist['A']['E'] =1.0;	map_dist['A']['F'] =5.0;
	map_dist['B']['A'] =12.0;		map_dist['B']['B'] =0.0;		map_dist['B']['C'] =9.0;		map_dist['B']['D'] =18.0;	map_dist['B']['E'] =3.0;	map_dist['B']['F'] =41.0;
	map_dist['C']['A'] =3.0;		map_dist['C']['B'] =9.0;		map_dist['C']['C'] =0.0;		map_dist['C']['D'] =89.0;	map_dist['C']['E'] =56.0;	map_dist['C']['F'] =21.0;
	map_dist['D']['A'] =23.0;		map_dist['D']['B'] =18.0;		map_dist['D']['C'] =89.0;		map_dist['D']['D'] =0.0;	map_dist['D']['E'] =87.0;	map_dist['D']['F'] =46.0;
	map_dist['E']['A'] =1.0;		map_dist['E']['B'] =3.0;		map_dist['E']['C'] =56.0;		map_dist['E']['D'] =87.0;	map_dist['E']['E'] =0.0;	map_dist['E']['F'] =55.0;
	map_dist['F']['A'] =5.0;		map_dist['F']['B'] =41.0;		map_dist['F']['C'] =21.0;		map_dist['F']['D'] =46.0;	map_dist['F']['E'] =55.0;	map_dist['F']['F'] =0.0;
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

char get_maximo(M_X &v_proba){
	int k=0;
	double max_v,max1;
	char c;
	for( const auto &ptr :v_proba){
	    for( const auto  &ptr2 :ptr.second){
	    	max_v =ptr2.second;
	    	if(k==0){
	    		max1 =max_v;
	    		c =ptr2.first;
	    		k++;
	    	}
	    	else{
	    		if(max_v > max1){
	    			max1 =max_v;
	    			c =ptr2.first;
	    		}
	    	}
	    }
	}
	return c;
}

void actulizar_arista_feromona(M_X &mx_fero, char c1, char c2){
	mx_fero[c1][c2] = fero_ini*phi + (1.0 - phi )*mx_fero[c1][c2];
	mx_fero[c2][c1] = fero_ini*phi + (1.0 - phi )*mx_fero[c2][c1];
}
void buscar_camino(M_X &map_dist, M_X &mx_fero, vector<char> &visitados){
	char visi_ini= '-';
	double t,n;
	double suma=0.0;
	M_X v_proba;//vector probabilidad
	double suma_cant;
	double n_random; 
	map<char, double> camino_ini;//camino inicial
	double q_rand;
	double max0,max1;
	char c_maximo;//arista maxima


	while(visitados.size() < map_dist.size()){
		q_rand =random_0_1();
		cout<<"\nValor de q: "<<q_rand<<endl;
		visi_ini =visitados[visitados.size()-1];
		cout<<"\nCIUDAD: "<<visi_ini<<endl;
		camino_ini= map_dist[visi_ini];
		
		if(q_rand > q0 )
		{
			cout<<"\nRecorrido por diversificacion\n";
			for (auto &ptr2: camino_ini){
				if(!find_ele(visitados, ptr2.first)){
				
					t =pow (mx_fero[visi_ini][ptr2.first],alfa);
					n =pow (1.0/map_dist[visi_ini][ptr2.first], beta);
					cout<<visi_ini<<"-"<<ptr2.first<<":\t t= "<<t<<"\tn = "<<n
						<<" \t t*n = "<<t*n<<endl;
					v_proba[visi_ini][ptr2.first] = t*n;
					suma +=t*n;
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
		}
		else{
			int k=0;
			cout<<"\nRecorrido por Intensificacion\n";
			for (auto &ptr2: camino_ini){
				if(!find_ele(visitados, ptr2.first)){
				
					t =mx_fero[visi_ini][ptr2.first];//sin alfa
					n =pow (1.0/map_dist[visi_ini][ptr2.first], beta);
					cout<<visi_ini<<"-"<<ptr2.first<<":\t t= "<<t<<"\tn = "<<n
						<<" \t t*n = "<<t*n<<endl;
					v_proba[visi_ini][ptr2.first] = t*n;

				}
			}
			visi_ini =get_maximo(v_proba);
		}



		visitados.push_back(visi_ini);
		cout<<"\tCiudad Siguiente: "<<visi_ini<<endl;
		char c1 =visitados[visitados.size()-2];
		char c2 =visitados[visitados.size()-1];
		cout<<"Actualizamos el arco: "<<c1<<"-"<<c2<<" : "<<fero_ini*phi + (1.0 - phi )*mx_fero[c1][c2]<<endl;
		actulizar_arista_feromona(mx_fero,c1,c2);
		cout<<"________________________________________\n";
		camino_ini.clear();
		v_proba.clear();
		suma=t=n=0.0;
	}
	/*
	cout<<"\nMostraremos la mx_fero\n";
	mostrar_map(mx_fero);*/
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
	double mejor, mejor_v;
	int posi;
	for (int i = 0; i < all_visitados.size(); ++i){
		mejor_v =costo_h(all_visitados[i],map_dist);
		if(i==0){
			mejor =costo_h(all_visitados[i],map_dist);
			posi =i;
		}
		else{
			if(mejor_v < mejor){
				mejor =mejor_v;
				posi =i;
			}

		}
	}
	//cout<<"\nMejor******: "<<posi<<endl;	
	double suma=0.0;
	for( const auto &ptr :mx_fero){
	    //suma=0.0;
	    for( const auto  &ptr2 :ptr.second){
	    	if(buscar_arista(all_visitados[posi],ptr.first , ptr2.first) && ptr.first != ptr2.first){
	    		cout << ptr.first << "-"<< ptr2.first << ": feromona = " << mx_fero[ptr.first][ptr2.first]*p<<" + "<<(1.0)/costo_h(all_visitados[posi], map_dist);
	    		mx_fero[ptr.first][ptr2.first] =mx_fero[ptr.first][ptr2.first]*p + (1.0)/costo_h(all_visitados[posi], map_dist);
	    		cout<<" = "<<mx_fero[ptr.first][ptr2.first]<<endl;
	    	}
	    	else{
	    		mx_fero[ptr.first][ptr2.first] =mx_fero[ptr.first][ptr2.first]*p ;
	    			
	    		cout << ptr.first << "-"<< ptr2.first << ": feromona = " << mx_fero[ptr.first][ptr2.first]*p<<endl;

	    	}
	    	//ptr2.second =ptr2.second*p + suma;
	    }
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
		//cout<<"\nENTOR: "<<global.size()<<endl;
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
		//cout<<posi_menor<<endl;
		for (int i = 0; i < all_visitados[0].size(); ++i){
			global[i] =all_visitados[posi_menor][i];
		}
	}
}
void get_local( vector<vector<char> > &all_visitados,M_X &map_dist){
	int posi_menor=0;
	double costo_menor =costo_h(all_visitados[0],map_dist);
	double costo_actual;
	//costo_menor =costo_h(global,map_dist);
	for (int i = 0; i < all_visitados.size(); ++i){
		costo_actual = costo_h(all_visitados[i],map_dist);
		if(costo_menor > costo_actual ){
			posi_menor =i;
			costo_menor =costo_actual; 
		}
	}
	cout<<"\nMejor Hormiga local: ";
	for (int i = 0; i < all_visitados[0].size(); ++i){
		cout<<all_visitados[posi_menor][i]<<"-";
	}
	cout<<"\tCosto: "<<costo_h(all_visitados[posi_menor],map_dist);
	cout<<endl;
}
void ACS(M_X &map_dist, M_X &mx_fero,char inicial){
	int i=0;
	vector<char> visitados;
	vector<vector<char> > all_visitados;
	std::vector<char> global;
	while(i <= 100 ){//iteraciones de las hormigas
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
		//return;
		mostrar_caminos(all_visitados, map_dist);
		actualizar_feromonas(mx_fero, all_visitados, map_dist);
		
		get_local(all_visitados,map_dist);
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
	ACS(map_dist,mx_fero,'E');

	




	//mostrar_map(mx_fero);
	cout<<"\nGOOD\n";
	return 0;
}
