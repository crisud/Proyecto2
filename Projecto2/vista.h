#pragma once
#include "empresaDots.h"
#include"ProcesaCompuesto.h"
#include"CampoNuevePuntos.h"
#include"CampoSeisPuntos.h"
#include"CampoQuincePuntos.h"
#include"CampoResultante.h"

class vista
{
public:
	static int menuGeneral();
	// Jugar
	static int menuJuego();
	static bool iniciarSesion(empresaDots*);
	static int modoDeJuego();
	static ProcesaCompuesto* crearCampo();
	//Empresa - Jugador
	static int menuEmpresa();
	static void suscribirJugador(empresaDots*);
	static void codigoActual(empresaDots*);
	static void jugadoresSuscritos(empresaDots*);
	static void simulacionCambioDia(empresaDots*);

};
