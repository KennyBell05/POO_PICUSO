#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

// Estructura template para almacenar el par clave-valor
template <typename K, typename V>
class ConjuntoClaveValor{
private:
	struct Nodo{
		K clave;
		V valor;
	};
	Nodo* elementos;
	int capacidad;
	int cantidad;
public:
	ConjuntoClaveValor(int cap = 10){
		capacidad = cap;
		cantidad = 0;
		elementos = new Nodo[capacidad];
	}
	void agregar(K clave, V valor){
		for (int i = 0; i < cantidad; i++){
			if (elementos[i].clave == clave){
				cout << "LA CLAVE YA EXISTE" << endl;
				return;
			}
		}
		if (cantidad == capacidad){
			cout << "NO HAY MAS LUGAR" << endl;
			return;
		}
		elementos[cantidad].clave = clave;
		elementos[cantidad].valor = valor;
		cantidad++;
	}
	void mostrar(){
		cout << "ELEMENTOS: " << endl;
		for (int i = 0; i < cantidad; i++){
			cout << elementos[i].clave << " -> " << elementos[i].valor << endl;
		}
	}
	V buscar(K clave){
		for (int i = 0; i < cantidad; i++){
			if (elementos[i].clave == clave){
				return elementos[i].valor;
			}
		}
		cout << "CLAVE NO ENCONTRADA" << endl; 
		return;
	}
	void eliminar(K clave){
		int pos = -1;
		for (int i = 0; i < cantidad; i++){
			if (elementos[i].clave == clave){
				pos = i;
				break;
			}
		}
		if (pos != -1){
			Nodo* aux = new Nodo[capacidad];;
			for (int i = 0; i < pos; i++){
				aux[i] = elementos[i];
			}
			for (int i = pos; i < cantidad - 1; i++){
				aux[i] = elementos[i + 1];
			}
			delete[] elementos;
			elementos = aux;
			cantidad--;
		} else {
			cout << "CLAVE NO ENCONTRADA" << endl;
			return;
		}
	}
};


class Viajes{
protected:
	char nombre[200];
	int tiempo;
public:
	Viajes(const char* nom, int t){
		strncpy(nombre, nom, sizeof(nombre));
		nombre[sizeof(nombre) - 1] = '\0';
		tiempo = t;
	}
	virtual float calculartotal() = 0;
	virtual int calculartiempo() = 0;
	virtual const char* getTipo() = 0;
	virtual const char* getNombre() = 0;
	virtual float getCosto() = 0;
	virtual ~Viajes(){};
};

class Avion : public Viajes{
private:
	float precio;
public:
	Avion(const char* nombre, int tiempo, float pre) : Viajes(nombre, tiempo){
		precio = pre;
	}
	const char* getNombre(){
		return nombre;
	}
	float getCosto(){
		return precio;
	}
	float calculartotal(){
		float total = precio * 1.20;
		return total;
	};
	int calculartiempo(){
		int tiempototal = tiempo + 2;
		return tiempototal;
	}
	const char* getTipo(){
		return "AVION";
	}
};

class Colectivo : public Viajes{
private:
	int kilometros;
public:
	Colectivo(const char* nombre, int kilo, int tiempo) : Viajes(nombre, tiempo){
		kilometros = kilo;
	}
	float getCosto(){
		return 8;
	}
	const char* getNombre(){
		return nombre;
	}
	float calculartotal(){
		int total = kilometros * 8;
		return total;
	}
	int calculartiempo(){
		return tiempo;
	}
	const char* getTipo(){
		return "COLECTIVO";
	}
};

class Combinado : public Viajes{
private:
	Avion* avion;
	Colectivo* colectivo;
public:
	Combinado(const char* nombre, Avion* a, Colectivo* c) : Viajes(nombre, 0){
		avion = a;
		colectivo = c;
	}
	float getCosto(){
		float sumacosto = avion->getCosto() + colectivo->getCosto();
		return sumacosto;
	}
	const char* getNombre(){
		return nombre;
	}
	float calculartotal(){
		int total = avion->calculartotal() + colectivo->calculartotal();
		return total;
	}
	int calculartiempo(){
		int total = avion->calculartiempo() + colectivo->calculartiempo();
		return total;
	}
	const char* getTipo(){
		return "COMBINADO";
	}
};

class Paquete : public Viajes{
private:
	vector<Viajes*> viajes;
public:
	Paquete(const char* nombre) : Viajes(nombre, 0){}
	void agregarviaje(Viajes* v){
		viajes.push_back(v);
	}
	float calculartotal(){
		float total = 0;
		for(const auto& v : viajes){
			total += v->calculartotal();
		}
		return total;
	}
	int calculartiempo(){
		int tiempo = 0;
		for (const auto& t : viajes){
			tiempo += t->calculartiempo();
		}
		return tiempo;
	}
	const char* getTipo(){
		return "Paquete";
	}
	const char* getNombre(){
		return nombre;
	}
};

