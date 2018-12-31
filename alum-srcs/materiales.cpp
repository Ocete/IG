// *********************************************************************
// **
// ** Gestión de materiales y texturas (implementación)
// **
// ** Copyright (C) 2014 Carlos Ureña
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


#include "matrices-tr.hpp"
#include "materiales.hpp"

using namespace std ;

//*********************************************************************

PilaMateriales::PilaMateriales()
{
   actual = nullptr ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarMaterial( Material * material )
{
   if ( material != actual ) {
      actual = material ;
      if ( actual != nullptr )
         actual->activar();
   }
}
// -----------------------------------------------------------------------------

void PilaMateriales::activarActual()
{
   if ( actual != nullptr )
      actual->activar() ;
}
// -----------------------------------------------------------------------------

void PilaMateriales::push(  )
{
   pila.push_back( actual );
}
// -----------------------------------------------------------------------------

void PilaMateriales::pop(  ) {
   if ( pila.size() == 0 ) {
      cout << "error: intento de hacer 'pop' de un material en una pila de materiales vacía." << endl << flush ;
      exit(1);
   }

   Material * anterior = pila[pila.size()-1] ;
   pila.pop_back();
   activarMaterial( anterior );  // cambia 'actual'
}

//**********************************************************************

Textura::Textura( const std::string & nombreArchivoJPG ) {
   imagen = new jpg::Imagen( nombreArchivoJPG );
   enviada = false;
   glGenTextures( 1, &ident_textura );
   modo_gen_ct = mgct_desactivada;
}

// ---------------------------------------------------------------------

void Textura::enviar() {
  if ( enviada ) {
    cerr << "\tABORTANDO - intentando enviar textura ya enviada" << endl;
    assert(false);
  }

  // Configuramos las coordenadas de textura antes de enviarla
  if ( modo_gen_ct == mgct_coords_objeto ) {
    glTexGenfv ( GL_S, GL_OBJECT_PLANE, coefs_s );
    glTexGenfv ( GL_T, GL_OBJECT_PLANE, coefs_t );
  } else if ( modo_gen_ct == mgct_coords_ojo ) {
    glTexGenfv ( GL_S, GL_EYE_PLANE, coefs_s );
    glTexGenfv ( GL_T, GL_EYE_PLANE, coefs_t );
  }

  // Activación y envío
  glBindTexture( GL_TEXTURE_2D, ident_textura );
  gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, imagen->tamX(), imagen->tamY(),
      GL_RGB, GL_UNSIGNED_BYTE, imagen->leerPixels() );

  // Configuramos la textura
  // glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR );
  // glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  enviada = true;
}

//----------------------------------------------------------------------

Textura::~Textura( ) {
   using namespace std ;
   cout << "destruyendo textura...imagen ==" << imagen << endl ;
   if ( imagen != NULL )
      delete imagen ;

   imagen = NULL ;
   cout << "hecho (no hecho!)" << endl << flush ;
}

//----------------------------------------------------------------------
// por ahora, se asume la unidad de texturas #0

void Textura::activar(  ) {
  glEnable( GL_TEXTURE_2D );
  if ( !enviada ) {
   enviar();
  }
  glBindTexture( GL_TEXTURE_2D, ident_textura );

  // Esto no se si hace falta
  if(modo_gen_ct==mgct_desactivada){
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
  } else {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    if (modo_gen_ct == mgct_coords_ojo){
      glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
      glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    } else {
       glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
       glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    }
  }
}

// *********************************************************************

Material::Material() {
   iluminacion = false ;
   tex = NULL ;
   coloresCero() ;
}

// ---------------------------------------------------------------------

Material::Material( const std::string & nombreArchivoJPG ) {
   iluminacion    = true ;
   tex            = new Textura( nombreArchivoJPG ) ;

   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   del.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   del.difusa    = VectorRGB( 0.5, 0.5, 0.5, 1.0 );
   del.especular = VectorRGB( 1.0, 1.0, 1.0, 1.0 );

   tra.emision   = VectorRGB(0.0,0.0,0.0,1.0);
   tra.ambiente  = VectorRGB( 0.0, 0.0, 0.0, 1.0);
   tra.difusa    = VectorRGB( 0.2, 0.2, 0.2, 1.0 );
   tra.especular = VectorRGB( 0.2, 0.2, 0.2, 1.0 );

}

