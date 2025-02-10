#include <iostream>
using namespace std;

template<typename T>
struct nodo_cola {
	T valor;
	nodo_cola<T>* siguiente;
};

template<typename T>
class Cola {
private:
	nodo_cola<T>* inicio;
	nodo_cola<T>* fin;
	
	bool cola_vacia() {
		return (inicio == nullptr);
	}
	
public:
	Cola() {
		inicio = nullptr;
		fin = nullptr;
	}
	
	void insertar_elemento(T valor) {
		nodo_cola<T>* nuevo_nodo = new nodo_cola<T>;
		nuevo_nodo->valor = valor;
		nuevo_nodo->siguiente = nullptr;
		
		if (cola_vacia()) {
			inicio = nuevo_nodo;
		} else {
			fin->siguiente = nuevo_nodo;
		}
		fin = nuevo_nodo;
	}
	
	T obtelemetno() {
		if (cola_vacia()) {
			return;
		}
		
		nodo_cola<T>* aux = inicio;
		T valor = aux->valor;
		inicio = inicio->siguiente;
		
		if (inicio == nullptr) {
			fin = nullptr;
		}
		
		delete aux;
		return valor;
	}
};



int main(int argc, char *argv[]) {
	
	return 0;
}

