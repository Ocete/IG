// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------

class MallaRevol : public MallaInd
{
protected:
    // numero de perfiles
    int nper;
    // numero de vertices por perfil
    int nvp;

    // Método que crea las tablas vertices y triangulos
    void crearMallaRevol ( const std::vector<Tupla3f> & perfil_original,
      const int nperfiles, const bool crear_tapas,
      const bool cerrar_malla ) ;

public:
      // crea una malla de revolucion
      MallaRevol( const std::string & nombre_arch,
                  const int nperfiles,
                  const bool crear_tapas,
                  const bool cerrar_malla ) ;

      // Constructor vacio
      MallaRevol () {}
} ;

// ---------------------------------------------------------------------

class Cilindro : public MallaRevol {
public:
  Cilindro( const int num_verts_per,
  const int nperfiles,
  const bool crear_tapas,
  const bool cerrar_malla,
  const float alt,
  const float radio_base);
} ;

// ---------------------------------------------------------------------
class Cono : public MallaRevol {
public:
  Cono (const int num_verts_per,
        const int nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const float alt,
        const float radio);
} ;

// ---------------------------------------------------------------------

class Esfera : public MallaRevol {
public:
Esfera ( const int num_verts_per,
        const int nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const float radio);
} ;

// ---------------------------------------------------------------------

class Toroide : public MallaRevol {
public:
Toroide ( const int num_verts_per,
        const int nperfiles,
        const bool cerrar_malla,
        const float desplazamiento,
        const float radio);
} ;


#endif
