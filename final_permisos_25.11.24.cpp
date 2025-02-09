#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std;

class Permiso{
private:
	char permisos[200];
public:
	Permiso(const char* texto){
		strncpy(permisos, texto, sizeof(permisos));
		permisos[sizeof(permisos) - 1] = '\0';
	}
	const char* getPermiso(){
		return permisos;
	}
};

class Cargo{
private:
	char nombrecargo[200];
	vector<Permiso*> permisoscargo;
public:
	Cargo(const char* ncargo){
		strncpy(nombrecargo, ncargo, sizeof(nombrecargo));
		nombrecargo[sizeof(nombrecargo) - 1] = '\0';
	}
	const char* getCargo() const {
		return nombrecargo;
	}
	void agregarpermiso(Permiso* permiso){
		permisoscargo.push_back(permiso);
	}
	vector<Permiso*>& getPermisosTotales(){
		return permisoscargo;
	}
};

class CargoExtendido : public Cargo{
private:
	char nombreextendido[200];
public:
	CargoExtendido(const char* nombrebase, const char* nom) : Cargo(nombrebase){
		strncpy(nombreextendido, nom, sizeof(nombreextendido));
		nombreextendido[sizeof(nombreextendido) - 1] = '\0';
	}
	const char* getnombreextendido(){
		return nombreextendido;
	}
	void agregarpermisoextendido(Permiso* permiso){
		agregarpermiso(permiso);
	}
	vector<Permiso*>& getPermisosTotales(){
		return Cargo :: getPermisosTotales();
	}
};

class Usuario{
private:
	char nombre[200];
	int codigo;
	vector<Cargo*> cargosusuario;
public:
	Usuario(const char* nom, int cod){
		strncpy(nombre, nom, sizeof(nombre) + 1);
		nombre[sizeof(nombre) + 1] = '\0';
		codigo = cod;
	}
	void agregarcargo(Cargo* carg){
		cargosusuario.push_back(carg);
	}
	int getCodigo() const {
		return codigo;
	}
	const char* getNombre() const {
		return nombre;
	}
	vector<Cargo*>& getCargosTotales(){
		return cargosusuario;
	}
	
};

class Gestor{
private:
	vector <Usuario*> totalusuarios;
public:
	void agregarusuario(Usuario* user){
		totalusuarios.push_back(user);
	}
	/// PUNTO 1
	void guardararchivo(const string& direcarchivo){
		if (totalusuarios.empty()){
			return;
		}
		ofstream archivo (direcarchivo, ios :: binary);
		if (archivo.fail()){
			return;
		}
		for (const auto& bucle : totalusuarios){
			
			int code = bucle->getCodigo();
			archivo.write(reinterpret_cast<const char*> (&code), sizeof(code));
			
			int larg = strlen(bucle->getNombre());
			archivo.write(reinterpret_cast<const char*> (&larg), sizeof(larg));
			archivo.write(bucle->getNombre(), larg);
			
			int numcargos = bucle->getCargosTotales().size();
			archivo.write(reinterpret_cast<const char*>(&numcargos), sizeof(numcargos));
			for (auto const& cargos : bucle->getCargosTotales()){
				int largcargo = strlen(cargos->getCargo());
				archivo.write(reinterpret_cast<const char*>(&largcargo), sizeof(largcargo));
				archivo.write(cargos->getCargo(), largcargo);
				
				int cantpermisos = cargos->getPermisosTotales().size();
				archivo.write(reinterpret_cast<const char*>(&cantpermisos), sizeof(cantpermisos));
				for(const auto& permisos : cargos->getPermisosTotales()){
					int longipermiso = strlen(permisos->getPermiso());
					archivo.write(reinterpret_cast<const char*>(&longipermiso), sizeof(longipermiso));
					archivo.write(permisos->getPermiso(), longipermiso);
				}
			}
		}
		archivo.close();
	}
	/// PUNTO 2
	void saberpermiso(int cod, const char* permiso){
		if (totalusuarios.empty()){
			return;
		}
		
		for (const auto& bucle : totalusuarios){
			if (cod == bucle->getCodigo()){
				for (const auto& cargos : bucle->getCargosTotales()){
					for (const auto& permisos : cargos->getPermisosTotales()){
						if (strcmp(permiso, permisos->getPermiso()) == 0){
							cout << "EL USUARIO CUENTA CON EL PERMISO" << endl;
							return;
						}
					}
				}
			}
		}
		cout << "EL USER NO TIENE ESE PERMISO"<< endl;
		return;
	}
	///PUNTO 3 A
	void cargomaspermisos(){
		if (totalusuarios.empty()){
			return;
		}
		map<string, int> cargosypermisos;
		for (const auto& user : totalusuarios){
			for (const auto& cargo : user->getCargosTotales()){
				int totalpermisos = cargo->getPermisosTotales().size();
				//LO ALMACENO
				cargosypermisos[cargo->getCargo()] = totalpermisos;
			}
		}
		int maxpermisos = 0;
		for (const auto& bucle : cargosypermisos){
			if (bucle.second > maxpermisos){
				maxpermisos = bucle.second;
			}
		}
		cout << "CARGO CON MAS PERMISOS:" << endl;
		for (const auto& bucle : cargosypermisos){
			if (maxpermisos == bucle.second){
				cout << bucle.first << endl;
			}
		}
	}
	/// PUNTO 3 B
	void permisorepetido(){
		if (totalusuarios.empty()){
			return;
		}
		map<string, int> permisoscontados;
		for (const auto& bucle : totalusuarios){
			for(const auto& cargos : bucle->getCargosTotales()){
				for (const auto& permisos : cargos->getPermisosTotales()){
					permisoscontados[permisos->getPermiso()]++;
				}
			}
		}
		cout << "PERMISOS QUE SE REPITEN:" << endl;
		for (const auto& repetidos : permisoscontados){
			if (repetidos.second > 1){
				cout << repetidos.first;
			}
		}
	}
	/// PUNTO 3 C
	void maspermisos(){
		if (totalusuarios.empty()){
			return;
		}
		int max = 0;
		int totalpermisos = 0;
		map<string, int> mascargos;
		for (const auto& bucle : totalusuarios){
			for (const auto& cargos : bucle->getCargosTotales()){
				totalpermisos = cargos->getPermisosTotales().size();
			}
			if (totalpermisos > max){
				max = totalpermisos;
				mascargos.clear();
				mascargos[bucle->getNombre()] = totalpermisos;
			} else if (totalpermisos == max){
				mascargos[bucle->getNombre()] = totalpermisos;
			}
		}
		cout << "USUARIO/S CON MAS PERMISOS:" << endl;
		for (const auto& bucle : mascargos){
			cout << bucle.first << "  " << bucle.second;
		}
	}
};

