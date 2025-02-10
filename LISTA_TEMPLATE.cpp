#include <iostream>
using namespace std;

template<typename T>
struct nodo_listase {
	T dato;
	nodo_listase* siguiente;
};


template<typename T>
class Lista {
private:
	typedef nodo_listase<T> Nodo;
	Nodo* inicio;
	
public:
	Lista() : inicio(nullptr) {}
	
	
	void insertar(const T& dato) {
		Nodo* nuevo = new Nodo;
		nuevo->dato = dato;
		nuevo->siguiente = inicio;
		inicio = nuevo;
	}
	
	
	T obtelemetno() const {
		if (inicio != nullptr) {
			return inicio->dato;
		}
		return T();
	}
	
	
	void suprimir() {
		if (inicio != nullptr) {
			Nodo* temp = inicio;
			inicio = inicio->siguiente;
			delete temp;
		}
	}
	
	
	void mostrar_lista() const {
		Nodo* actual = inicio;
		while (actual != nullptr) {
			cout << actual->dato << " -> ";
			actual = actual->siguiente;
		}
		cout << "NULL" << endl;
	}
	
	~Lista() {
		Nodo* actual = inicio;
		while (actual != nullptr) {
			Nodo* temp = actual;
			actual = actual->siguiente;
			delete temp;
		}
	}
};

int main() {
	Lista<int> lista;
	lista.insertar(10);
	lista.insertar(20);
	lista.insertar(30);
	lista.mostrar_lista();
	
	cout << "elemento " << lista.obtelemetno() << endl;
	lista.suprimir();
	lista.mostrar_lista();
	
	return 0;
}