class Gestor{
private:
	vector<Viajes*> todoviajes;
public:
	void agregarviaje(Viajes* viaje){
		todoviajes.push_back(viaje);
	}
	void calcularviajes(){
		if (todoviajes.empty()){
			return;
		}
		for(const auto& bucle : todoviajes){
			cout << "VIAJE: " << bucle->getTipo() << endl;
			cout << "TIEMPO: " << bucle->calculartiempo() << endl;
			cout << "TOTAL: " << bucle->calculartotal();
			cout << endl;
		}
	}
	void guardararchivo(string direcarchivo){
		if (todoviajes.empty()){
			return;
		}
		ofstream archivo(direcarchivo, ios :: binary);
		if (archivo.fail()){
			return;
		}
		for (const auto& bucle: todoviajes){
			int tamnombre = strlen(bucle->getNombre());
			archivo.write(reinterpret_cast<char const*>(&tamnombre), sizeof(tamnombre));
			archivo.write(bucle->getNombre(), tamnombre);
			
			int tamtipo = strlen(bucle->getTipo());
			archivo.write(reinterpret_cast<char const*>(&tamtipo), sizeof(tamtipo));
			archivo.write(bucle->getTipo(), tamtipo);
			
			float totalcosto = bucle->getCosto();
			archivo.write(reinterpret_cast<const char*>(&totalcosto), sizeof(totalcosto));
			
			int horastotales = bucle->calculartiempo();
			archivo.write(reinterpret_cast<const char*>(&horastotales), sizeof(horastotales));
			
			float totalprecio = bucle->calculartotal();
			archivo.write(reinterpret_cast<const char*>(&totalprecio), sizeof(totalprecio));
			
			
		}
	}
	void masbarato(){
		sort (todoviajes.begin(), todoviajes.end(), [](Viajes* a, Viajes* b){
			return a->calculartotal() < b->calculartotal();
		});
		
		cout << "LOS 5 VIAJES MAS BARATOS SON: " << endl;
		for (int i = 0; i < 5 && todoviajes.size(); i++){
			cout << "VIAJE " << todoviajes[i]->getNombre() << ", Tipo: " << todoviajes[i]->getTipo();
			cout << endl;
		}
	}
	void mastiempo(){
		auto max = max_element(todoviajes.begin(), todoviajes.end(), [](Viajes* a, Viajes* b){
			return a->calculartiempo() < b->calculartiempo();
		});
		if (max != todoviajes.end()){
			cout << "EL VIAJE CON MAS TIEMPO ES: " << (*max)->getNombre() << " con la cantidad de horas de " << (*max)->calculartiempo();
		}
	}
	void viajeespecial(int max){
		if (todoviajes.empty()){
			return;
		}
		vector<Viajes*> especiales;
		copy_if(todoviajes.begin(), todoviajes.end(), back_inserter(especiales), [max](Viajes* viaje){
			return viaje->calculartiempo() <= max;
		});
		if (especiales.empty()){
			return;
		} else{
			for (const auto& viajes : especiales){
				cout << "VIAJE " << viajes->getNombre() << " TIEMPO DE: " << viajes->calculartiempo();
			}
		}
	}
	
};

int main(int argc, char *argv[]) {
	// Crear un viaje en avión
	Avion* a = new Avion("Viaje en Avion", 10, 1000); // 10 horas de vuelo, 1000 USD ida y vuelta
	
	// Crear un viaje en colectivo
	Colectivo* c = new Colectivo("Viaje en Colectivo", 200, 5); // 200 km, 5 horas
	
	// Crear un viaje combinado
	Combinado* combinado = new Combinado("Viaje Combinado", a, c);
	
	// Crear gestor y agregar los viajes
	Gestor gestor;
	gestor.agregarviaje(a); // Agrega el viaje en avión
	gestor.agregarviaje(c); // Agrega el viaje en colectivo
	gestor.agregarviaje(combinado); // Agrega el viaje combinado
	
	// Calcular y mostrar los resultados
	gestor.calcularviajes();
	
	// Guardar los viajes en un archivo binario
	gestor.guardararchivo("viajes.dat");
	
	// Liberar memoria
	delete a;
	delete c;
	delete combinado;
	
	return 0;
}

/// Los templates permiten escribir código genérico, lo que significa que podemos 
/// definir una función o una clase sin especificar el tipo de dato exacto. Luego, el 
/// compilador generará versiones específicas del código cuando se utilicen con tipos concretos.
/// Lo que lo hace flexible y reutilizable.

/// Las tecninas de reutilizacion son: Templates, Herencia, Polimorfismo y Composicion.
/// Templates: Permite el codigo generico, lo que por ejemplo si tenemos que hacer una funcion que
/// calcule el mayor de dos numeros, pero no queremos hacer una funcion por tipo de dato, usamos 
/// templates que nos ahorra el trabajo de definir todas las funciones, y tambien lo hace reutilizable.
/// Herencia: Sirve para cuando tenemos una clase padre, por ejemplo Curso, y sabemos que comparten atributos y/o metodos
/// podemos ahorrrarnos codigo y usar esos atributos en la clase hija, o metodos que nos sirvan en ambas. Por eso el nombre herencia, hereda a la otra
/// clase.
/// Polimorfismo: Se aplica cuando se usa herencia, y es un concepto el cual permite que un metodo tenga diferentes comportamientos segun el objeto que lo invoque
/// Si tenemos el metodo comer, y las humano y perro, sabemos que ambos comen pero de distinta manera, el humano con cubiertos sentado en una mesa y el perro de un
/// plato en el piso
/// Composicion: Se utiliza cuando en vez de heredar, podemos reutilizar codigo y crear objetos dentro de otro. Por ejemplo si tenemos la clase auto,
/// y luego la clse motor, podemos crear un atributo dentro de coche que sea motor, ya que forma parte.