int main(int argc, char *argv[]) {
	Gestor g;
	// Crear permisos
	Permiso* permiso1 = new Permiso("ver stock");
	Permiso* permiso2 = new Permiso("editar stock");
	Permiso* permiso3 = new Permiso("auditar venta");  // Permiso adicional para cargo extendido
	
	// Crear cargos
	Cargo* cargo1 = new Cargo("Vendedor");
	cargo1->agregarpermiso(permiso1);
	cargo1->agregarpermiso(permiso2);
	
	// Crear un cargo extendido (Jefe de ventas) que extiende de "Vendedor"
	CargoExtendido* cargoExtendido = new CargoExtendido("Vendedor", "Jefe de ventas");
	cargoExtendido->agregarpermisoextendido(permiso3);  // Agregar permiso adicional
	
	// Crear usuario y asignar cargos
	Usuario* user1 = new Usuario("Pipina", 12);
	user1->agregarcargo(cargo1);  // Usuario tiene el cargo base
	user1->agregarcargo(cargoExtendido);  // Usuario tiene el cargo extendido
	g.agregarusuario(user1);
	
	
	// Verificar permisos
	cout << "Prueba 1: Usuario con permiso" << endl;
	g.saberpermiso(12, "ver stock");  // El usuario tiene este permiso
	
	cout << "Prueba 2: Usuario con permiso adicional" << endl;
	g.saberpermiso(12, "auditar venta");  // El usuario tiene este permiso

	return 0;
}

/// TEORIA
/// 4A)¿Por qué son necesarias las clases template en los lenguajes de tipado estático?
/// Las clases template son fundamentales en lenguajes de tipado estatito porque 
/// ofrecen grandes ventajas en la flexibilidad y reutilizacion de codigo, tambien
/// no pierden la ventaja de uso de recursos

/// 4B) ¿Por qué C++ utiliza punteros para implementar el polimorfismo?
/// El polimorfismo se puede lograr usando punteros y referencias combinando tambien la herencia y sus funciones virtuales.
/// De las principales razones es la de permitir manejar objetos de clases derivadas desde una clase base, lo que hace posible
/// las funciones virtuales. Tambien evitan la duplicacion de objetos grandes, lo que mejora la eficiencia del codigo
