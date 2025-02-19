#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
using namespace std;

class Bodega{
private:
	int nro;
	char tipo;
	int cantidad_litros;
	int anio_produccion;
public:
	Bodega(int n, char t, int cl, int ap){
		nro = n;
		tipo = t;
		cantidad_litros = cl;
		anio_produccion = ap;
	}
	virtual char getTipo() = 0;
	virtual ~Bodega(){};
};

class Mesa : public Bodega{
private:
	float precio;
	int anio_barrica;
public:
	Mesa (int nro, char tipo, int cantidad_litros, int anio_produccion, float pre = 120, int ab = 1) : Bodega(nro, tipo, cantidad_litros, anio_produccion){
		precio = pre;
		anio_barrica = ab;
	}
	char getTipo(){
		return tipo;
	}
};

class Premium : public Bodega{
private:
	float precio;
	int anio_barrica;
public:
	Premium (int nro, char tipo, int cantidad_litros, int anio_produccion, float pre = 200, int ab = 2) : Bodega(nro, tipo, cantidad_litros, anio_produccion){
		precio = pre;
		anio_barrica = ab;
	}
	char getTipo(){
		return tipo;
	}

};

class Especial : public Bodega{
private:
	float precio;
	int anio_barrica;
	int anio_botella;
public:
	Especial (int nro, char tipo, int cantidad_litros, int anio_produccion, float pre = 320, int ab = 2, int anibote = 1) : Bodega(nro, tipo, cantidad_litros, anio_produccion){
		precio = pre;
		anio_barrica = ab;
		anio_botella = anibote;
	}
	char getTipo(){
		return tipo;
	}

};

class Gestor{
private:
	vector<Bodega*> totalvinos;
public:
	void agregarvino(Bodega* vino){
		totalvinos.push_back(vino);
	}
	void vinosentiempo(int anio){
		if (totalvinos.empty()){
			return;
		}
		int aniosmaduro;
		for (const auto& bucle : totalvinos){
			if (bucle->getTipo() == 'M'){
				///HAGO UN BOOL
			} else if (bucle->getTipo() == 'P'){
				///HAGO UN BOOL
			} else if (bucle->getTipo() == 'E') {
				///HAGO UN BOOL
			}
			
		}
	}
};

void leerarchivo(string direcarchivo, Gestor* vinos){
	ifstream archivo(direcarchivo, ios :: binary);
	if (archivo.fail()){
		return;
	}
	while (archivo.peek() != EOF){
		int nro;
		char tipo;
		int	cantidadlitros; 
		int anioprodu;
		
		archivo.read(reinterpret_cast<char*>(&nro), sizeof(nro));
		archivo.read(&tipo, sizeof(tipo));
		archivo.read(reinterpret_cast<char*>(&cantidadlitros), sizeof(cantidadlitros));  // Leer cantidad_litros
		archivo.read(reinterpret_cast<char*>(&anioprodu), sizeof(anioprodu)); 
		
		if (tipo == 'M'){
			Mesa* mes = new Mesa(nro, tipo, cantidadlitros, anioprodu);
			vinos->agregarvino(mes);
		} else if (tipo == 'P'){
			Premium* prem = new Premium(nro, tipo, cantidadlitros, anioprodu);
			vinos->agregarvino(prem);
		} else if (tipo == 'E'){
			Especial* espe = new Especial(nro, tipo, cantidadlitros, anioprodu);
			vinos->agregarvino(espe);
		}
	}
	archivo.close();
}


int main(int argc, char *argv[]) {
	Gestor* vinos = new Gestor();
	string direcarchivo = "vinos_en_punga.det";
	leerarchivo(direcarchivo);
	return 0;
}