// ---------------------------------------------------------------------
// crea un material usando textura y coeficientes: ka,kd,ks y exponente
// (la textura puede ser NULL, la ilum. queda activada)

Material::Material( Textura * text, float ka, float kd, float ks, float exp )
      :  Material() {
   tex = text;
   iluminacion = true;
   coloresCero();

   del.emision   = VectorRGB(0.0,0.0,0.0, 1.0);
   del.ambiente  = VectorRGB(ka, ka, ka, 1.0);
   del.difusa    = VectorRGB(kd, kd, kd, 1.0);
   del.especular = VectorRGB(ks, ks, ks, 1.0);

   tra.emision   = VectorRGB(0.0,0.0,0.0, 1.0);
   tra.ambiente  = VectorRGB(ka, ka, ka, 1.0);
   tra.difusa    = VectorRGB(kd, kd, kd, 1.0);
   tra.especular = VectorRGB(ks, ks, ks, 1.0);

   del.exp_brillo = exp;
   tra.exp_brillo = exp;

   ponerNombre("material con textura e iluminación") ;
 }

// ---------------------------------------------------------------------
// crea un material con un color único para las componentes ambiental y difusa
// en el lugar de textura (textura == NULL)
Material::Material( const Tupla3f & colorAmbDif, float ka, float kd, float ks,
        float exp ) : Material (NULL, ka, kd, ks, exp) {

  coloresCero();
  glColor3fv(colorAmbDif); //
  glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

  //color = VectorRGB(colorAmbDif[0], colorAmbDif[1], colorAmbDif[2], 0.0);
  ponerNombre("material color plano, ilum.") ;
}
// ---------------------------------------------------------------------

// crea un material con un color plano sin textura ni iluminación
Material::Material( const float r, const float g, const float b ) {
  tex = NULL;
  iluminacion = false;
  coloresCero();
  color = VectorRGB(r,g,b, 1.0);
  ponerNombre("material con color plano, sin iluminación");
}

//----------------------------------------------------------------------

void Material::coloresCero() {
   const VectorRGB ceroRGBopaco(0.0,0.0,0.0,1.0);

   color         =

   del.emision   =
   del.ambiente  =
   del.difusa    =
   del.especular =

   tra.emision   =
   tra.ambiente  =
   tra.difusa    =
   tra.especular = ceroRGBopaco ;

   del.exp_brillo =
   tra.exp_brillo = 1.0 ;
}
//----------------------------------------------------------------------

Material::~Material() {
   if ( tex != nullptr )
   {  delete tex ;
      tex = nullptr ;
   }
}

//----------------------------------------------------------------------

void Material::ponerNombre( const std::string & nuevo_nombre )
{
   nombre_mat = nuevo_nombre ;
}

//----------------------------------------------------------------------

std::string Material::nombre() const {
   return nombre_mat ;
}

//----------------------------------------------------------------------

void Material::activar(  ) {
  glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Tupla4f {1,1,1,1} ) ;

  if (tex != NULL) {
    tex->activar();
  } else {
    glDisable( GL_TEXTURE_2D );
  }

  if ( iluminacion ) {

    glMaterialfv( GL_FRONT, GL_EMISSION, del.emision ) ;
    glMaterialfv( GL_FRONT, GL_AMBIENT, del.ambiente ) ;
    glMaterialfv( GL_FRONT, GL_DIFFUSE, del.difusa ) ;
    glMaterialfv( GL_FRONT, GL_SPECULAR, del.especular ) ;
    glMaterialf( GL_FRONT, GL_SHININESS, del.exp_brillo ) ;

    glMaterialfv( GL_BACK, GL_EMISSION, tra.emision ) ;
    glMaterialfv( GL_BACK, GL_AMBIENT, tra.ambiente ) ;
    glMaterialfv( GL_BACK, GL_DIFFUSE, tra.difusa ) ;
    glMaterialfv( GL_BACK, GL_SPECULAR, tra.especular ) ;
    glMaterialf( GL_BACK, GL_SHININESS, tra.exp_brillo ) ;

    glEnable (GL_LIGHTING);

  } else {
    glDisable( GL_LIGHTING );
    glColor4fv( color );
    glColorMaterial( GL_FRONT_AND_BACK, GL_EMISSION );
    glColorMaterial( GL_FRONT_AND_BACK, GL_SPECULAR );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );

    glEnable( GL_COLOR_MATERIAL );
  }
}

