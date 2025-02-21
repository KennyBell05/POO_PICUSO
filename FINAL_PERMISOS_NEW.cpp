#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;

class BaseCargo {
protected:
	int id;
	char nombre[200];
public:
	BaseCargo(int i, const char* n) {
		id = i;
		strcpy(nombre, n);
	}
	virtual vector<const char*> getpermisos() = 0;
	int getid(){
		return id; 
	}
	const char* getnombre() const {
		return nombre; 
	}
	virtual ~BaseCargo() {}
};

class Cargo : public BaseCargo {
protected:
	vector<const char*> permisos;
public:
	Cargo(int i, const char* n) : BaseCargo(i, n) {}
	
	void agregarpermisos(const char* permiso) {
		permisos.push_back(permiso);
	}
	
	vector<const char*> getpermisos() override {
		return permisos;
	}
};

class Extendido : public BaseCargo {
private:
	BaseCargo* base;
	vector<const char*> permisosadicionales;
public:
	Extendido(int i, const char* n, BaseCargo* basecargo) : BaseCargo(i, n){
		base = basecargo;
	}
	
	void agregarpermisoadicional(const char* permiso) {
		permisosadicionales.push_back(permiso);
	}
	
	int getidbase(){
		return base->getid();
	}
	
	vector<const char*> getpermisos() override {
		vector<const char*> permisoscompletos = base->getpermisos();
		permisoscompletos.insert(permisoscompletos.end(), permisosadicionales.begin(), permisosadicionales.end());
		return permisoscompletos;
	}
};

class Usuario {
private:
	int id;
	char nombre[200];
	vector<BaseCargo*> cargos;
public:
	Usuario(int i, const char* n) {
		id = i;
		strcpy(nombre, n);
	}
	
	void agregarcargo(BaseCargo* c) {
		cargos.push_back(c);
	}
	
	int getid() { return id; }
	const char* getnombre() { return nombre; }
	vector<BaseCargo*> getcargos() { return cargos; }
};

class Gestor {
private:
	vector<Usuario*> usuarios;
	vector<BaseCargo*> cargos;
public:
	void agregarusuario(Usuario* u) {
		usuarios.push_back(u);
	}
	
	void agregarcargo(BaseCargo* c) {
		cargos.push_back(c);
	}
	
	void creararchivos(string direc1, string direc2, string direc3){
		ofstream archiusuarios(direc1, ios :: binary);
		ofstream archicargos(direc2, ios :: binary);
		ofstream archiextendidos(direc3, ios :: binary);
		if (archiusuarios.fail() || archicargos.fail() || archiextendidos.fail()){
			return;
		}
		if (usuarios.empty()){
			return;
		}
		for (const auto& u : usuarios){
			int id = u->getid();
			archiusuarios.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam = strlen(u->getnombre());
			archiusuarios.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archiusuarios.write(u->getnombre(), tam);
			int total = u->getcargos().size();
			archiusuarios.write(reinterpret_cast<const char*>(&total), sizeof(int));
			for (const auto& c : u->getcargos()){
				int idcargo = c->getid();
				archiusuarios.write(reinterpret_cast<const char*>(&idcargo), sizeof(int));
			}
		}
		archiusuarios.close();
		if (cargos.empty()){
			return;
		}
		for (const auto& c : cargos){ /// GUARDO POR SEPARADO CARGOS Y EXTENDIDOS CON DYNAMIC DIFERENCIO
			Extendido* extendido = dynamic_cast<Extendido*>(c);
			if (!extendido){
				int id = c->getid();
				archicargos.write(reinterpret_cast<const char*>(&id), sizeof(int));
				
				int tam = strlen(c->getnombre());
				archicargos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
				archicargos.write(c->getnombre(), tam);
				
				int totalpermisos = c->getpermisos().size();
				archicargos.write(reinterpret_cast<const char*>(&totalpermisos), sizeof(int));
				for (const auto& p : c->getpermisos()) {
					int tampermiso = strlen(p);
					archicargos.write(reinterpret_cast<const char*>(&tampermiso), sizeof(int));
					archicargos.write(p, tampermiso);
				}
			} else {
				int id = extendido->getid();
				int idbase = extendido->getidbase();
				archiextendidos.write(reinterpret_cast<const char*>(&id), sizeof(int));
				archiextendidos.write(reinterpret_cast<const char*>(&idbase), sizeof(int));
				
				int tam = strlen(extendido->getnombre());
				archiextendidos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
				archiextendidos.write(extendido->getnombre(), tam);
				
				int totalpermisos = extendido->getpermisos().size();
				archiextendidos.write(reinterpret_cast<const char*>(&totalpermisos), sizeof(int));
				for (const auto& p : extendido->getpermisos()) {
					int tampermiso = strlen(p);
					archiextendidos.write(reinterpret_cast<const char*>(&tampermiso), sizeof(int));
					archiextendidos.write(p, tampermiso);
				}
			}
		}
	}
	
