#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>
using namespace std;

class Costoextra {
public:
	virtual int getcosto() const = 0;
	virtual int gettipo() const = 0;
};

class Impuesto : public Costoextra {
private:
	int monto;
public:
	Impuesto(int m) : monto(m) {}
	int gettipo() const override {
		return 1;  // 1 ES IMPUESTO
	}
	int getcosto() const override {
		return monto;
	}
};

class Manodeobra : public Costoextra {
private:
	int horastrabajo;
	int costoxhora;
public:
	Manodeobra(int horas, int porhora) : horastrabajo(horas), costoxhora(porhora) {}
	int gettipo() const override {
		return 2;  // 2 MANOOBRA
	}
	int getcosto() const override {
		return horastrabajo * costoxhora;
	}
};

class Ingrediente {
protected:
	int id;
	char nombre[100];
	int costo;
public:
	Ingrediente(int i, const char* nom, int c){
		id = i;
		strncpy(nombre, nom, sizeof(nombre));
		nombre[sizeof(nombre) - 1] = '\0';
		costo = c;
	}
	virtual int getcosto() const {
		return costo;
	}
	virtual const char* getnombre() const {
		return nombre;
	}
	int getid() const {
		return id;
	}
};

class IngredienteBasico : public Ingrediente {
public:
	IngredienteBasico(int id, const char* nom, int c) : Ingrediente(id, nom, c) {}
};

class Plato : public Ingrediente {
private:
	vector<Ingrediente*> ingredientes_ba;
	vector<Plato*> ingredientes_pl;
	vector<Costoextra*> costos_extras;
public:
	Plato(int id, const char* nom) : Ingrediente(id, nom, 0) {}
	
	void agregaringrediente(Ingrediente* ing) {
		ingredientes_ba.push_back(ing);
	}
	
	void agregarplato(Plato* p) {
		ingredientes_pl.push_back(p);
	}
	
	void agregarcostoextra(Costoextra* c) {
		costos_extras.push_back(c);
	}
	
	int calcularcosto() const {
		int total = 0;
		for (auto& bucle : ingredientes_ba) {
			total += bucle->getcosto();
		}
		for (auto& bucle : ingredientes_pl) {
			total += bucle->calcularcosto();
		}
		for (auto& bucle : costos_extras) {
			total += bucle->getcosto();
		}
		return total;
	}
	
	const vector<Ingrediente*>& getingredientes() const {
		return ingredientes_ba;
	}
	
	const vector<Plato*>& getplatos() const {
		return ingredientes_pl;
	}
	
	const vector<Costoextra*>& getcostosextras() const {
		return costos_extras;
	}
};

class Gestor {
private:
	vector<Plato*> totalplatos;
public:
	void agregarPlato(Plato* p) {
		totalplatos.push_back(p);
	}
	
	void mostrarCostos() const {
		if (totalplatos.empty()) {
			return;
		}
		for (auto& plato : totalplatos) {
			cout << plato->getnombre() << plato->calcularcosto() << endl;
		}
	}
	
	void guardararchivo(string direcarchivo) {
		if (totalplatos.empty()) {
			return;
		}
		ofstream archivo(direcarchivo, ios::binary);
		if (archivo.fail()){
			return;
		}
		for (auto& plato : totalplatos) {
			int tamanioplat = strlen(plato->getnombre()) + 1;
			archivo.write(reinterpret_cast<char*>(&tamanioplat), sizeof(tamanioplat));
			archivo.write(plato->getnombre(), tamanioplat);
			int cantingredientes = plato->getingredientes().size();
			int cantplatos = plato->getplatos().size();
			int cantcostos = plato->getcostosextras().size();
			archivo.write(reinterpret_cast<char*>(&cantingredientes), sizeof(cantingredientes));
			archivo.write(reinterpret_cast<char*>(&cantplatos), sizeof(cantplatos));
			archivo.write(reinterpret_cast<char*>(&cantcostos), sizeof(cantcostos));
			for (auto& ing : plato->getingredientes()) {
				int tamnombre = strlen(ing->getnombre()) + 1;
				archivo.write(reinterpret_cast<char*>(&tamnombre), sizeof(tamnombre));
				archivo.write(ing->getnombre(), tamnombre);
				int costo = ing->getcosto();
				archivo.write(reinterpret_cast<char*>(&costo), sizeof(costo));
			}
			for (auto& p : plato->getplatos()) {
				int tamaniosubplato = strlen(p->getnombre()) + 1;
				archivo.write(reinterpret_cast<char*>(&tamaniosubplato), sizeof(tamaniosubplato));
				archivo.write(p->getnombre(), tamaniosubplato);
				int subcantingredientes = p->getingredientes().size();
				int subcantplatos = p->getplatos().size();
				int subcantcostos = p->getcostosextras().size();
				archivo.write(reinterpret_cast<char*>(&subcantingredientes), sizeof(subcantingredientes));
				archivo.write(reinterpret_cast<char*>(&subcantplatos), sizeof(subcantplatos));
				archivo.write(reinterpret_cast<char*>(&subcantcostos), sizeof(subcantcostos));
				for (auto& subing : p->getingredientes()) {
					int tamaniosubingredientes = strlen(subing->getnombre()) + 1;
					archivo.write(reinterpret_cast<char*>(&tamaniosubingredientes), sizeof(tamaniosubingredientes));
					archivo.write(subing->getnombre(), tamaniosubingredientes);
					int subcosto = subing->getcosto();
					archivo.write(reinterpret_cast<char*>(&subcosto), sizeof(subcosto));
				}
				for (auto& subcosto : p->getcostosextras()) {
					int tipo = subcosto->gettipo();
					int valor = subcosto->getcosto();
					archivo.write(reinterpret_cast<char*>(&tipo), sizeof(tipo));
					archivo.write(reinterpret_cast<char*>(&valor), sizeof(valor));
				}
			}
			for (auto& costo : plato->getcostosextras()) {
				int tipo = costo->gettipo();
				int valor = costo->getcosto();
				archivo.write(reinterpret_cast<char*>(&tipo), sizeof(tipo));
				archivo.write(reinterpret_cast<char*>(&valor), sizeof(valor));
			}
		}
		archivo.close();
	}
	void ordenar() {
		if (totalplatos.empty()) {
			return;
		}
		sort(totalplatos.begin(), totalplatos.end(), [](Plato* a, Plato* b) {
			return a->calcularcosto() < b->calcularcosto();
		});
	}
	void topbaratos() {
		if (totalplatos.empty()) {
			return;
		}
		ofstream archivo2 ("baratos");
		if (archivo2.fail()){
			return;
		}
		ordenar(); 
		for (int i = 0; i < 5 ; i++) {
			archivo2 << totalplatos[i]->getnombre() << ":" << totalplatos[i]->calcularcosto() << endl;
		}
	}
};

int main(int argc, char *argv[]) {
	
	return 0;
}

/// TEORIA 1: si permite tratar a los objetos de tipo ingrediente y plato de manera unica, aunque sean instancias de clases diferentes,
/// en un principio use composicion pero despues me di cuenta que se puede usar polimorfismo para un mejor manejo y mas comodo 

/// TEORIA 4: los conceptos se relacionan en que la herencia permite ahorrar codigo con atri y met mediante jerarquias de clases, la composicion
/// tambien permite reutilizar codigo pero sin jerarquia, con la inclusion de objetos de una clase dentro de otra, y reutilizacion de codigo 
/// seria como una fusion de ambos conceptos, que nos ahorra codigo y tambien lo hace reutilizable
