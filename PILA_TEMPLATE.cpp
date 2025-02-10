#include <iostream>
using namespace std;

template<typename T>
struct nodo_pila {
	T dato;
	nodo_pila* siguiente;
};

template<typename T>
class Pila {
private:
	typedef nodo_pila<T> Nodo;
	Nodo* tope;
	
public:
	Pila() : tope(nullptr) {}
	
	void insertar_elemento(const T& dato) {
		Nodo* nuevo = new Nodo;
		nuevo->dato = dato;
		nuevo->siguiente = tope;
		tope = nuevo;
	}
	
	void quitar_elemento() {
		if (tope != nullptr) {
			Nodo* temp = tope;
			tope = tope -> siguiente;
			delete temp;
		}
	}
	
	
	T obtelemetno() const {
		if (tope != nullptr) {
			return tope->dato;
		}
		return T(); 
	}
	
	
	void mostrar_pila() const {
		Nodo* actual = tope;
		while (actual != nullptr) {
			cout << actual->dato << " ";
			actual = actual->siguiente;
		}
		cout << "NULL" << endl;
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

