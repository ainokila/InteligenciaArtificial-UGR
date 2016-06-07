#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cmath>
//#include <unistd.h>
//#include<windows.h>
//#include<netdb.h>
using namespace std;
#include"GUI.h"
using namespace GUI;
#include <cstdlib>
#include "belkan.h"
#include "conexion.h"
#include "dibujosGL.h"
#include "environment.h"
#include "luz.hpp"

const int QUIT_BTN_ID=1;
const int NEWGAME_BTN_ID=102,SELECTMAP_BTN_ID=103,DISPLAY_BTN_ID=104,NEWMAP_BTN_ID=105,REMOTEGAME_BTN_ID=106;
const int NEWMAP_OK_BTN_ID=202,NEWMAP_CANCEL_BTN_ID=203;
const int SELECTMAP_SELECT_ID=302,SELECTMAP_CANCEL_BTN_ID=303;
const int DO_ONE_STEP_BTN_ID=402,DO_ONE_RUN_BTN_ID=403,NEXT_RUN_BTN_ID=404,DO_ALL_RUN_BTN_ID=405;
const int REINICIAR_BTN_ID=500, CURSOR_PUT1_BTN_ID=501, CURSOR_PUT2_BTN_ID=502, CURSOR_PUT3_BTN_ID=503,
CURSOR_PUT4_BTN_ID=504, CURSOR_PUT5_BTN_ID=505, CURSOR_PUT6_BTN_ID=506, CURSOR_PUT7_BTN_ID=507,CURSOR_BOOM_BTN_ID=508;
const int ERROR_OK_BTN_ID=602;

int main_window, punto_vista;
char field_ip_server[1024]="150.214.190.149",field_nickname[1024]="Pepe",field_ip_ninja[1024]="150.214.190.149";
char display_step[20]="10",display_time[20]="10";
char ip_server[1024],mi_nick[1024],file_name_temp[1024], nick_adversario[1024]="Adversario";
int tx,ty,tw,th;
float magnification_x,magnification_y;
long long life_time=20000,current_time,total_runs=10,current_run,dirty_degree,consumed_energy;
long long total_dirty_degree,total_consumed_energy;
char time_step_msg[100],action_msg[100],dirty_degree_msg[100],consumed_energy_msg[100];


char life_time_input[1000]="20000",total_runs_input[1000]="10",file_name_input[1000]="map/mapa.map";
char life_time_temp[1000],total_runs_temp[1000];

// Variables para pintar la escena

GLfloat anguloCuboX = 0.2;
GLfloat anguloCuboY = 0;
GLfloat anguloCuboZ = 0;

GLfloat anguloEsfera = 0.0f;

bool activarLuz =false;
GLfloat luz_y=100.0f;
 
GLint ancho=200;

GLint alto=200;

GLfloat anguloPespectiva =15.0f;
GLfloat distanciaObjetoCercano =1.0f;
GLfloat distanciaObjetoLejano = 40.0f;

GLfloat incrx =0;
GLfloat incry =0;

char vision[9][5];
char objetos[9][5];

int secuencia=0;
Agent::ActionType UltimaAccion;
int puntuacion_mapa=0;
int finDeJuegoProcesado=false;

void GUI::cerrar(){

	cout << "Cerrando ventana\n";
}



int modo=0, jugador=0;

Environment *env=NULL;
Agent *agent=NULL;
int action=3;

ifstream fin;
conexion_client Cliente;
conexion_server Servidor;
bool partida_remota=false, esperando_server=false, esperando_client=false;
int conectado;

GLUI *main_glui,*score_glui,*new_map_glui,*select_map_glui,*error_glui, *cursor_glui, *punto_vista_glui;
GLUI_Button *remote_map_btn,*select_map_btn,*new_map_ok_btn,*new_map_cancel_btn;
GLUI_Button *do_one_step_btn,*display_btn,*do_one_run_btn;
GLUI_Button *cursor_put1, *cursor_put2, *cursor_put3, *cursor_put4, *cursor_put5, *cursor_put6, *cursor_put7, *cursor_boom;
GLUI_Button *reiniciar_btn;
GLUI_Button *quit_btn;
GLUI_StaticText *terreno_text,*EnUso_text,*boolean_text,*dirty_degree_text,*consumed_energy_text,*robot_text;
GLUI_StaticText *mensaje_text,*total_score_text,*average_score_text, *juega_txt, *espera_txt;
GLUI_FileBrowser *map_list;
GLUI_RadioGroup *radio1, *radio2;
GLUI_Panel *obj_panel1, *obj_panel2;
GLUI_EditText *obj_ip, *obj_mi_nick;



