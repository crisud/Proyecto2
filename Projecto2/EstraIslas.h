#ifndef ESTRAISLAS_H
#define ESTRAISLAS_H
#include"Estrategia.h"

class EstraIslas : public Estrategia {
public:
	EstraIslas();
	virtual ~EstraIslas();
	virtual bool aplicaEstra(ContenedorM*,Partida*);
	virtual string getNombreEstra();
};

#endif // !ESTRAISLAS_H
