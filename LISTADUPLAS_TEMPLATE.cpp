#include <iostream>
using namespace std;

template<typename K, typename V>
struct nodo_listase {
	K llave;
	V dato;
	nodo_listase* siguiente;
};


template<typename K, typename V>
class Dupla {
private:
	typedef nodo_listase<K, V> Nodo;
	Nodo* inicio;
	
	Nodo* buscar_nodo(const K& llave) const {
		Nodo* actual = inicio;
		while (actual != nullptr) {
			if (actual->llave == llave) {
				return actual;
			}
			actual = actual->siguiente;
		}
		return nullptr;
	}
	
public:
	Dupla() : inicio(nullptr){}
	
	
	void insertar(const K& llave, const V& dato) {
		if (buscar_nodo(llave) == nullptr) { 
			Nodo* nuevo = new Nodo;
			nuevo->llave = llave;
			nuevo->dato = dato;
			nuevo->siguiente  = inicio;
			inicio = nuevo;
		}
	}
	
	V obtelemetno(const K& llave) {
		Nodo* encontrado = buscar_nodo(llave);
		
		if(encontrado != nullptr){
			V dato = encontrado->dato;
			return dato;
		}
		
		return nullptr;
	}
	
	void suprimir(const K& llave) {
		Nodo* actual = inicio;
		Nodo* anterior = nullptr;
		
		while (actual != nullptr) {
			if (actual->llave == llave) {
				if (anterior == nullptr) {
					inicio = actual->siguiente; 
				} else {
					anterior->siguiente = actual->siguiente;
				}
				delete actual;
				return;
			}
			anterior = actual;
			actual = actual->siguiente;
		}
	}
};


int main(int argc, char *argv[]) {
	
	return 0;
}