//----------------------------------------------------------------------

MaterialLata::MaterialLata() {

}

//----------------------------------------------------------------------

MaterialTapasLata::MaterialTapasLata() {

}

//----------------------------------------------------------------------

MaterialPeonMadera::MaterialPeonMadera()
    : Material (new Textura("../imgs/text-madera.jpg"),0.5,1,1,1) {
  ponerNombre( "MaterialMadera" );
  //del.emision = Tupla4f{0.5,0.5,0.5,1.0};
  //tra.emision = Tupla4f{0.5,0.5,0.5,1.0};
}

//----------------------------------------------------------------------

MaterialPeonBlanco::MaterialPeonBlanco()
    : Material( {1,0,0}, 1, 1, 0, 0) {
  ponerNombre( "MaterialBlanco" );
  del.emision = Tupla4f{1,1,1,1.0};
  tra.emision = Tupla4f{1,1,1,1.0};
}

//----------------------------------------------------------------------

MaterialPeonNegro::MaterialPeonNegro()
    : Material( {0.5,0.5,0.5}, 1, 0.2, 1, 1) {
  ponerNombre( "MaterialNegro" );
  //del.emision = Tupla4f{0.0,0.0,0.0,1.0};
  //tra.emision = Tupla4f{0.0,0.0,0.0,1.0};
}

//**********************************************************************

/*
FuenteLuz::FuenteLuz( GLfloat p_longi_ini, GLfloat p_lati_ini, const VectorRGB & p_color )
{
   //CError();

   if ( trazam )
      cout << "creando fuente de luz." <<  endl << flush ;

   // inicializar parámetros de la fuente de luz
   longi_ini = p_longi_ini ;
   lati_ini  = p_lati_ini  ;
   longi = longi_ini ;
   lati  = lati_ini ;

   col_ambiente  = p_color ;
   col_difuso    = p_color ;
   col_especular = p_color ;

   ind_fuente = -1 ; // la marca como no activable hasta que no se le asigne indice

   //CError();
}
*/

//**********************************************************************

FuenteLuz::FuenteLuz( const VectorRGB & p_color ) {
  //cout << "Creando fuente de luz." <<  endl << flush ;

  col_ambiente  = p_color ;
  col_difuso    = p_color ;
  col_especular = p_color ;

  ind_fuente = -1 ;
}

//----------------------------------------------------------------------

void FuentePosicional::activar() {
   if (ind_fuente == -1) {
     cerr << "ABORTANDO - intentando activar una fuente posicional con indice -1" << endl;
     assert (false);
   }

   glEnable (GL_LIGHT0 + ind_fuente);
   glLightfv (GL_LIGHT0 + ind_fuente, GL_AMBIENT, col_ambiente);
   glLightfv (GL_LIGHT0 + ind_fuente, GL_DIFFUSE, col_difuso);
   glLightfv (GL_LIGHT0 + ind_fuente, GL_SPECULAR, col_especular);
   glLightfv (GL_LIGHT0 + ind_fuente, GL_POSITION, posvec );
}

//----------------------------------------------------------------------
/*
// Si hago esta función abstract (como debería ser)
// aparecen una serie de errores que no se resolver
bool FuenteLuz::gestionarEventoTeclaEspecial( int key ) {
    return false;
}*/

//**********************************************************************

FuenteDireccional::FuenteDireccional(float alpha_inicial, float beta_inicial,
  const VectorRGB & p_color) : FuenteLuz (p_color) {
    this->lati_ini = lati_ini;
    this->longi_ini = longi_ini;
    lati = lati_ini;
    longi = longi_ini;
}

//----------------------------------------------------------------------

void FuenteDireccional::activar() {
  if (ind_fuente == -1) {
   assert (false);
  }

  // Habilitamos la fuente
  glEnable(GL_LIGHT0+ind_fuente);

  // Colores de ambiente
  glLightfv( GL_LIGHT0 + ind_fuente, GL_AMBIENT, col_ambiente );
  glLightfv( GL_LIGHT0 + ind_fuente, GL_DIFFUSE, col_difuso );
  glLightfv( GL_LIGHT0 + ind_fuente, GL_SPECULAR, col_especular );

  ActualizarDireccion();
}

