#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <map>
using namespace std;

class Plan{
public:
	virtual int getpromedio(vector<int> notas) = 0;
	virtual bool aprobo(vector<int> notas) = 0;
	virtual const char* getplan() = 0;
	virtual ~Plan(){}
};

class Invitado : public Plan{
public:
	int getpromedio(vector<int> notas) {
		if (notas.empty()){
			return 0;
		}
		int acum = 0;
		int tamanio = notas.size();
		for (const auto& n : notas){
			acum += n;
		}
		int promedio = acum / tamanio;
		return promedio;
	}
	bool aprobo(vector<int> notas){
		if (notas.empty()){
			return false;
		}
		return getpromedio(notas) > 60;
	}
	const char* getplan(){
		return "Invitado";
	}
};

class Medio : public Plan{
public:
	int getpromedio(vector<int> notas) {
		if (notas.empty()){
			return 0;
		}
		int acum = 0;
		int tamanio = notas.size();
		for (const auto& n : notas){
			acum += n;
		}
		int promedio = acum / tamanio;
		return promedio;
	}
	bool aprobo(vector<int> notas){
		int tamanio = notas.size();
		if (tamanio < 3){
			return false;
		}
		return getpromedio(notas) > 70;
	}
	const char* getplan(){
		return "Medio";
	}
};

class Premium : public Plan{
public:
	int getpromedio(vector<int> notas) {
		if (notas.empty()){
			return 0;
		}
		int acum = 0;
		int tamanio = notas.size();
		for (const auto& n : notas){
			acum += n;
		}
		int promedio = acum / tamanio;
		return promedio;
	}
	bool aprobo(vector<int> notas){
		int tamanio = notas.size();
		if (tamanio < 5){
			return false;
		}
		for (const auto& n : notas){
			if (n < 70){
				return false;
			}
		}
		return getpromedio(notas) > 80;
	}
	const char* getplan(){
		return "Premium";
	}
};

class Alumno{
private:
	int id;
	char nya[100];
	vector<int> notas;
	Plan* plan;
public:
	Alumno(int i, const char* n, Plan* p){
		id = i;
		strcpy(nya, n);
		plan = p;
	}
	void agregarnota(int n){
		notas.push_back(n);
	}
	int getid(){
		return id;
	}
	const char* getnya(){
		return nya;
	}
	vector<int> getnotas(){
		return notas;
	}
	const char* getnombreplan(){
		return plan->getplan();
	}
	int getpromedio(){
		return plan->getpromedio(notas);
	}
	bool aprobo(){
		return plan->aprobo(notas);
	}
	void setplan(Plan* p){
/*		delete plan;*/
		plan = p;
	}
};

class Curso{
private:
	int id_curso;
	char nombrecurso[100];
	vector<Alumno*> alumnos;
public:
	Curso(int i, const char* nc){
		id_curso = i;
		strcpy(nombrecurso, nc);
	}
	void pushalumno(Alumno* al){
		alumnos.push_back(al);
	}
	int getid(){
		return id_curso;
	}
	const char* getnombrecurso(){
		return nombrecurso;
	}
	vector<Alumno*> getalumnos(){
		return alumnos;
	}
};

