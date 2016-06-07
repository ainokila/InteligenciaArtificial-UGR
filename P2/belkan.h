
#ifndef AGENT__
#define AGENT__

#include <string>
#include <iostream>
#include <cstdlib>
#include "conexion.h"
#include "queue"

using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
		//Variables de posicionamiento de mapa BUENO ->

		primerK = false;
		segK = false;
		nEjecucion = 0;
		x_= 99;
		y_= 99;
		x=-1;
		y=-1;
		orientacion_=3;
		role_="PlYR";
		last_accion_=3;
		REINICIADO_=false;
		size_=200;
		for (int i=0;i<200;i++)
		    for(int j=0;j<200;j++){
		      mapa_entorno_[i][j]='?';
		      mapa_objetos_[i][j]='?';
			  mapa_pulgarcito[i][j]= -1; //Mapa para la tecnica de PULGARCITO!
		    }

		for (int i=0;i<100;i++)
		    for(int j=0;j<100;j++)
		      mapa_solucion_[i][j]='?';

		contadorPulgarcito = 0;
		orientacion_mapa = -1; // Por defecto a -1 , ningun posible cambio.

	}

	enum ActionType
	{
	    actFORWARD, // avanzar
	    actTURN_L,  // Girar Izquierda
	    actTURN_R,  // Girar Derecha
	    actIDLE,    // No hacer nada
	    actPICKUP,  // Recoger un objeto
	    actPUTDOWN, // Soltar un objeto
	    actPUSH,    // Meter en la mochila
	    actPOP,     // Sacar de la mochila
	    actGIVE,	// Dar un objeto a un personaje
	    actTHROW	// Lanzar un objeto

	};

	void Perceive(Environment &env);
	bool Perceive_Remote(conexion_client &Cliente, Environment &env);
	void ActualizarInformacion(Environment *env);
	ActionType Think();
	void FixLastAction(Agent::ActionType x){last_accion_=x;};

	char mapa_entorno_[200][200]; // mapa que muestra el tipo de terreno
	char mapa_objetos_[200][200]; // mapa que muestra los objetos que estan encima del terreno
	char mapa_solucion_[100][100]; // Mapa que almacena la solucion que el alumno propone

	int mapa_pulgarcito[200][200]; // Mapa para la tecnica de PULGARCITO!
	
	int contadorPulgarcito ;

	// Funciones de acceso a los datos
	void GetCoord(int &fila, int &columna, int &brujula){fila=y_;columna=x_;brujula=orientacion_;};

	bool libreParaPasar(int x);


	void calcularOrientacion();

	void buscaK();

	ActionType siguienteMovimiento(int x , int y );

	/*********FUNCIONES AUXILIARES PULGARCITO******************/
	int delante(int x , int y );
	int izquierda(int x , int y );
	int derecha(int x , int y );
	bool libreParaPasar(int x, int y);

	//********************************************************/

	void rellenaMapa();

	//<------- FUNCIONES PARA ACTUALIZAR LA X E Y REALES DEPENDIENDO DE COMO AVANCE ----->
	void cambiaXY0();
	void cambiaXY1();
	void cambiaXY2();
	void cambiaXY3();


	void pintaMapita();
	char colorMayoritario(int,int);


private:
	//Variables de interaccion con el entorno grafico
	int size_;	

	//SENSORES
	char VISTA_[10];
	char SURFACE_[10];
	bool REINICIADO_;
	string MENSAJE_;
	char EN_USO_;
	char MOCHILLA_[5];
	char PUNTUACION_[9];
	bool FIN_JUEGO_;
	char SALUD_;

	//Variables de estado
	int x_,y_, orientacion_;
	int last_accion_;
	string role_;

	int x , y;

	bool primerK ;
	bool segK ;

	int orientacion_1 ;
	int orientacion_2 ;

 	//Posiciones buenas y malas del k1 y k2

	int fila_1_buena = 0 ;
	int columna_1_buena= 0 ;
	int fila_2_buena= 0 ;
	int columna_2_buena= 0 ;

	int fila_1_mala= 0 ;
	int columna_1_mala= 0 ;
	int fila_2_mala= 0 ;
	int columna_2_mala= 0 ;

	int orientacion_mapa;

	queue<ActionType> movimientos;

	int nEjecucion;

};

string ActionStr(Agent::ActionType);

#endif