//----------------------------------------------------------------------

bool FuenteDireccional::gestionarEventoTeclaEspecial( int key ) {
  bool actualizar = true ;
  const float incr = 3.0f ;

  switch( key ) {
    case GLFW_KEY_RIGHT:
      variarAngulo(1, incr);
      break ;
    case GLFW_KEY_LEFT:
      variarAngulo(1, (-1)*incr);
      break ;
    case GLFW_KEY_UP:
      variarAngulo(0, incr);
      break ;
    case GLFW_KEY_DOWN:
      variarAngulo(0, (-1)*incr);
      break ;
    case GLFW_KEY_HOME:
      lati  = lati_ini ;
      longi = longi_ini ;
      ActualizarDireccion();
      break ;
    default :
      actualizar = false ;
      cout << "tecla no usable para la fuente de luz." << endl << flush ;
  }

  if ( actualizar )
    cout << "fuente de luz cambiada: longi == " << longi << ", lati == " << lati << endl << flush ;
  return actualizar ;
}

//----------------------------------------------------------------------

void FuenteDireccional::variarAngulo( unsigned angulo, float incr ) {
  if (incr != 0) {
    if (angulo == 0) {
      lati = incr > 0 ? min( lati+incr, 90.0f) : max( lati+incr, -90.0f );
    } else if (angulo == 1) {
      longi += incr;
    }
    ActualizarDireccion();
  }
}

//----------------------------------------------------------------------

void FuenteDireccional::ActualizarDireccion() {
  float ejeZ[4] = {0.0, 0.0, 1.0, 0.0 };
  glMatrixMode (GL_MODELVIEW );
  glPushMatrix() ;
  glLoadIdentity();
  glRotatef (longi, 0.0, 1.0, 0.0);
  glRotatef (lati, -1.0, 0.0, 0.0 );

  glLightfv (GL_LIGHT0 + ind_fuente, GL_POSITION, ejeZ);
  // Dudo que esto vaya a funcionar
  //posvec = ejeZ;

  glPopMatrix();
}

//**********************************************************************

FuentePosicional::FuentePosicional ( const Tupla3f & posicion,
      const VectorRGB & p_color ) : FuenteLuz (p_color) {
  posvec = { posicion[0], posicion[1], posicion[2], 1.0 };
}

//----------------------------------------------------------------------

bool FuentePosicional::gestionarEventoTeclaEspecial( int key ) {
  cout << "Las fuentes posicionales no gestionan teclas" << endl;
  return false;
}

//**********************************************************************

ColFuentesLuz::ColFuentesLuz() {
   max_num_fuentes = 8 ;
}

//----------------------------------------------------------------------

// Inserta una fuente de luz nueva
void ColFuentesLuz::insertar( FuenteLuz * pf ) {
   if ( pf == nullptr ) {
     cerr << "\tABORTANDO - Intentando insertar fuente nula" << endl;
     assert(false);
   } else if ( vpf.size() == max_num_fuentes ) {
     cerr << "\tABORTANDO - número máximo de fuentes alcanzado: " << max_num_fuentes << endl;
     assert(false);
   }

   pf->ind_fuente = vpf.size() ;
   vpf.push_back( pf ) ;
}

//----------------------------------------------------------------------

// activa una colección de fuentes de luz
void ColFuentesLuz::activar( unsigned id_prog ) {

  glEnable(GL_LIGHTING);
  for (int i=0; i<vpf.size(); i++) {
    vpf[i]->activar();
  }
  for (int i=vpf.size(); i<max_num_fuentes; i++) {
    glDisable (GL_LIGHT0 + i);
  }

}

//----------------------------------------------------------------------

FuenteLuz * ColFuentesLuz::ptrFuente( unsigned i ) {
   assert(i < vpf.size()) ;
   return vpf[i] ;
}

//----------------------------------------------------------------------

ColFuentesLuz::~ColFuentesLuz() {
   for( unsigned i = 0 ; i < vpf.size() ; i++ ) {
      assert( vpf[i] != NULL );
      delete vpf[i] ;
      vpf[i] = NULL ;
   }
}