int GUI::startDraw(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(668,693);

    //glutCloseFunc(cerrar);
    //glutWMCloseFunc(cerrar);
    //glutSetOption ( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );

	main_window=glutCreateWindow("PRACTICA 2: BelKan's World");
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	glutDisplayFunc(myGlutDisplay);
	glutKeyboardFunc(keyboard);

	glEnable(GL_DEPTH_TEST);

	score_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_BOTTOM);
	new GLUI_StaticText(score_glui,""); 
	terreno_text = new GLUI_StaticText(score_glui,"");
	new GLUI_StaticText(score_glui,""); 
	EnUso_text = new GLUI_StaticText(score_glui,"");
	new GLUI_StaticText(score_glui,""); 
	boolean_text = new GLUI_StaticText(score_glui,"");
	new GLUI_StaticText(score_glui,""); 
	mensaje_text = new GLUI_StaticText(score_glui,"");
	new GLUI_StaticText(score_glui,""); 

    	new GLUI_StaticText(score_glui,"");//以空白行做分隔
    	robot_text = new GLUI_StaticText(score_glui,"");		

    	score_glui->set_main_gfx_window( main_window );
    	main_glui=GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_RIGHT);
    	new GLUI_StaticText(main_glui,""); 
    	select_map_btn=new GLUI_Button(main_glui,"Elegir Mapa",SELECTMAP_BTN_ID,control_cb);
    	remote_map_btn=new GLUI_Button(main_glui,"Juego Remoto",REMOTEGAME_BTN_ID,control_cb);
    	new GLUI_StaticText(main_glui,""); 
    	do_one_step_btn=new GLUI_Button(main_glui,"Paso",DO_ONE_STEP_BTN_ID,control_cb);
    	do_one_step_btn->disable();
    	do_one_run_btn=new GLUI_Button(main_glui,"Ejecucion",DO_ONE_RUN_BTN_ID,control_cb);
    	do_one_run_btn->disable();
    	// next_run_btn=new GLUI_Button(main_glui,"Sig. ejecucion",NEXT_RUN_BTN_ID,control_cb);
    	// next_run_btn->disable();
    	// do_all_run_btn=new GLUI_Button(main_glui,"Todas las ejec.",DO_ALL_RUN_BTN_ID,control_cb);
    	// do_all_run_btn->disable();
    	new GLUI_StaticText(main_glui,""); 
    	new GLUI_EditText(main_glui,"Pasos:",display_step);
    	new GLUI_EditText(main_glui,"Retardo:",display_time);
    	new GLUI_StaticText(main_glui,""); 
    	display_btn=new GLUI_Button(main_glui,"Ejecutar",DISPLAY_BTN_ID,control_cb);
    	display_btn->disable();
    	new GLUI_StaticText(main_glui,""); 

    	new GLUI_Button(main_glui,"Salir",QUIT_BTN_ID, control_cb );
    	main_glui->set_main_gfx_window( main_window );


    	// Ventana de punto de vista
		glutInitWindowPosition(720,50);
		glutInitWindowSize(550,213);

		punto_vista=glutCreateWindow("En primera persona");
		glClearColor (0., 1., 1, 0); 
		GLUI_Master.set_glutReshapeFunc(reshape);
		glutDisplayFunc(display);
	    //glutKeyboardFunc(keyboard);
	    //GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN

	    //glutIdleFunc(idle);
		glEnable(GL_DEPTH_TEST);

		punto_vista_glui= GLUI_Master.create_glui_subwindow(punto_vista,GLUI_SUBWINDOW_BOTTOM);
		//punto_vista_glui->hide();
		punto_vista_glui->set_main_gfx_window( main_window );

    	// punto_vista= glutCreateWindow("En primera persona");
	    // glutDisplayFunc(display);
	    // glutReshapeFunc(reshape);
	    // glutIdleFunc(idle);

    	glutMainLoop();
    	return 0;
    }




void GUI::showScore(){
	if (env!=0){
    	ostringstream sout;
    	char vision[11], movil[11], en_uso, mochila[5], puntuacion[10], salud;
    	string mensaje;
    	bool reiniciado, finjuego;
    	string aux;

		    //env->WhatIsInFrontOfMe(vision, movil, mensaje);
			env->SenSorStatus(vision,movil,mensaje, reiniciado, en_uso, mochila, puntuacion, finjuego, salud, (modo!=0));
		    vision[10]='\0';
		    movil[10]='\0';
			puntuacion[9]='\0';

	    	sout.str("");
	    	sout<<"Terreno: "<< vision << "  Superficie: " << movil << "  Salud: " << salud;
	    	terreno_text->set_text(sout.str().c_str());

	    	sout.str("");
	    	sout<<"En uso: "<< en_uso << "  Mochila: " << mochila 
	    	<< "  Puntuacion: " << puntuacion << " Acierto Mapa: " << puntuacion_mapa;
	    	EnUso_text->set_text(sout.str().c_str());


	    	
	    	sout.str("");
	    	if (finjuego)
		    	sout<<"JUEGO TERMINADO ";
		    else if (reiniciado)
		    	sout << "REINICIADO: Has sido reiniciado";
		    else
		    	sout << "Ultima Accion: " << ActionStr(UltimaAccion);

	    	boolean_text->set_text(sout.str().c_str());


	    	sout.str("");
	    	sout<<"MENSAJE: "<<mensaje;
	    	mensaje_text->set_text(sout.str().c_str());	

	    	sout.str("");
    }
}


