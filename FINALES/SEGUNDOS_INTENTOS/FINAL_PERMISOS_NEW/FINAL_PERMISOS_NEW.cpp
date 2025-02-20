#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class Cargo{
private:
	int id;
	char nombre[200];
	vector<const char*> permisos_cargo;
public:
	Cargo(int i, const char* n){
		id = i;
		strcpy(nombre, n);
	}
	virtual ~Cargo(){}
	void pushpermiso(const char* p){
		permisos_cargo.push_back(p);
	}
	int getid(){
		return id;
	}
	const char* getnombre(){
		return nombre;
	}
	vector<const char*> getpermisos(){
		return permisos_cargo;
	}
};

class Extendido : public Cargo{
private:
	Cargo* base;
	vector<const char*> permisos_extendidos;
public:
	Extendido(int i_base, const char* n, Cargo* b) : Cargo (i_base, n){
		base = b;
		for (const auto& permiso : base->getpermisos()) {
			permisos_extendidos.push_back(permiso);
		}
	}
	void agregarpermisos(const char* permiso){
		permisos_extendidos.push_back(permiso);
	}
	vector<const char*> getpermisos(){
		return permisos_extendidos;
	}
};

class Usuario{
private:
	char nombre[200];
	int id;
	vector<Cargo*> cargos;
public:
	Usuario(const char* n, int i){
		strcpy(nombre, n);
		id = i;
	}
	void pushcargo(Cargo* c){
		cargos.push_back(c);
	}
	int getid(){
		return id;
	}
	const char* getnombre(){
		return nombre;
	}
	vector<Cargo*> getcargos(){
		return cargos;
	}
};

class Gestor{
private:
	vector<Usuario*> usuarios;
	vector<Cargo*> cargos;
public:
	void pushusuario(Usuario* u){
		usuarios.push_back(u);
	}
	void pushcargo(Cargo* c){
		cargos.push_back(c);
	}
	void guardararchivo(string direc1, string direc2, string direc3){
		ofstream archiusuarios(direc1, ios :: binary);
		ofstream archicargos(direc2, ios :: binary);
		ofstream archiextendidos(direc3, ios :: binary);
		if (archiusuarios.fail() || archicargos.fail() || archiextendidos.fail()){
			return;
		}
		for (const auto& u : usuarios){
			int id = u->getid();
			archiusuarios.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam = strlen(u->getnombre());
			archiusuarios.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archiusuarios.write(u->getnombre(), tam);
			int cant = u->getcargos().size();
			archiusuarios.write(reinterpret_cast<const char*>(&cant), sizeof(int));
			for (const auto& cargos : u->getcargos()){
				int idcargo = cargos->getid();
				archiusuarios.write(reinterpret_cast<const char*>(&idcargo), sizeof(int));
			}
		}
		archiusuarios.close();
		for (const auto& c : cargos){
			int id = c->getid();
			archicargos.write(reinterpret_cast<const char*>(&id), sizeof(int));
			
			int tam = strlen(c->getnombre());
			archicargos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archicargos.write(c->getnombre(), tam);
			
			int cant = c->getpermisos().size();
			archicargos.write(reinterpret_cast<const char*>(&cant), sizeof(int));
			
			for (const auto& permiso : c->getpermisos()) {
				int permiso_len = strlen(permiso);
				archicargos.write(reinterpret_cast<const char*>(&permiso_len), sizeof(int));
				archicargos.write(permiso, permiso_len);
			}
		}
		archicargos.close();
		for (const auto& c : cargos) {
			Extendido* ext = dynamic_cast<Extendido*>(c);
			if (ext){
				int id = ext->getid();
				archiextendidos.write(reinterpret_cast<const char*>(&id), sizeof(int));
				
				int tam = strlen(ext->getnombre());
				archiextendidos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
				archiextendidos.write(ext->getnombre(), tam);
				
				int cant = ext->getpermisos().size();
				archiextendidos.write(reinterpret_cast<const char*>(&cant), sizeof(int));
				for (const auto& permiso : ext->getpermisos()) {
					int permiso_len = strlen(permiso);
					archiextendidos.write(reinterpret_cast<const char*>(&permiso_len), sizeof(int));
					archiextendidos.write(permiso, permiso_len);
				}
			}
		}
	}
	bool saberpermiso(int id, const char* permiso){
		if (usuarios.empty() || cargos.empty()){
			return{};
		}
		for (const auto& u : usuarios){
			if (u->getid() == id){ /// USER ENCONTRADO
				cout << "ENCONTRADO" << endl;
				for (const auto& c : u->getcargos()){
					for (const auto& p : c->getpermisos()){
						if (strcmp(permiso, p) == 0){
							return true; /// PERMISO ENCONTRADO
						}
					}
				}
			}
		}
		return false; /// NO ENCONTRADO
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

