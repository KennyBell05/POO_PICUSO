#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;

class Alumno {
protected:
	int codigo;
	char nombreyapellido[200];
public:
	Alumno(int cod, const char* nya) {
		codigo = cod;
		strcpy(nombreyapellido, nya);
	}
	virtual ~Alumno() {}
	virtual int getcodigo() const = 0;
	virtual const char* getnya() const = 0;
	virtual void cargarnotas(ofstream &archivo) const = 0;
	virtual float calcularProm() const = 0;
	virtual const char* getTipo() const = 0;
};

class Invitado : public Alumno {
private:
	float nota;
public:
	Invitado(int cod, const char* nya, float n) : Alumno(cod, nya) {
		nota = n;
	}
	int getcodigo() const override {
		return this->codigo;
	}
	const char* getnya() const override {
		return this->nombreyapellido;
	}
	void cargarnotas(ofstream &archivo) const override {
		archivo.write(reinterpret_cast<const char*>(&nota), sizeof(float));
	}
	float calcularProm() const override {
		return nota;
	}
	const char* getTipo() const override {
		return "Invitado";
	}
};

class Medio : public Alumno {
private:
	float notasmed[3];
public:
	Medio(int cod, const char* nya, float n1, float n2, float n3) : Alumno(cod, nya) {
		notasmed[0] = n1;
		notasmed[1] = n2;
		notasmed[2] = n3;
	}
	int getcodigo() const override {
		return this->codigo;
	}
	const char* getnya() const override {
		return this->nombreyapellido;
	}
	void cargarnotas(ofstream &archivo) const override {
		archivo.write(reinterpret_cast<const char*>(notasmed), 3 * sizeof(float));
	}
	float calcularProm() const override {
		float total = 0;
		for (int i = 0; i < 3; i++) {
			total += notasmed[i];
		}
		return total / 3;
	}
	const char* getTipo() const override {
		return "Medio";
	}
};

class Premium : public Alumno {
private:
	float notasprem[5];
public:
	Premium(int cod, const char* nya, float n1, float n2, float n3, float n4, float n5) : Alumno(cod, nya) {
		notasprem[0] = n1;
		notasprem[1] = n2;
		notasprem[2] = n3;
		notasprem[3] = n4;
		notasprem[4] = n5;
	}
	int getcodigo() const override {
		return this->codigo;
	}
	const char* getnya() const override {
		return this->nombreyapellido;
	}
	void cargarnotas(ofstream &archivo) const override {
		archivo.write(reinterpret_cast<const char*>(notasprem), 5 * sizeof(float));
	}
	float calcularProm() const override {
		float total = 0;
		for (int i = 0; i < 5; i++) {
			if (notasprem[i] < 70) {
				return 0;
			}
			total += notasprem[i];
		}
		return total / 5;
	}
	const char* getTipo() const override {
		return "Premium";
	}
};

class Curso {
private:
	int cod_curso;
	vector<Alumno*> alumnos;
public:
	Curso(int cod){
		cod_curso = cod;
	}
	
	void cargaralumno(Alumno* alumn) {
		alumnos.push_back(alumn);
	}

	void cargararchivo(string direcarchivo) {
		if (alumnos.empty()) {
			cout << "TA VACIO EL VECTOR" << endl;
			return;
		}
		
		ofstream archivo(direcarchivo, ios::binary);
		if (archivo.fail()) {
			return;
		}
		
		int tamanio = alumnos.size();
		archivo.write(reinterpret_cast<const char*>(&tamanio), sizeof(tamanio));
		for (const auto& bucle : alumnos) {
			int codigo = bucle->getcodigo();
			archivo.write(reinterpret_cast<const char*>(&codigo), sizeof(int));
			size_t tam = strlen(bucle->getnya()) + 1;
			archivo.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
			archivo.write(bucle->getnya(), tam);
			bucle->cargarnotas(archivo);
		}
		
		archivo.close();
	}
	
