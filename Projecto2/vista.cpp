#include "vista.h"
#include "utiles.h"
#include "arbitro.h"
#include <Windows.h>
int vista::menuGeneral()
{
	int opcion;

	system("cls");
	imprimeCadena("[1] Juego");
	imprimeCadena("[2] Menu empresa");
	imprimeCadena("[3] Salir");
	imprimSinEndl("Opcion: ");
	opcion = leerEntero();
	system("cls");
	return opcion;
}

int vista::menuJuego()
{
	int opcion;

	system("cls");
	imprimeCadena("[1] Jugar");
	imprimeCadena("[2] Ver repeticion");
	imprimeCadena("[3] Volver");
	imprimSinEndl("Opcion: ");
	opcion = leerEntero();
	system("cls");
	return opcion;
}
bool vista::iniciarSesion(empresaDots* empresa) // false si el codigo o id son incorrectos   - Aqui se entra si elige jugar
{
	string id, codigo;

	system("cls");
	imprimeCadena("Digite su cedula: "); id = leerCadena2();
	imprimeCadena("Digite el codigo del dia de hoy: "); codigo = leerCadena2();
	
	if ((codigo == empresa->getCodigo()) && (empresa->estaJugador(id)))
			return true;
		else
			return false;
}
int vista::modoDeJuego() // si inicio sesion true
{
	int opcion;

	system("cls");
	imprimeCadena("[1] Jugador vs Jugador"); // Metodo aparte
	imprimeCadena("[2] Jugador vs Maquina"); //Metodo aparte
	imprimSinEndl("Opcion: "); opcion = leerEntero();
	return opcion;
}
ProcesaCompuesto* vista::crearCampo()
{
	int opcion = 0, cont=0;
	CampoResultante* composite = new CampoResultante;

	while (opcion != 4)
	{
		system("cls");
		imprimeCadena("Seleccione el tipo de campo (El maximo de columnas por tema espacio de pantalla es 30)");
		imprimeCadena("[1] Campo 6 puntos");
		imprimeCadena("[2] Campo 9 puntos");
		imprimeCadena("[3] Campo 15 puntos");
		imprimeCadena("[4] Continuar");
		imprimSinEndl("Opcion: ");
		opcion = leerEntero();

		switch (opcion)
		{
			case 1: 
			{
				cont +=2;
				if(cont <= 30)
					composite->ingresaCampo(new CampoSeisPuntos);
				else
				{
					string aux = enteroAstring(cont);
					imprimeCadena("Espacio de pantalla excedido, " + aux + " columnas");
					return nullptr;
				}
			}break;
			case 2:
			{
				cont += 3;
				if (cont <= 30)
					composite->ingresaCampo(new CampoNuevePuntos);
				else
				{
					string aux = enteroAstring(cont);
					imprimeCadena("Espacio de pantalla excedido, " + aux + " columnas");
					return nullptr; 
				}
			}break;
			case 3:
			{
				cont += 3;
				if (cont <= 30)
					composite->ingresaCampo(new CampoQuincePuntos);
				else
				{
					string aux = enteroAstring(cont);
					imprimeCadena("Espacio de pantalla excedido, " + aux + " columnas");
					return nullptr;
				}
			}break;
		}
		
	}//retorne excepcion si es mas grande de lo debido 
	if (composite->getCan() > 0)
		return new ProcesaCompuesto(composite->retornaContenedor());
	else
	{
		imprimeCadena("No se agrego ningun campo");
		return nullptr;
	}
}
void vista::turnoJugador(char nom, ContenedorM* matriz,Partida* parti)
{
	int x1, y1, x2, y2;
	string aux = charAstring(nom);
	imprimeCadena(" -> Ingrese lo solicitado \n (En caso de querer terminar la partida ingrese el numero 911 en fila...)\n");
	imprimeCadena("Jugador "+ aux);
	imprimSinEndl("Fila: "); x1 = leerEntero();
	if (x1 == 911) { throw 911; }
	imprimSinEndl("Columna: "); y1 = leerEntero();
	imprimeCadena("Se conecta con: ");
	imprimSinEndl("Fila: "); x2 = leerEntero();
	imprimSinEndl("Columna: "); y2 = leerEntero();

	while (!arbitro::dirrecionJugada(x1, y1, x2, y2,matriz))
	{
		imprimeCadena("Jugada Invalida, vuelva a intentarlo");
		imprimSinEndl("Fila: "); x1 = leerEntero();
		imprimSinEndl("Columna: "); y1 = leerEntero();
		imprimeCadena("Se conecta con: ");
		imprimSinEndl("Fila: "); x2 = leerEntero();
		imprimSinEndl("Columna: "); y2 = leerEntero();
	}
	Jugada* jugadita = new Jugada(x1, y1, x2, y2, nom);
	parti->getJugadas()->insertarFinal(jugadita);
	matriz->toString(nom);
}
Partida* vista::partidaJugadorJugador()
{
	Partida* parti = new Partida; // se crea la partida que despues se agregra a juego(controladora) en su lista de partidas
	ProcesaCompuesto* campo = nullptr;
	while (campo == nullptr)
	{
		campo = crearCampo();
		Sleep(1000);
	}
	parti->setProCompu(campo); // el procesaCompuesto de la partida (su campo)
	ListaJugada* lista = new ListaJugada; // el registro de las jugadas de las partidas
	parti->setJugadas(lista);
	
	char jugadorA = 'A', jugadorB = 'B';
	int cont = 1;
	int puntosA = 0, puntosB = 0; // puntos de jugador A y B respectivamente
	ContenedorM* matriz = parti->getProCompu()->getMatriz(); // para manejar la matriz de forma individual

	try {
		while (!matriz->estaLlena())
		{
			system("cls");

			if (cont % 2 != 0)
			{
				puntosA = matriz->cuentaPuntos(jugadorA, matriz->toString(' '));
				imprimeCadena(matriz->toString(jugadorA));
				turnoJugador(jugadorA, matriz, parti);
				while (!matriz->estaLlena() && puntosA < matriz->cuentaPuntos(jugadorA, matriz->toString(' ')))
				{
					system("cls");
					puntosA = matriz->cuentaPuntos(jugadorA, matriz->toString(' '));
					imprimeCadena(matriz->toString('A'));
					turnoJugador(jugadorA, matriz, parti);
				}
				if (!matriz->estaLlena())
					cont++;
			}

			if (cont % 2 == 0)
			{
				system("cls");
				puntosB = matriz->cuentaPuntos(jugadorB, matriz->toString(' '));
				imprimeCadena(matriz->toString(jugadorB));
				turnoJugador(jugadorB, matriz, parti);
				while (!matriz->estaLlena() && puntosB < matriz->cuentaPuntos(jugadorB, matriz->toString(' ')))
				{
					system("cls");
					puntosB = matriz->cuentaPuntos(jugadorB, matriz->toString(' '));
					imprimeCadena(matriz->toString('B'));
					turnoJugador(jugadorB, matriz, parti);
				}
				if (!matriz->estaLlena())
					cont++;
			}
		}
	}
	catch (int)
	{
		system("cls");
		imprimeCadena(matriz->toString(' '));
		imprimeCadena("\nPartida terminada...");
		string aux1 = enteroAstring(puntosA);
		string aux2 = enteroAstring(puntosB);
		if (puntosA > puntosB)
			if (puntosA == 1)
				imprimeCadena("Ganador Jugador A con " + aux1 + " punto");
			else
				imprimeCadena("Ganador Jugador A con " + aux1 + " puntos");
		else
			if (puntosA == puntosB)
				imprimeCadena("El juego termino en empate, con ambos jugadores con " + aux1 + " puntos");
			else
				if (puntosB > puntosA)
					if (puntosB == 1)
						imprimeCadena("Ganador Jugador B con " + aux2 + " punto");
					else
						imprimeCadena("Ganador Jugador B con " + aux2 + " puntos");
		imprimeCadena("<Enter>");
		esperandoEnter();
	}
	system("cls");
	imprimeCadena(matriz->toString(' '));
	return parti;
}
maquina* vista::crearMaquina()
{
	int opcion;
	Estrategia* estra = nullptr;
	maquina* maqui = new maquina;
	imprimeCadena("Selecciona la estrategia que seguira la maquina");
	imprimeCadena("[1] Aleatoria");
	imprimeCadena("[2] Islas");
	imprimeCadena("[3] Central");
	imprimeCadena("[4] Cercana");
	imprimeCadena("[5] Periferica");
	imprimSinEndl("Opcion: "); opcion = leerEntero();

	switch (opcion)
	{
		case 1: estra = new EstraAleatorio; break;
		case 2: estra = new EstraIslas; break;
		case 3: estra = new EstraCentral; break;
		case 4: estra = new EstraCercano; break;
		case 5: estra = new EstraPeriferico; break;
	}
	if (estra != nullptr)
	{
		maqui->setEstrategia(estra);
		return maqui;
	}
	else
		return nullptr;
}
void vista::turnoMaquina(ContenedorM* matriz, Partida* parti, maquina* maqui)
{
	maqui->aplicarEstrategia(matriz, parti);
}
Partida* vista::partidaJugadorMaquina()
{
	Partida* parti = new Partida; // se crea la partida que despues se agregra a juego(controladora) en su lista de partidas
	ProcesaCompuesto* campo = nullptr;
	while (campo == nullptr)
	{
		campo = crearCampo();
		Sleep(1000);
	}
	parti->setProCompu(campo); // el procesaCompuesto de la partida (su campo)
	ListaJugada* lista = new ListaJugada; // el registro de las jugadas de las partidas
	parti->setJugadas(lista);
	ContenedorM* matriz = parti->getProCompu()->getMatriz();
	system("cls");

	maquina* maqui = nullptr;
	while (maqui == nullptr)
	{
		maqui = crearMaquina();
	}
	int puntosM = 0;

	while (!matriz->estaLlena())
	{
		system("cls");
		turnoMaquina(matriz, parti, maqui);
		imprimeCadena(matriz->toString('M'));
		puntosM = matriz->cuentaPuntos('M',matriz->toString('M'));
		cout << "Puntos: " << puntosM << endl;
		system("pause");
	}

	return parti;
}

