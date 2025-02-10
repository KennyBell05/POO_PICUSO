#include <iostream>
using namespace std;

template <class T>
class vector_ordenado{
private:
	T* elementos;
	int capacidad_max;
	int total_elementos;
	
	void expandir_capacidad(){
		capacidad_max *= 2;
		T* nuevo_arreglo = new T[capacidad_max];
		
		for(int i = 0; i < total_elementos; i++){
			nuevo_arreglo[i] = elementos[i];
		}
		
		delete [] elementos;
		elementos = nuevo_arreglo;
	}
	void ordenar_vector(){
		for (int i = 0; i < total_elementos; i++){
			for(int j = 1; j < total_elementos; j++){
				if(elementos[i] < elementos[j]){
					T aux = elementos[i];
					elementos[i] = elementos[j];
					elementos[j] = aux;
				}
			}
		}
	}
	bool elemento_repetido(T dato){
		for(int i = 0; i < total_elementos; i++){
			if (dato == this->elementos[i]){
				return true;
			}
		}
		return false;
	}
public:
	vector_ordenado(): capacidad_max(2){
	total_elementos = 0;
	elementos = new T[capacidad_max];
	}
	void insertar_elemento(const T& elemento){
		if(elemento_repetido(elemento)){
			return;
		}
		if(total_elementos == capacidad_max){
			expandir_capacidad();
		}
		elementos[total_elementos] = elemento;
		total_elementos ++;
		ordenar_vector();
	}
	T quitar_elemento(int indice){
		if(indice < 0 || indice >= total_elementos){
			return {};
		}
		T aux = elementos[indice];
		for(; indice < total_elementos - 1; indice++){
			elementos[indice] = elementos[indice+1];
		}
		total_elementos--;
		return aux;
	}
	void mostrar_elementos(){
		for(int i = 0; i < total_elementos; i++){
			cout << elementos[i] << endl;
		}
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