class Gestor{
private:
	vector<Curso*> cursos;
public:
	void pushcurso(Curso* cur){
		cursos.push_back(cur);
	}
	void creararchivos(string direc1, string direc2, string direc3){
		if (cursos.empty()){
			return;
		}
		ofstream archivocursos(direc1, ios :: binary);
		ofstream archivoalumnos(direc2, ios :: binary);
		ofstream archivonotas(direc3, ios :: binary);
		if (archivocursos.fail() || archivoalumnos.fail() || archivonotas.fail()){
			return;
		}
		for (const auto& c : cursos){ /// ARCHIVO DE CURSOS
			int id = c->getid();
			archivocursos.write(reinterpret_cast<const char*>(&id), sizeof(int));
			int tam = strlen(c->getnombrecurso());
			archivocursos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
			archivocursos.write(c->getnombrecurso(), tam);
		}
		archivocursos.close();
		for (const auto& c : cursos){ /// ARCHIVO DE ALUMNOS
			for (const auto& a : c->getalumnos()){
				int id = a->getid();
				archivoalumnos.write(reinterpret_cast<const char*>(&id), sizeof(int));
				int tam = strlen(a->getnya());
				archivoalumnos.write(reinterpret_cast<const char*>(&tam), sizeof(int));
				archivoalumnos.write(a->getnya(), tam);
				int tamplan = strlen(a->getnombreplan());
				archivoalumnos.write(reinterpret_cast<const char*>(&tamplan), sizeof(int));
				archivoalumnos.write(a->getnombreplan(), tamplan);
			}
		}
		archivoalumnos.close();
		for (const auto& c : cursos){ /// ARCHIVO DE NOTAS
			for (const auto& a : c->getalumnos()){
				int id = a->getid();
				archivonotas.write(reinterpret_cast<const char*>(&id), sizeof(int));
				int cant = a->getnotas().size();
				archivonotas.write(reinterpret_cast<const char*>(&cant), sizeof(int));
				for (const auto& n : a->getnotas()){
					archivonotas.write(reinterpret_cast<const char*>(&n), sizeof(int));
				}
			}
		}
		archivonotas.close();
	}
	void aprobaron(){
		if (cursos.empty()){
			return;
		}
		for (const auto& c : cursos){
			for (const auto& a : c->getalumnos()){
				if (a->aprobo()){
					cout << "CODIGO: " << a->getid() << " NOMBRE: " << a->getnya() << endl;;
				}
			}
		}
	}
	void cambiar_plan(int codigo){
		if (cursos.empty()){
			return;
		}
		for (const auto& c : cursos){
			for (const auto& a : c->getalumnos()){
				if (a->getid() == codigo){
					if (strcmp(a->getnombreplan(),"Invitado")== 0){
						int opcion;
						cout << "Ingrese 1 para ser medio y 2 para ser Premium: " << endl << ">";
						cin >> opcion;
						if (opcion == 1){
							a->setplan(new Medio());
							int nuevanota;
							while (a->getnotas().size() < 3){
								cout << "Ingrese la nota adicional: " << endl << ">";
								cin >> nuevanota;
								a->agregarnota(nuevanota);
							}
						} else if (opcion == 2){
							a->setplan(new Premium());
							while (a->getnotas().size() < 5){
								int nuevanota;
								cout << "Ingrese la nota adicional: " << endl << ">";
								cin >> nuevanota;
								a->agregarnota(nuevanota);
							}
						}
						// Mostrar la información del alumno después de cambiarle el plan
						cout << "Alumno con ID: " << a->getid() << endl;
						cout << "Nombre: " << a->getnya() << endl;
						cout << "Nuevo plan: " << a->getnombreplan() << endl;
						cout << "Promedio: " << a->getpromedio() << endl;
						cout << "Notas: ";
						for (const auto& nota : a->getnotas()) {
							cout << nota << " ";
						}
						cout << endl;
					}
				}
			}
		}
	}
	int mejorpromedio(vector<Alumno*> alumnos){
		if (alumnos.empty()){
			return 0;
		}
		auto maxalumno = *max_element(alumnos.begin(), alumnos.end(), [](Alumno* a, Alumno* b){
			return a->getpromedio() < b->getpromedio();
		});
		return maxalumno->getpromedio();
	}
	void topnotas(){
		if (cursos.empty()){
			return;
		}
		int max = -1;
		for (const auto& c : cursos){
			int promedio = mejorpromedio(c->getalumnos());
			if (promedio > max){
				max = promedio;
			}
		}
		if (max == -1) {
			cout << "No hay alumnos con notas registradas." << endl;
			return;
		}
		for (const auto& c : cursos){
			for (const auto& a : c->getalumnos()){
				if (a->getpromedio() == max){
					cout << "Alumno: " << a->getnya() << " Promedio: " << a->getpromedio() << endl;
				}
			}
		}
	}
	void saber_tipos(){
		if (cursos.empty()){
			return;
		}
		map<string, int> cantidades;
		for (const auto& c : cursos){
			for (const auto& a : c->getalumnos()){
				cantidades[a->getnombreplan()]++;
			}
		}
		cout << "Cantidad de alumnos por tipo de plan:" << endl;
		for (const auto& par : cantidades){
			cout << "Plan: " << par.first << " - Cantidad: " << par.second << endl;
		}
	}
};


int main(int argc, char *argv[]) {
	// Crear algunos planes
	Plan* invitado = new Invitado();
	Plan* medio = new Medio();
	Plan* premium = new Premium();
	
	// Crear algunos alumnos
	Alumno* alumno1 = new Alumno(1, "Juan", invitado);
	Alumno* alumno2 = new Alumno(2, "Maria", medio);
	Alumno* alumno3 = new Alumno(3, "Pedro", premium);
	Alumno* alumno4 = new Alumno(4, "PIPINA", invitado);
	
	// Asignar notas a los alumnos
	alumno1->agregarnota(99);
	alumno2->agregarnota(80);
	alumno2->agregarnota(75);
	alumno2->agregarnota(85);
	alumno3->agregarnota(99);
	alumno3->agregarnota(99);
	alumno3->agregarnota(99);
	alumno3->agregarnota(99);
	alumno3->agregarnota(99);
	alumno4->agregarnota(100);
	
	// Crear algunos cursos
	Curso* curso1 = new Curso(1, "Matematicas");
	Curso* curso2 = new Curso(2, "Fisica");
	
	// Agregar alumnos a los cursos
	curso1->pushalumno(alumno1);
	curso1->pushalumno(alumno2);
	curso2->pushalumno(alumno3);
	curso2->pushalumno(alumno4);
	
	// Crear el gestor de cursos
	Gestor gestor;
	gestor.pushcurso(curso1);
	gestor.pushcurso(curso2);
	
	// Crear los archivos binarios con la información de cursos, alumnos y notas
	gestor.creararchivos("cursos.dat", "alumnos.dat", "notas.dat");
	
	// Mostrar los alumnos que aprobaron
	cout << "Alumnos que aprobaron:" << endl;
	gestor.aprobaron();
	
	// Cambiar el plan de un alumno
	gestor.cambiar_plan(1); // Cambiar el plan del alumno con ID 1
	
	// Mostrar los mejores promedios
	cout << "Top de mejores promedios:" << endl;
	gestor.topnotas();
	gestor.saber_tipos();
	

	return 0;
}