void GUI::myGlutReshape(int w,int h){
	GLUI_Master.get_viewport_area(&tx,&ty,&tw,&th);
	glViewport(tx,ty,tw,th);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(tw<=th){
		magnification_x=1;
		magnification_y=(GLfloat)th/(GLfloat)tw;
	}
	else{
		magnification_x=(GLfloat)tw/(GLfloat)th;
		magnification_y=1;
	}
	glOrtho(-10.0*magnification_x,10.0*magnification_x,-10.0*magnification_y,10.0*magnification_y,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
}



void GUI::myGlutDisplay(){
	glClearColor(200.0/255,200.0/255,200.0/255,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(env!=0 and agent!=0){
		gluOrtho2D(-50.0,50.0,-50.0,50.0);
		int fila, columna, orienta;
		if (modo!=0){
			agent->GetCoord(fila,columna,orienta);
			env->PutCoord(fila,columna,orienta);
			env->Show(tw,th,agent->mapa_entorno_, agent->mapa_objetos_);
		}
		else {
			int fila2, columna2, orienta2;
			env->GetCoord(fila2,columna2,orienta2);
			agent->GetCoord(fila,columna,orienta);
			env->PutCoord(fila,columna,orienta);
			env->Show(tw,th,agent->mapa_entorno_, agent->mapa_objetos_);
			env->PutCoord(fila2,columna2,orienta2);
		}
		showScore();
	}
	glFlush();
	glutPostRedisplay();
	glutSwapBuffers();

}


void GUI::myGlutTimer(int x){ // con x==0 actitud bloqueante.
	if (partida_remota){
		if (esperando_server){
			if (!env->JuegoTerminado() and x!=0){
              //Recibir informacion del entorno del servidor
				bool actualizado;
				cout << "Espero entorno del servidor no bloqueante" << endl;
				actualizado = agent->Perceive_Remote(Cliente, *env);
				if (actualizado){
		            agent->ActualizarInformacion(env);
		            current_time++;
					esperando_server=false;
					do_one_step_btn->enable();
					do_one_run_btn->enable();
					display_btn->enable();
					myGlutDisplay();
					display();
				}
			}
			else if (!env->JuegoTerminado() and x==0){
              //Recibir informacion del entorno del servidor en modo bloqueante
				bool actualizado=false;
				cout << "Espero entorno del servidor bloqueante" << endl;
				while (!actualizado)
					actualizado = agent->Perceive_Remote(Cliente, *env);

				if (actualizado){
		            agent->ActualizarInformacion(env);
		            current_time++;
					esperando_server=false;
					do_one_step_btn->enable();
					do_one_run_btn->enable();
					display_btn->enable();
					myGlutDisplay();

				}
			}
			else {
				Cliente.Cerrar_Conexion();
				esperando_server=false;
				esperando_client=false;
				remote_map_btn->enable();
				select_map_btn->enable();
				modo=0;
			}
		}
		else if (esperando_client){
			if (!env->JuegoTerminado()){
              //Recibir Accion
				cout << "Esperando la accion del otro jugador" << endl;
				char ac=Servidor.Recibir_Accion();
				cout << "Accion Recibida: " << (int) (ac) << endl;

              //Actualizar el estado
				action = ac;
				env->AcceptAction(static_cast<Agent::ActionType>(action));
				UltimaAccion=static_cast<Agent::ActionType>(ac);
				esperando_client=false;
				//myGlutDisplay();
              //glutTimerFunc(1000, myGlutTimer2,10);
			}
			else {
				Servidor.Cerrar_Conexion();
				esperando_server=false;
				esperando_client=false;
				remote_map_btn->enable();
				select_map_btn->enable();
				modo=0;
			}

		}
	}
	glutTimerFunc(1000,myGlutTimer,10);
}

bool GUI::Juego_Terminado(){
	bool finished=false;
	char msg[100];

	if (modo==0){
		if (current_time>=life_time){
			env->PutFinJuego();
			finished=true;
			if (!finDeJuegoProcesado){
					puntuacion_mapa = env->Comparar(agent->mapa_solucion_);
					sprintf(msg,"%d",puntuacion_mapa);
					error_glui=GLUI_Master.create_glui("Puntacion Final",0,100,100);
					char error_message[1000]="La valoracion sobre el mapa es ";
					strcat(error_message,msg);
					strcat(error_message," por ciento");
					new GLUI_StaticText(error_glui,error_message);
					new_map_ok_btn=new GLUI_Button(error_glui,"OK",ERROR_OK_BTN_ID,control_cb);
					finDeJuegoProcesado=true;
			}
		}
		return (finished);
	}
	else if (env->JuegoTerminado()){
			if (!finDeJuegoProcesado){
					Cliente.Enviar_Mapa(agent->mapa_solucion_);
					Cliente.Recibir_Msg(msg);
					puntuacion_mapa = atoi(msg);
					error_glui=GLUI_Master.create_glui("Puntacion Final",0,100,100);
					char error_message[1000]="La valoracion sobre el mapa es ";
					strcat(error_message,msg);
					strcat(error_message," por ciento");
					new GLUI_StaticText(error_glui,error_message);
					new_map_ok_btn=new GLUI_Button(error_glui,"OK",ERROR_OK_BTN_ID,control_cb);
					finDeJuegoProcesado=true;
			}

			return true;
	}
	else
		return false;
		
}


bool Existe(const char *fichname){
	fstream f;
	f.open(fichname,fstream::in);
	if (f.is_open()){
		f.close();
		return true;
	}
	else {
		cout << "No existe el fichero " << fichname << endl;
		return false;
	}
}



void GUI::Load_Conf(){
	fstream f("game.conf",fstream::in);
	string aux;
	getline(f,aux);
	strcpy(ip_server,aux.c_str());
	getline(f,aux);
	strcpy(mi_nick,aux.c_str());
	f.close();
}

void GUI::Save_Conf(){
	strcpy(ip_server,field_ip_server);
	strcpy(mi_nick,field_nickname);
	fstream f("game.conf",fstream::out);
	f << ip_server << endl;
	f << mi_nick << endl;
	f.close();
}


void GUI::control_cb(int id){
	bool done=false;

	switch(id){
		case QUIT_BTN_ID:
						  if (partida_remota){
						  	partida_remota=false;
						  	Cliente.Cerrar_Conexion();
						  	if (env!=0)
						  		delete env;
						  	if (agent!=0)
						  		delete agent;
						  }
						  exit(0);
						  break;
		case SELECTMAP_BTN_ID:
			select_map_btn->disable();
			remote_map_btn->disable();
			strcpy(life_time_temp,life_time_input);
			strcpy(total_runs_temp,total_runs_input);
			strcpy(file_name_temp,file_name_input);
			select_map_glui=GLUI_Master.create_glui("Nuevo Juego",0,100,100);
			new GLUI_EditText(select_map_glui,"Numero de pasos maxima:",life_time_temp);
			map_list=new GLUI_FileBrowser(select_map_glui,"",false,SELECTMAP_SELECT_ID,control_cb);
			map_list->fbreaddir("map/");
			new_map_cancel_btn=new GLUI_Button(select_map_glui,"Cancelar",NEWMAP_CANCEL_BTN_ID,control_cb);
			break;

		case REMOTEGAME_BTN_ID:
			//select_map_btn->disable();
			remote_map_btn->disable();
			select_map_btn->disable();
			if (Existe("game.conf")){
				Load_Conf();
			}
			else {
				Save_Conf();
			}
			strcpy(file_name_temp,field_ip_ninja);
			select_map_glui=GLUI_Master.create_glui("New Game Setting",0,100,100);
			obj_mi_nick = new GLUI_EditText(select_map_glui,"Mi Apodo:",mi_nick);
					    //map_list=new GLUI_FileBrowser(select_map_glui,"",false,SELECTMAP_SELECT_ID,control_cb);
			// obj_ip = new GLUI_EditText(select_map_glui,"IP Servidor:",ip_server);
			// obj_ip->set_w(200);
					//conect_btn=new GLUI_Button(select_map_glui,"Conect",CONECT_BTN_ID,control_cb);
			new_map_cancel_btn=new GLUI_Button(select_map_glui,"Ok",SELECTMAP_CANCEL_BTN_ID,control_cb);
			break;
		case DISPLAY_BTN_ID:

			for(long long t=atol(display_step);t>0 && !Juego_Terminado();/*--t*/){
				done=doNextMove();
				if (done){
					myGlutDisplay();
					t--;
				}
			}
			if (Juego_Terminado()){
				do_one_run_btn->disable();
				do_one_step_btn->disable();
				display_btn->disable();
				remote_map_btn->enable();
				display_btn->disable();
			}

			break;
		case NEWMAP_OK_BTN_ID:
			strcpy(field_ip_server,ip_server);
			strcpy(field_nickname,mi_nick);
			newGame();
				//select_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case NEWMAP_CANCEL_BTN_ID:
			remote_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case SELECTMAP_SELECT_ID:
			strcpy(field_ip_server,ip_server);
			strcpy(field_nickname,mi_nick);
			strcpy(file_name_input,map_list->get_file());
			strcpy(life_time_input,life_time_temp);
			life_time = atoi(life_time_temp);	
			newGame();
				//select_map_btn->enable();
			select_map_btn->enable();
			select_map_glui->close();
			break;
		case SELECTMAP_CANCEL_BTN_ID:
			//select_map_btn->enable();
			strcpy(field_ip_server,ip_server);
			strcpy(field_nickname,mi_nick);

			Save_Conf();
			myGlutDisplay();
			glutPostRedisplay();
			glutSetWindow(main_window);
			glutPostRedisplay();
			select_map_btn->enable();
			select_map_glui->close();
			partida_remota=false;
			modo=4;
			int correcto;
			if (modo==4){ // Vs Ninja
				char mensaje[1024];

				correcto = Cliente.Establecer_Conexion(field_ip_ninja, "5126");
				if (correcto){
					cout << "Empezamos las partida..." << endl;
					cout << "Enviando mi nick al servidor..." << mi_nick << endl;
					Cliente.Enviar_Msg(mi_nick);
					Cliente.Enviar_Msg("PlYR");
					partida_remota=true;
					newGame();
				}
				else {
					error_glui=GLUI_Master.create_glui("ERROR",0,100,300);
					new GLUI_StaticText(error_glui,"El Servido no esta activo");

					new GLUI_Button(error_glui,"Ok",ERROR_OK_BTN_ID,control_cb);
					remote_map_btn->enable();
					partida_remota=false;
					modo=0;
				}

			}
			break;
		case DO_ONE_STEP_BTN_ID:
			doNextMove();
			break;
		case DO_ONE_RUN_BTN_ID:
			doOneRun();
			break;
		case ERROR_OK_BTN_ID:
			error_glui->close();
			break;
		case REINICIAR_BTN_ID:
			newGame();
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

bool GUI::doNextMove(){
	char aux;
	bool hecho=false;
	switch (modo){
        case 0: //Normal
        	if (!Juego_Terminado()){
				action = agent->Think();
				env->AcceptAction(static_cast<Agent::ActionType>(action));
				UltimaAccion = static_cast<Agent::ActionType>(action);


				agent->Perceive(*env);
				agent->ActualizarInformacion(env);
				current_time++;



				if (env->Reiniciado())
					secuencia=15;
	        	else if (action>=0 and action<=2)
	        		secuencia = action;
		        myGlutDisplay();
		        display();
		    }
			else {
				do_one_run_btn->disable();
				display_btn->disable();
				remote_map_btn->enable();
			}

	        hecho=true;

        break;
        case 4: //Jugando en Red

        	if (!esperando_server){

		        action = agent->Think();
				//env->ActualizarMatrizUsuario(agent->mapa_entorno_);

		        cout << "Accion Enviada: " << action << endl;

		        aux = static_cast<char>(action);
		                      // Enviar Accion
		        cout << "Envio la accion al servidor" << endl;
		        Cliente.Enviar_Accion(aux);

		        //env->AcceptAction(static_cast<Agent::ActionType>(action));
				UltimaAccion = static_cast<Agent::ActionType>(action);
		        esperando_client=false;
		        esperando_server=true;
	        	hecho=true;

	        	if (action>=0 and action<=2)
	        		secuencia = action;

		        myGlutDisplay();
		        display();
		        myGlutTimer(0);
		    }

	        glutTimerFunc(1000,myGlutTimer,10);

        break;

    }
    return hecho;
}

void GUI::doOneStep(){
	if (!Juego_Terminado()){

		action = agent->Think();
		env->AcceptAction(static_cast<Agent::ActionType>(action));
		UltimaAccion = static_cast<Agent::ActionType>(action);

		agent->Perceive(*env);
		agent->ActualizarInformacion(env);
		current_time++;


	}
	else {
		do_one_run_btn->disable();
		display_btn->disable();
		remote_map_btn->enable();
	}

}

void GUI::do_move(Agent::ActionType accion){
	if (modo==0){
		if (!Juego_Terminado()){
			env->AcceptAction(accion);
			UltimaAccion = accion;
			agent->Perceive(*env);
			agent->ActualizarInformacion(env);
			current_time++;


			if (env->Reiniciado()){
				secuencia=15;
				while (secuencia!=-1)
					display();
			}
	    	else if (action>=0 and action<=2)
	    		secuencia = action;
	        myGlutDisplay();
	        display();
		}
		else {
			do_one_run_btn->disable();
			display_btn->disable();
			remote_map_btn->enable();
			do_one_step_btn->disable();

		}
	}
	else {
    	if (!esperando_server){

    		int action = static_cast<int>(accion);

	        cout << "Accion Enviada: " << action << endl;

	        char aux = static_cast<char>(action);
	                      // Enviar Accion
	        cout << "Envio la accion al servidor" << endl;
	        Cliente.Enviar_Accion(aux);

			UltimaAccion = accion;
	        esperando_client=false;
	        esperando_server=true;
        	//hecho=true;

        	if (action>=0 and action<=2)
        		secuencia = action;

	        myGlutDisplay();
	        display();
	        myGlutTimer(0);
	    }

        glutTimerFunc(1000,myGlutTimer,10);


	}
}



void GUI::doOneRun(){
	do_one_run_btn->disable();
	display_btn->disable();
	do_one_step_btn->disable();
	remote_map_btn->enable();

	while(!Juego_Terminado()){
		doNextMove();
		myGlutDisplay();
	    //Sleep(100);
	    //glutMainLoop();

	}

}



void GUI::newGame(){
	ifstream fin;

	if (env!=0)
		delete env;

	if (agent!=0)
		delete agent;

	switch(modo){
	    case 0: // Modo normal
	
		fin.open(file_name_input);
		if(fin){
			current_time=0;
			puntuacion_mapa=0;
			env=new Environment(fin);
			agent=new Agent();
			finDeJuegoProcesado=false;
			
			env->randomPosPlayer();
			agent->Perceive(*env);
			agent->ActualizarInformacion(env);
			current_time++;


			do_one_step_btn->enable();
			display_btn->enable();
			do_one_run_btn->enable();

			myGlutDisplay();
			glutPostRedisplay();
		}
		else{
			char temp[1000]={"map/"};
			strcat(temp,file_name_input);
			fin.open(temp);
			if(fin){
				current_time=0;
				puntuacion_mapa=0;
				env=new Environment(fin);
				agent=new Agent();

				finDeJuegoProcesado=false;
				env->randomPosPlayer();
				agent->Perceive(*env);
				agent->ActualizarInformacion(env);
				current_time++;
			
				
				do_one_step_btn->enable();
				display_btn->enable();
				do_one_run_btn->enable();
				
			myGlutDisplay();
			glutPostRedisplay();
			}
			else{
				do_one_step_btn->disable();
				display_btn->disable();
				do_one_run_btn->disable();
				error_glui=GLUI_Master.create_glui("Error",0,100,100);
					char error_message[1000]="El fichero ";
					strcat(error_message,file_name_input);
					strcat(error_message," no se puede abrir");
					new GLUI_StaticText(error_glui,error_message);
					new_map_ok_btn=new GLUI_Button(error_glui,"OK",ERROR_OK_BTN_ID,control_cb);
			}
		}

        break;
        case 4: // Vs Ninja
	        cout << "Esperando que empiece la partida" << endl;
			finDeJuegoProcesado=false;

			error_glui=GLUI_Master.create_glui("Espera al Servidor",0,300,100);
			espera_txt=new GLUI_StaticText(error_glui,"Se paciente....");
			espera_txt->set_w(200);


	        glutTimerFunc(100,myGlutTimer2,10);

    }


}



void GUI::myGlutTimer2(int x){
	char buffer[1000];
	Cliente.Recibir_Msg(buffer);
	if (buffer[0]!='%')
	        glutTimerFunc(100,myGlutTimer2,10);
	else {
	
		error_glui->close();

	    cout << "Partida iniciada" << endl;
	    partida_remota=true;

	    env=new Environment();
	    agent=new Agent();

	    do_one_step_btn->disable();
	    display_btn->disable();
	    do_one_run_btn->disable();

	    esperando_client=false;
	    esperando_server=true;

	    myGlutDisplay();
	    display();

	    glutTimerFunc(1000,myGlutTimer,10);
	}
}


void GUI::display()
{
	glutSetWindow(punto_vista);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int tx,ty,tw,th;
	GLUI_Master.get_viewport_area(&tx,&ty,&tw,&th);
	//cout << "Ancho: "<< tw << "  Alto: "<< th << endl;

 
 // Almacenar en la matriz lo que ve el agente
    int fila, columna, orienta;

    if (agent!=NULL and env!=NULL){
	    agent->GetCoord(fila,columna,orienta);

		//cout << "\tEl agente esta operativo, paso a pintar\n";
	    //cout << "fila: " << fila << "  columna: " << columna << " o: "<< orienta << endl;
	    switch(orienta){
	    	case 0: fila--;
	    			for (int i=0; i<9; i++)
	    				for (int j=0; j<5; j++)
	    					if (fila-i+1<0 or j-2+columna<0 or j-2+columna>199){
	    						vision[i][j]='?';
	    						objetos[i][j]='?';
	    					}
	    					else{
	    						vision[i][j]=agent->mapa_entorno_[fila-i+1][j-2+columna];
	    						objetos[i][j]=agent->mapa_objetos_[fila-i+1][j-2+columna];
	    					}
	    			break;

	    	case 1: columna++;
	    			for (int j=0; j<9; j++)
	    				for (int i=0; i<5; i++)
	    					if (columna+j-1>199 or i-2+fila-1<0 or i-2+fila-1>199){
	    						vision[j][i]='?';
	    						objetos[j][i]='?';
	    					}
	    					else{
	    						vision[j][i]=agent->mapa_entorno_[i-2+fila][columna+j-1];
	    						objetos[j][i]=agent->mapa_objetos_[i-2+fila][columna+j-1];
	    					}
	    			break;
	    	case 2: fila++;
	    			for (int i=0; i<9; i++)
	    				for (int j=0; j<5; j++)
	    					if (fila+i-1>199 or 2-j+columna<0 or 2-j+columna>199){
	    						vision[i][j]='?';
	    						objetos[i][j]='?';
	    					}
	    					else{
	    						vision[i][j]=agent->mapa_entorno_[fila+i-1][2-j+columna];
	    						objetos[i][j]=agent->mapa_objetos_[fila+i-1][2-j+columna];
	    					}
	    			break;
	    	case 3: columna--;
	    			for (int j=0; j<9; j++)
	    				for (int i=0; i<5; i++)
	    					if (columna-j+1<0 or 2-i+fila+1<0 or 2-i+fila+1>199){
	    						vision[j][i]='?';
	    						objetos[j][i]='?';
	    					}
	    					else{
	    						vision[j][i]=agent->mapa_entorno_[2-i+fila][columna-j+1];
	    						objetos[j][i]=agent->mapa_objetos_[2-i+fila][columna-j+1];
	    					}
	    			break;
	    }


	// Pintar la escena

	    glLoadIdentity();
	 
	    glTranslatef(0, -0.4, -10.0);
	 
	 switch(secuencia){
	 	case -1:anguloCuboY=0;
	 			anguloCuboX=0.2;
	 			anguloCuboZ=0;
				anguloPespectiva =15.0f;
				reshape(ancho, alto);
	 			break;
	 	case 0: 
				anguloPespectiva =20.0f; 
				anguloCuboY=0;	
	 			secuencia=10;
				reshape(ancho, alto);
	 			break;
	 	case 1: anguloCuboY=3;
	 			secuencia=3;
	 			break;
	 	case 3: anguloCuboY=6;
	 			secuencia=4;
	 			break;
	 	case 4: anguloCuboY=9;
	 			secuencia=-1;
	 			break;
	 	case 2: anguloCuboY=-3;
	 			secuencia=5;
	 			break;
	 	case 5: anguloCuboY=-6;
	 			secuencia=6;
	 			break;
	 	case 6: anguloCuboY=-9;
	 			secuencia=-1;
	 			break;
	 	case 10: 
				anguloPespectiva =18.0f; 	
				anguloCuboY=0;	
	 			secuencia=11;
				reshape(ancho, alto);
	 			break;
	 	case 11: 
				anguloPespectiva =16.0f; 	
				anguloCuboY=0;	
	 			secuencia=-1;
				reshape(ancho, alto);
	 			break;
	 	case 15:
				anguloPespectiva =16.0f; 	
				anguloCuboX=30;	
	 			secuencia=16;
				reshape(ancho, alto);
	 			break;
	 	case 16:
				anguloPespectiva =16.0f; 	
				anguloCuboX=20;	
	 			secuencia=17;
				reshape(ancho, alto);
	 			break;
	 			
	 	case 17:
				anguloPespectiva =16.0f; 	
				anguloCuboX=10;	
	 			secuencia=18;
				reshape(ancho, alto);
	 			break;
	 	case 18:
				anguloPespectiva =16.0f; 	
				anguloCuboX=5;	
	 			secuencia=-1;
				reshape(ancho, alto);
	 			break;


	 }
	    glRotatef(anguloCuboX, 1.0f, 0.0f, 0.0f);
	    glRotatef(anguloCuboY, 0.0f, 1.0f, 0.0f);
	    glRotatef(anguloCuboZ, 0.0f, 0.0f, 1.0f);

		// gluLookAt(0,0.5,3,0,0.1,0,0,4,0);

	    glColor3f(0,1,1);

	    // Terreno

	    for (int i=0; i<=8; i++){
	        for (int j=-2; j<=2; j++){
	            switch(vision[8-i][j+2]){
	              case 'P': ponerMaterial(0.0,0.0,0.0);
	                        drawCube(j,0, i,1,0.1);
	                    break;
	              case 'A': ponerMaterial(0.0,0.0,1.0);
	                        drawCube(j,0, i,1,0.0);
	                    break;
	              case 'S': ponerMaterial(0.6,0.6,0.6);
	                        drawCube(j,0, i,1,0.1);
	                    break;
	              case 'T': ponerMaterial(0.3,0.25,0.2);
	                        drawCube(j,0, i,1,0.1);
	                    break;
	              case 'K': ponerMaterial(1.0,1.0,0.0);
	                        drawCube(j,0, i,1,0.2);
	                    break;
	              case 'Z': ponerMaterial(0.3,0.3,0.3);
	                        drawCube(j,0, i,1,0.1);
	                    break;
	              case 'B': 
	                        drawTree(j,0, i,1,1);
	                    break;
	              case 'M': //ponerMaterial(0.6,0.0,0.0,1);
	                        //drawCube(j,0, i,1,1);
	                        drawWall(j,0,i,1,1);
	                    break;

	              case 'D': ponerMaterial(1.0,1.0,1.0);
	                        drawCube(j,0, i,1,1);
	              case '?': ponerMaterial(1.0,1.0,1.0);
	                        drawCube(j,0, i,1,0.1);
	                    break;

	            }

	         }
	    }


	    // Objetos sobre el terreno
	    for (int i=0; i<=8; i++){
	        for (int j=-2; j<=2; j++){
	            switch(objetos[8-i][j+2]){
	            			// Objetos que es posible recoger
				  case '0': //Banador
				  case '1': //Piedra
				  case '2': //Espada
				  case '3': //Rosa
				  case '4': //Oro
				  case '5': //Oscar
				  case '6': //Zapatillas
				  case '7': //Pala
				  case '8': //Manzana
				  case '9': //Algoritmo
				  		    drawGenericObject(j,0,i);
				  			break;
	              case 'a': // Otros jugadores en distintas orientaciones
	              			drawPlayer(j,0,i,'0');  //De frente
	              			break;
	              case 'b':
	              			drawPlayer(j,0,i,'3');	// Para la izquierda 
	              			break;
	              case 'c':
	              			drawPlayer(j,0,i,'1'); // De espaldas
	              			break; 
	              case 'd':
	              			drawPlayer(j,0,i,'2');  // Para la derecha
	              			break;
				  case 'e': // Aldeano de frente
				  			drawPersonaje(j,0,i,true,0,1,4, "fotos/aldeano.jpg");
				  			break;
				  case 'f': // Aldeano de espaldas
				  			drawPersonaje(j,0,i,true,0,1,4, "fotos/aldenao.jpg");
				  			break;
				  case 'g': // Princesa Capichosa
				  			drawPersonaje(j,0,i,true,2,5,5, "fotos/princesa.jpg");
				  			break;
				  case 'h': // Princesa Caprichosa
				  			drawPersonaje(j,0,i,true,2,5,5, "fotos/princesa.jpg");
				  			break;
				  case 'i': // Leonardo Di Caprio
				  			drawPersonaje(j,0,i,true,2,4,1, "fotos/leonardo.jpg");
				  			break;
				  case 'j': // Leonardo Di Caprio
				  			drawPersonaje(j,0,i,true,2,4,1, "fotos/leonardo.jpg");
				  			break;
				  case 'k': // Princeso Valeroso
				  			drawPersonaje(j,0,i,true,5,1,5, "fotos/princeso.jpg");
				  			break;
				  case 'l': // Princeso Valeroso
				  			drawPersonaje(j,0,i,true,5,1,5, "fotos/princeso.jpg");
				  			break;
				  case 'm': // Bruja Malvada
				  			drawPersonaje(j,0,i,true,3,0,0, "fotos/bruja.jpg");
				  			break;
				  case 'n': // Bruja Malvada
				  			drawPersonaje(j,0,i,true,3,0,0, "fotos/bruja.jpg");
				  			break;
				  case 'o': // Profe IA
				  			drawPersonaje(j,0,i,true,4,2,4, "fotos/profeia.jpg");
				  			break;
				  case 'p': // Profe IA
				  			drawPersonaje(j,0,i,true,4,2,4, "fotos/profeia.jpg");
				  			break;
				  case 'q': // Oso
				  			drawDog(j,0,i);
				  			break;
	            }

	         }
	    }

	 
	     // drawCube(-2,0,1,0);
	     // drawCube(-1,0,1,-1);
	     // drawCube(0,0,1,-2);
	     // drawCube(1,0,1,-3);
	     // drawCube(2,0,1,-4);

	     // drawCube(0,-1,1,0);

	    //drawQuesitoDeEspaldas(0, 0, 8);

		}
		else{
			for (int i=0; i<9; i++)
				for (int j=0; j<5; j++)
					vision[i][j]='?';
		}

	glEnable(GL_LIGHTING);
    Luz luz(GL_LIGHT0);
    luz.setPosicion(_vertex3<float>(0,3.0,9.0));
    luz.encender();
    luz.draw();
 
    glLoadIdentity();
 
    // glTranslatef(0.0f, 0.0f, -5.0f);
    // glRotatef(anguloEsfera, 0.0f, 1.0f, 0.0f);
    // glTranslatef(3.0f, 0.0f, 0.0f);
 
    // glColor3f(1.0f, 1.0f, 1.0f);
    // glutWireSphere(0.5f, 8, 8);
  
 	glFlush();
	glutPostRedisplay();
	glutSwapBuffers();

	glutSetWindow(main_window);

	if (secuencia!=-1){
		//usleep(1000);
	}

    //anguloCuboX+=0.1f;
    //anguloCuboY+=0.1f;
    // anguloEsfera+=0.2f;

}

void GUI::idle()
{
    display();
    myGlutDisplay();
}


void GUI::reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(anguloPespectiva, (GLfloat)width/(GLfloat)height, distanciaObjetoCercano, distanciaObjetoLejano);

    glMatrixMode(GL_MODELVIEW);
 
    ancho = width;
    alto = height;
}


void GUI::keyboard(unsigned char key, int x, int y)
{
	if (env!=0 and agent!=0){
		//cout << "tecla: "<< key << endl;
	    switch(key){
	    	case 'E':
	    	case 'e':
	    		agent->FixLastAction(Agent::actFORWARD);
	    		do_move(Agent::actFORWARD);
	    		break;
	    	case 'S':
	    	case 's':
	    		agent->FixLastAction(Agent::actTURN_L);
	    		do_move(Agent::actTURN_L);
	    		break;
	    	case 'F':
	    	case 'f':
	    		agent->FixLastAction(Agent::actTURN_R);
	    		do_move(Agent::actTURN_R);
	    		break;
	    	case 'H':
	    	case 'h':
	    		agent->FixLastAction(Agent::actPICKUP);
	    		do_move(Agent::actPICKUP);
	    		break;
	    	case 'N':
	    	case 'n':
	    		agent->FixLastAction(Agent::actPUTDOWN);
	    		do_move(Agent::actPUTDOWN);
	    		break;
	    	case 'J':
	    	case 'j':
	    		agent->FixLastAction(Agent::actPUSH);
	    		do_move(Agent::actPUSH);
	    		break;
	    	case 'M':
	    	case 'm':
	    		agent->FixLastAction(Agent::actPOP);
	    		do_move(Agent::actPOP);
	    		break;
	    	case 'D':
	    	case 'd':
	    		agent->FixLastAction(Agent::actGIVE);
	    		do_move(Agent::actGIVE);
	    		break;
	    	case ' ':
	    		agent->FixLastAction(Agent::actTHROW);
	    		do_move(Agent::actTHROW);
	    		break;
	    }
	}
}