	bool tienepermiso(int id, const char* permiso) {
		for (const auto& u : usuarios) {
			if (u->getid() == id) { // ENCONTRADO
				for (const auto& c : u->getcargos()) {
					for (const auto& p : c->getpermisos()) {
						if (strcmp(p, permiso) == 0) {
							return true; // LO TIENE
						}
					}
				}
				return false; // NO LO TIENE
			}
		}
		return false; // USER NO EXISTE
	}
	
	vector<BaseCargo*> maspermisos(){
		if (cargos.empty()){
			return{};
		}
		vector<BaseCargo*> maspermisivos;
		auto max = max_element(cargos.begin(), cargos.end(), [](BaseCargo* a, BaseCargo* b){
			return a->getpermisos().size() < b->getpermisos().size();
		});
		int maximo = (*max)->getpermisos().size();
		for (const auto& c : cargos){
			int tamanio = c->getpermisos().size();
			if (tamanio == maximo){
				maspermisivos.push_back(c);
			}
		}
		return maspermisivos;
	}
	vector<const char*> permisoscompartidos(){
		if (cargos.empty()){
			return{};
		}
		map<const char*, int> contadorpermisos;
		vector<const char*> permisosrepetidos;
		for (const auto& c : cargos) {
			for (const auto& p : c->getpermisos()) {
				contadorpermisos[p]++;
			}
		}
		for (const auto& par : contadorpermisos){
			if (par.second > 1){
				permisosrepetidos.push_back(par.first);
			}
		}
		return permisosrepetidos;
	}
	void usuariosmaspermisos(){
		if (usuarios.empty()){
			return;
		}
		masp<Usuario*, int> permisosdeusuario;
		for (const auto& u : usuarios){ /// GUARDAMOS TODOS LOS PERMISOS DE TODOS LOS USERS
			int totalpermisos = accumulate(u->getcargos().begin(), u->getcargos().end(), 0, [](int sum, BaseCargo* c){
				return sum += c->getpermisos().size();
			});
			permisosdeusuario[u] = totalpermisos;
		}
		auto max = max_element( )
	}
};

int main() {
	Gestor gestor;
	
	// Crear cargos con permisos
	Cargo* cargo1 = new Cargo(1, "Administrador");
	cargo1->agregarpermisos("Leer");
	cargo1->agregarpermisos("Escribir");
	cargo1->agregarpermisos("Eliminar");
	
	Cargo* cargo2 = new Cargo(2, "Editor");
	cargo2->agregarpermisos("Leer");
	cargo2->agregarpermisos("Escribir");
	
	Cargo* cargo3 = new Cargo(3, "Invitado");
	cargo3->agregarpermisos("Leer");
	
	// Agregar los cargos al gestor
	gestor.agregarcargo(cargo1);
	gestor.agregarcargo(cargo2);
	gestor.agregarcargo(cargo3);
	
	// Obtener permisos compartidos
	vector<const char*> comunes = gestor.permisoscompartidos();
	
	// Mostrar los permisos en más de un cargo
	cout << "Permisos en varios cargos:" << endl;
	for (const auto& p : comunes) {
		cout << p << endl;
	}
	
	// Liberar memoria
	delete cargo1;
	delete cargo2;
	delete cargo3;
	
	return 0;
}