int vista::menuEmpresa()
{
	int opcion;

	system("cls");
	imprimeCadena("[1] Suscribir Jugador");
	imprimeCadena("[2] Codigo actual");
	imprimeCadena("[3] Jugadores suscritos");
	imprimeCadena("[4] Simular cambio de dia");
	imprimeCadena("[5] Salir");
	imprimSinEndl("Opcion: ");
	opcion = leerEntero();
	system("cls");
	return opcion;
}
void vista::suscribirJugador(empresaDots* empresa)
{
	string id, nom;

	imprimeCadena("Digite la cedula del jugador: "); id = leerCadena2();
	if (!empresa->estaJugador(id))
	{
		imprimeCadena("Digite el nombre del jugador: "); nom = leerCadena2();
		jugador* jugaAux = new jugador(nom, id, empresa);
		system("cls");
		imprimeCadena("\tJugador suscrito correctamente\n\n");
	}
	else
	{
		system("cls");
		imprimeCadena("\tEl jugador ya esta suscrito a la empresa\n");
	}
	system("pause");
}
void vista::codigoActual(empresaDots* empresa)
{
	string id;

	imprimeCadena("Digite la cedula del jugador: ");  id = leerCadena2();
	if (empresa->estaJugador(id))
	{
		jugador* aux = empresa->devuelveJugador(id);
		imprimeCadena( "Codigo del dia: " + aux->mostrarCodigo() + "\n\n");
	}
	else
	{
		system("cls");
		imprimeCadena( "\tJugador no esta suscrito a la empresa\n");
	}
	system("pause");
}
void vista::jugadoresSuscritos(empresaDots* empresa)
{
	imprimeCadena(empresa->mostrarJugadores() + "\n\n");
	system("pause");
}
void vista::simulacionCambioDia(empresaDots* empresa)
{
	empresa->cambioDeDia();
	imprimeCadena( "El codigo ha cambiado\nLos jugadores suscritos han sido borrados para comenzar el dia\n\n");
	system("pause");
}
