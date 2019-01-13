#include <aux.hpp>
#include "CamaraInter.hpp"


// -----------------------------------------------------------------------------
// constructor de cámaras interactivas, los parámetros son:
//
//    * examinar_ini: fija modo examinar (true) o modo primera persona (false)
//    * ratio_yx_vp_ini: ratio del viewport (alto/ancho)
//    * longi_ini_grad, lati_ini_grad: longitud y latitud iniciales (en grados)
//    * aten_ini: punto de atención inicial
//    * pers_ini: fija proy. perspectiva (true) u ortográfica (false)
//

CamaraInteractiva::CamaraInteractiva( bool examinar_ini, float ratio_yx_vp_ini,
                                      float longi_ini_grad, float lati_ini_grad,
                                      const Tupla3f & aten_ini, bool pers_ini,
                                      float hfov_grad_ini, float dist_ini  )
{
   using namespace std ;

   examinar    = examinar_ini ;
   longi       = longi_ini_grad ;
   lati        = lati_ini_grad ;
   dist        = dist_ini ;
   aten        = aten_ini ;
   ratio_yx_vp = ratio_yx_vp_ini; //cout << "ratio_yx_vp_ini == " << ratio_yx_vp_ini << endl ;
   perspectiva = pers_ini ;
   hfov_grad   = hfov_grad_ini ;
   dx          = 0.0 ;
   dy          = 0.0 ;

   amplitud = 1000.0;

   calcularViewfrustum();    // lee: dist, ratio_yx_vp, perspectica
   calcularMarcoCamara();    // lee: longi, lati, dist, aten
}


// ----------------------------------------------------------------------------
// calcula view-frustum (la matriz de proyección) usando:
//    perspectiva, dist, ratio_yx_vp

constexpr float n = 0.01 ; // valor 'near' en proy. persp

