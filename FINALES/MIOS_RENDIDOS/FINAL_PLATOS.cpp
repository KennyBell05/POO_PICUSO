#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

class Extra{
public:
	virtual int getextra() = 0;
};

class Impuesto : public Extra{
private:
	int valor;
public:
	Impuesto(int v){
		valor = v;
	}
	int getextra(){
		return valor;
	}
};

class Manodeobra : public Extra{
private:
	int horas;
	int valorporhora;
public:
	Manodeobra(int h, int v){
		horas = h;
		valorporhora = v;
	}
	int getextra(){
		int total = horas * valorporhora;
		return total;
	}
};

class Comida{
private:
	int id;
	char nombre[100];
public:
	Comida(int i, const char* n){
		id = i;
		strcpy(nombre, n);
	}
	virtual int getcosto() = 0;
	int getid(){
		return id;
	}
	const char* getnombre(){
		return nombre;
	}
};

class Ingrediente : public Comida{
private:
	int costo;
public:
	Ingrediente(int id, const char* nombre, int c) : Comida(id, nombre){
		costo = c;
	}
	int getcosto(){
		return costo;
	}
};

class Plato : public Comida{
private:
	vector<Comida*> ingredientes;
	vector<Extra*> extras;
public:
	Plato(int id, const char* nombre) : Comida(id, nombre){}
	void pushingrendiente(Ingrediente* ingre){
		ingredientes.push_back(ingre);
	}
	void pushextra(Extra* e){
		extras.push_back(e);
	}
	int getcosto(){
		int total = 0;
		for (const auto& ing : ingredientes){
			total += ing->getcosto();
		}
		for (const auto& ext : extras){
			total += ext->getextra();
		}
		return total;
	}
	vector<Comida*> getingredientes(){
		return ingredientes;
	}
	
};

class Gestor{
private:
	vector<Plato*> platos;
	vector<Ingrediente*> ingredientes;
public:
	void pushplato(Plato* p){
		platos.push_back(p);
	}
	void pushingrediente(Ingrediente* ing){
		ingredientes.push_back(ing);
	}
	void crear_archivos(string direc1, string direc2, string direc3){
		ofstream archivoingredientes(direc2, ios :: binary);
		ofstream archivoplatos(direc1, ios :: binary);
		ofstream archivorelacion(direc3, ios :: binary);
		if (archivoplatos.fail() || archivoingredientes.fail() || archivorelacion.fail()){
			return;
		}
		for (const auto& ing : ingredientes){
			int id = ing->getid();
			archivoingredientes.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam = strlen(ing->getnombre());
			archivoingredientes.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archivoingredientes.write(ing->getnombre(), tam);
			int precio = ing->getcosto();
			archivoingredientes.write(reinterpret_cast<const char*>(&precio), sizeof(int));
		}
		for (const auto& p : platos){
			int id = p->getid();
			archivoplatos.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam = strlen(p->getnombre());
			archivoplatos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archivoplatos.write(p->getnombre(), tam);
			int valor = p->getcosto();
			archivoplatos.write(reinterpret_cast<const char*>(&valor), sizeof(int));
		}
		for (const auto& p : platos){
			int id = p->getid();
			archivorelacion.write(reinterpret_cast<const char*>(&id), sizeof(int));
			
			int cant_ingredientes = p->getingredientes().size();
			archivorelacion.write(reinterpret_cast<const char*>(&cant_ingredientes), sizeof(int));
			
			for (const auto& i : p->getingredientes()){
				Ingrediente* ingrediente = dynamic_cast<Ingrediente*>(i);
				if (ingrediente){
					int id = ingrediente->getid();
					archivorelacion.write(reinterpret_cast<const char*>(&id), sizeof(int));
				} else {
					Plato* plato = dynamic_cast<Plato*>(i);
					if (plato){
						int id = plato->getid();
						archivorelacion.write(reinterpret_cast<const char*>(&id), sizeof(int));
					}
				}
			}
		}
		archivoingredientes.close();
		archivoplatos.close();
		archivorelacion.close();
	}
	vector<Plato*> ordenarplatos(vector<Plato*> p){
		if (p.empty()){
			return{};
		}
		sort(p.begin(), p.end(),[](const auto& a, const auto& b){return a->getcosto() < b->getcosto();});
		return p;
	}
	void topbaratos(vector<Plato*> p, string direcarchi){
		if (p.empty()){
			return;
		}
		ofstream archivo(direcarchi);
		if (archivo.fail()){
			return;
		}
		vector<Plato*> baratos = ordenarplatos(p);
		baratos.resize(5);
		for (const auto& p : baratos){
			archivo << p->getnombre()  << ":" << p->getcosto() << endl;
		}
		archivo.close();
	}
	void sinharina(vector<Plato*> platosaux, string direcarchi){
		if (platosaux.empty()) {
			return;
		}
		vector<Plato*> platosSH;
		
		for (const auto& p : platosaux) {
			bool harina = false;
			for (const auto& c : p->getingredientes()) {
				Ingrediente* ingre = dynamic_cast<Ingrediente*>(c);
				if (ingre && strcmp(ingre->getnombre(), "harina") == 0){
					harina = true;
					break; 
				}
			}
			if (!harina) {
				platosSH.push_back(p);
			}
		}
		ofstream archivo(direcarchi);
		if (archivo.fail()){
			return;
		}
		for (const auto& bucle : platosSH){
			archivo << bucle->getid() << " " << bucle->getnombre() << endl;
		}
		archivo.close();
	}
};


int main(int argc, char *argv[]) {
	
	return 0;
}

