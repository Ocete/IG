// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************


#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );

   vertices.clear();
   caras.clear();
   std::vector<float> vert_aux;
   std::vector<int> caras_aux;
   ply::read (nombre_arch.c_str(), vert_aux, caras_aux);

   for (unsigned i=0; i+2<vert_aux.size(); i+=3) {
     vertices.push_back ( Tupla3f{ vert_aux[i], vert_aux[i+1], vert_aux[i+2] } );
   }
   for (unsigned i=0; i+2<caras_aux.size(); i+=3) {
     caras.push_back ( Tupla3i{ caras_aux[i], caras_aux[i+1], caras_aux[i+2] } );
   }

   asignarColores();
   calcular_normales();
}

// *****************************************************************************