void CamaraInteractiva::calcularViewfrustum( ) {
  using namespace std ;
  // COMPLETAR: práctica 5: calcular los parámetros del view frustum (vf),
  // y actualiza la matriz de proyección (vf.matrizProy)

  if ( perspectiva ) {
    // caso perspectiva: usar hfov_grad, n, ratio_yx_vp, dist, función MAT_Frustum

    //vf.persp = true;
    // diapo 43 de 248, tema 3

    vf = ViewFrustum (hfov_grad, ratio_yx_vp, 0.1, dist + 20 );
    //vf.matrizProy = MAT_Perspectiva ( hfov_grad, ratio_yx_vp, dist, dist + amplitud );
    //gluPerspective ( hfov_grad, ratio_yx_vp, dist, dist + amplitud );


  } else {
    // caso ortográfica: usar ratio_yx_vp, dist, función MAT_Ortografica
    vf.persp = false;

    assert( epsilon < hfov_grad );
    float hfov_rad = (hfov_grad*M_PI)/180.0 ;

    vf.near = 0.1;
    vf.far = 120;

    vf.top    = vf.near*tan( 0.5*hfov_rad ) ;
    vf.bottom = -vf.top ;
    vf.right  = vf.top*ratio_yx_vp  ;
    vf.left   = -vf.right ;

    vf.matrizProy = MAT_Ortografica( vf.left, vf.right, vf.bottom,
                                      vf.top, vf.near, vf.far );
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf( vf.matrizProy );
}

//-----------------------------------------------------------------------
// calcula el marco de referencia de la camara (y la matriz de vista), usando:
//     longi, lati, dist, aten

void CamaraInteractiva::calcularMarcoCamara() {

  // COMPLETADO: práctica 5: recalcular 'mcv.matrizVista' y 'mcv.matriVistaInv'
  //    (1) Matriz = Trasl( aten )*Rotacion( longi, Y )*Rotacion( -lati, X )* Traslacion( (0,0,dist) )
  //    (2) ejes mcv = ejes mcv * matriz
  //    (3) recalcular matrices marco camara

  // Creo que la segunda rotacion deberia ser alrededor del eje Z
  Matriz4f mat = MAT_Traslacion( aten )
                * MAT_Rotacion( longi, 0, 1, 0)
                * MAT_Rotacion( -lati, 1, 0, 0)
                * MAT_Traslacion ( 0, 0, dist);

  /*Matriz4f mat = MAT_Traslacion ( 0, 0, dist)
                * MAT_Rotacion( -lati, 1, 0, 0)
                * MAT_Rotacion( longi, 0, 1, 0)
                * MAT_Traslacion( aten );
*/

  Matriz4f mEjes = MAT_Ident();
  mEjes = mat * mEjes;

  for (int i=0; i<3; i++) {
    for (int j=0; j<4; j++) {
      mcv.eje[i][j] = mEjes(i,j);
    }
  }

  for (int i=0; i<3; i++) {
    mcv.org[i] = mEjes(3,i);
  }
/*
  Matriz4f mEjes = MAT_Filas(mcv.eje);
  mEjes(3, 3) = 1.0;
  Matriz4f ejesCambiados = mEjes * mat;
  for (unsigned i = 0; i < 3; ++i)
  for (unsigned j = 0; j < 3; ++j)
    mcv.eje[i](j) = ejesCambiados(i, j);
/*
  for (int i=0; i<3; i++) {
    mcv.eje[i] = mat * mcv.eje[i];
  }
*/
  recalcularMatrMCV();
}


//-----------------------------------------------------------------------
// actualiza las matrices que hay en 'cam.mcv'
// a patir de 'cam.mcv.org' y 'cam.mcv.ejes'

void CamaraInteractiva::recalcularMatrMCV() {
  // COMPLETADO: práctica 5: recalcular 'mcv.matrizVista' y
  // 'mcv.matriVistaInv' en 'mcv' usando 'mcv.eje[X/Y/Z]' y 'mcv.org'
  mcv.matrizVista = MAT_Vista( mcv.eje, mcv.org ) ;
  mcv.matrizVistaInv = MAT_Vista_inv( mcv.eje, mcv.org );

  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity();
  glMultMatrixf ( mcv.matrizVista ) ;
}


// -----------------------------------------------------------------------------
//  métodos para manipular (desplazar o rotar) la cámara
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// desplazar o rotar la cámara en horizontal/vertical

constexpr float urot  = 1,    // unidad de rotación (1 grado)
                udesp = 0.1 ;  // unidad de desplazamiento

void CamaraInteractiva::moverHV( float nh, float nv ) {
  using namespace std ;

  if ( nh == 0 && nv == 0 ) {
    return ;
  }

  if ( examinar ) { // examinar

    // COMPLETADO: práctica 5: actualizar 'longi' y 'lati'
    // y recalcular marco de cámara
    longi += nh*urot;
    lati += nv*urot;

    calcularMarcoCamara();

  } else { // primer persona

    // COMPLETADO: práctica 5: desplazar 'mcv.org' en X e Y
    // y recalcular la matriz de vista
    // (y movimiento solidario del punto de atención)
    mcv.org[0] += nh*udesp;
    mcv.org[1] += nv*udesp;

    // dudoso:
    aten[0] += nh*udesp;
    aten[1] += nv*udesp;

    recalcularMatrMCV();
  }
}
// -----------------------------------------------------------------------------
// desplazar en el eje Z de la cámara (hacia adelante o hacia detrás)

constexpr float dmin = 2.0*n,  // distancia minima (2*near)
                porc = 2 ; // porcentaje de cambio (1%)

void CamaraInteractiva::desplaZ( float nz ) {

  using namespace std ;

  if ( nz == 0 ) {
    return ;
  }

  if ( examinar ) {// examinar

    // COMPLETADO: práctica 5: actualizar 'dist' usando 'nz',
    // 'dmin' y 'porc' y recalcular marco de cámara
    dist = dmin + (dist-dmin) * (1.0 - nz * porc/100.0);

    calcularMarcoCamara();

  } else { // primer persona

    // COMPLETADO: práctica 5: desplazar 'mcv.org' en Z,
    // y recalcular la matriz de vista
    // (y movimiento solidario del punto de atención)
    mcv.org[2] += nz*udesp;
    aten[2] += nz*udesp;

    recalcularMatrMCV();

  }
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// métodos para cambiar de modo
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// fija punt. aten. y activa modo examinar

void CamaraInteractiva::modoExaminar( const Tupla3f & pAten ) {
  // COMPLETADO: práctica 5: fija punt. aten. y activa modo examinar, recalcula marco de camara
  examinar = true;
  aten = pAten;
  calcularMarcoCamara();
}

// -----------------------------------------------------------------------------
// pasa a modo examinar (mantiene p.aten.)

void CamaraInteractiva::modoExaminar() {
  // COMPLETADO: práctica 5: activa modo examinar
  // (no es necesario recalcular el marco de cámara)
  examinar = true;
}

// -----------------------------------------------------------------------------
// pasa al modo primera persona

void CamaraInteractiva::modoPrimeraPersona() {
  // COMPLETADO: práctica 5: activa modo primera persona
  // (no es necesario recalcular el marco de cámara)
  examinar = false;
}
