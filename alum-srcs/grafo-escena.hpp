// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************


#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <obj-gra.hpp>
//#include <materiales.hpp>

#include "matrices-tr.hpp"
#include "materiales.hpp"
#include "Objeto3D.hpp"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {  Objeto3D * objeto ;  // ptr. a un objeto (no propietario)
      Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
      Material * material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
   EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D {
protected:
     std::vector<EntradaNGE> entradas ;
     bool color_fijado;
public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL( ContextoVis & cv ) ;
   virtual void fijarColorNodo( const Tupla3f & nuevo_color ) ;

   // calcular y obtener la caja englobante
   //virtual CajaEngf cajaEnglobante() ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE & entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material * pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   Matriz4f * leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const Matriz4f & mmodelado,
                    Objeto3D ** objeto, Tupla3f & centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;

   void asignarIdentificadores( int &nuevo_ident );

} ;

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos que tienen además un vector de
// parámetros

class NodoGrafoEscenaParam : public NodoGrafoEscena
{
   protected:
      std::vector<Parametro> parametros ; // vector de parámetros
   public:
      // devuelve el número de parámetros
      int numParametros();

      // devuelve un puntero al i-ésimo parámetro (i < numParametros())
      Parametro * leerPtrParametro( unsigned i ) ;

      // actualiza el objeto para ir al siguiente cuadro,
      // se usa cuando están activadas las animaciones, una vez antes de cada frame
      void siguienteCuadro();

      // Resetea todos los parámetros
      void reset();
} ;

// *********************************************************************

// clase para la práctica 3
class C : public NodoGrafoEscenaParam {
  std::string nombre;
public:
  C(bool atraccion);
  C(); // Ojo, este constructor no hace NADA
} ;

// *********************************************************************
// clase para modelizar un Asiento

class Asiento : public NodoGrafoEscena {
public:
  Asiento();
} ;

// *********************************************************************
// clase para modelizar un Asiento Delantero

class AsientoDelantero : public NodoGrafoEscena {
public:
  AsientoDelantero(float d);
} ;

// *********************************************************************
// clase para modelizar un Asiento Trasero

class AsientoTrasero : public NodoGrafoEscena {
public:
  AsientoTrasero(float d);
} ;

// *********************************************************************
// clase para modelizar un Cilindro girado, posteriormente se le añaden los asientos.

class CilindroAsientos : public NodoGrafoEscena {
public:
  CilindroAsientos();
} ;

// *********************************************************************
// clase para modelizar un Brazo mecánico con asientos.

class FinalBrazoMecanico : public NodoGrafoEscena {
public:
  FinalBrazoMecanico(std::vector<Parametro> *v, float d, int num);
} ;

// *********************************************************************
// clase para modelizar un Brazo mecánico con asientos.

class BrazoMecanico : public NodoGrafoEscena {
public:
  BrazoMecanico(std::vector<Parametro> *v, float d, int num);
} ;

// *********************************************************************
// clase para modelizar un base de brazos mecanicos

class BaseBrazos : public NodoGrafoEscena {
public:
  BaseBrazos(std::vector<Parametro> *v);
} ;

// *********************************************************************
// clase para modelizar una atraccion

class Atraccion : public NodoGrafoEscena {
public:
  Atraccion(std::vector<Parametro> *v);
} ;

// *********************************************************************

class Ojo : public NodoGrafoEscena {
public:
  Ojo(float ang);
} ;

// *********************************************************************

class Pata : public NodoGrafoEscena {
public:
  Pata(std::vector<Parametro> *v, int num);
} ;

// *********************************************************************

class ParDePatas : public NodoGrafoEscena {
public:
  ParDePatas(std::vector<Parametro> *v, int num);
} ;

// *********************************************************************

class Cuerpo : public NodoGrafoEscena {
public:
  Cuerpo(std::vector<Parametro> *v);
} ;

// *********************************************************************

class Colgante : public NodoGrafoEscena {
public:
  Colgante(std::vector<Parametro> *v);
} ;

// *********************************************************************

class Brazo : public NodoGrafoEscena {
public:
  Brazo(Tupla3f color);
} ;

// *********************************************************************

class BrazoOrientado : public NodoGrafoEscena {
public:
  BrazoOrientado(std::vector<Parametro> *v, float ang, Tupla3f color);
} ;

// *********************************************************************

class Tronco : public NodoGrafoEscena {
public:
  Tronco(std::vector<Parametro> *v);
} ;

// *********************************************************************

class C2 : public C {
  std::string nombre;
public:
  C2();
} ;


//**********************************************************************

class C4 : public C {
  std::string nombre;
public:
  C4();
} ;

//**********************************************************************

class PeonP4 : public NodoGrafoEscena {
  std::string nombre;
public:
  // 0 = Madera, 1 = Blanco, 2 = Negro
  PeonP4(int tipo);
} ;

//**********************************************************************

class Lata : public NodoGrafoEscena {
  std::string nombre;
public:
  Lata();
} ;

//**********************************************************************

class TapaSuperiorLata : public NodoGrafoEscena {
  std::string nombre;
public:
  TapaSuperiorLata();
} ;

//**********************************************************************

class TapaInferiorLata : public NodoGrafoEscena {
  std::string nombre;
public:
  TapaInferiorLata();
} ;

//**********************************************************************

class CuerpoLata : public NodoGrafoEscena {
  std::string nombre;
public:
  CuerpoLata();
} ;

#endif // GRAFO_ESCENA_HPP