	vector<Alumno*> alumnosPromocionados() {
		vector<Alumno*> alumnosProm;
		for (const auto& bucle : alumnos) {
			if (strcmp(bucle->getTipo(), "Invitado") == 0 && bucle->calcularProm() > 60) {
				alumnosProm.push_back(bucle);
			}
			if (strcmp(bucle->getTipo(), "Medio") == 0 && bucle->calcularProm() > 70) {
				alumnosProm.push_back(bucle);
			}
			if (strcmp(bucle->getTipo(), "Premium") == 0 && bucle->calcularProm() > 80) {
				alumnosProm.push_back(bucle);
			}
		}
		return alumnosProm;
	}
	
	void promocionarAlumno(int code, const char* newestado) {
		for (size_t i = 0; i < alumnos.size(); i++) {
			if (alumnos[i]->getcodigo() == code) {
				if (strcmp(alumnos[i]->getTipo(), newestado) == 0) {
					cout << "El alumno ya es de tipo " << newestado << endl;
					return;
				}
				
				Alumno* nuevoAlumno = nullptr;
				float nota = alumnos[i]->calcularProm();
				if (strcmp(alumnos[i]->getTipo(), "Invitado") == 0) {
					if (strcmp(newestado, "Medio") == 0) {
						float n2, n3;
						cout << "Ingrese dos notas adicionales para Medio: ";
						cin >> n2 >> n3;
						nuevoAlumno = new Medio(code, alumnos[i]->getnya(), nota, n2, n3);
					} else if (strcmp(newestado, "Premium") == 0) {
						float n2, n3, n4, n5;
						cout << "Ingrese cuatro notas adicionales para Premium: ";
						cin >> n2 >> n3 >> n4 >> n5;
						nuevoAlumno = new Premium(code, alumnos[i]->getnya(), nota, n2, n3, n4, n5);
					}
				}
				
				if (nuevoAlumno) {
					delete alumnos[i]; 
					alumnos[i] = nuevoAlumno;
					cout << "Alumno promocionado correctamente a " << newestado << endl;
					return;
				}
			}
		}
	}
	
	int getCodigoCurso() const {
		return cod_curso;
	}
	
	vector <Alumno*>& getAlumnos(){
		return alumnos;
	}
	
};

class Gestor {
private:
	map<int, Curso*> cursos;
public:
	void agregaracurso(Curso* curso) {
		cursos[curso->getCodigoCurso()] = curso;
	}
	void mostrarcursos(){
		if (cursos.empty()){
			return;
		}
		for (const auto& bucle : cursos){
			cout << "CURSO: " << bucle.first << endl;
		}
	}
	void agregararchivo(Curso* curso, string direcarchivo){
		if (cursos.empty()){
			return;
		}
		ofstream archivo (direcarchivo, ios :: binary);
		if (archivo.fail()){
			return;
		}
		int codecurso = curso->getCodigoCurso();
		archivo.write(reinterpret_cast<const char*>(&codecurso), sizeof(codecurso));
		vector<Alumno*>& alumnos = curso->getAlumnos();
		for (const auto& bucle : alumnos){
			int codigo = bucle->getcodigo();
			archivo.write(reinterpret_cast<const char*>(&codigo), sizeof(codigo));
			
			// Guardar el nombre y apellido del alumno
			size_t tam = strlen(bucle->getnya()) + 1;
			archivo.write(reinterpret_cast<const char*>(&tam), sizeof(tam));
			archivo.write(bucle->getnya(), tam);
			
			// Guardar las notas del alumno
			bucle->cargarnotas(archivo);
		}
		archivo.close();
	}
};

int main(int argc, char *argv[]) {
	Gestor gestor;
	Curso* curso1 = new Curso(101);
	Alumno* alumno1 = new Invitado(1, "Juan Perez", 75);
	curso1->cargaralumno(alumno1);
	gestor.agregaracurso(curso1);
	
	return 0;
}
