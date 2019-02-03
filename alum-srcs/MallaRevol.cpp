// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************

MallaRevol::MallaRevol(const string &nombre_arch, const int nperfiles,
      const bool crear_tapas, const bool cerrar_malla, const bool crear_textura  ) {

  // Nos aseguramos de que no intentamos crear una malla cerrada con textura
  if (cerrar_malla && crear_textura) {
    cerr << "\n\tABORTANDO - No se puede crear una MallaRevol cerrada con texturas" << endl;
    cerr << "\tError generado para el archivo - " << nombre_arch << endl;
    assert (false);
  }

  ponerNombre( string("malla por revolución del perfil en '"+ nombre_arch + "'" ));

  vector<Tupla3f> perfil_original;
  vector<float> vert_aux;
  ply::read_vertices (nombre_arch.c_str(), vert_aux);

  for (unsigned i=0; i+2<vert_aux.size(); i+=3) {
    perfil_original.push_back ( Tupla3f{ vert_aux[i], vert_aux[i+1], vert_aux[i+2] } );
  }

  crearMallaRevol ( perfil_original, nperfiles, crear_tapas, cerrar_malla );
  if (crear_textura) {
      asignarCoordenadasTextura(perfil_original, nperfiles);
  }

  asignarColores();
  calcularCentroExplicito();
  calcular_normales();
}

// *****************************************************************************

void rotarPerfil( const Matriz4f &mat, std::vector<Tupla3f> &perfil ) {
  for (int i=0; i<perfil.size(); i++) {
    perfil[i] = mat * perfil[i];
  }
}

void MallaRevol::crearMallaRevol ( const std::vector<Tupla3f> & perfil_original,
    const int nperfiles, const bool crear_tapas, const bool cerrar_malla ) {

  assert (nperfiles != 0);
  nper = nperfiles;
  nvp = perfil_original.size();
  std::vector<Tupla3f> perfil = perfil_original;

  caras.clear();
  vertices.clear();
  float ang =  cerrar_malla ? (float) 360/nper : (float) 360/(nper-1);

  Matriz4f mat = MAT_Rotacion(ang,0,1,0);
  vertices.insert(vertices.end(), perfil.begin(), perfil.end() );

  for (int i=0; i<nper-1; i++) {
    rotarPerfil( mat, perfil );
    vertices.insert(vertices.end(), perfil.begin(), perfil.end() );

    for (int j=0; j<nvp-1; j++) {
      //if (/*i+1<nper &&*/ j+1<nvp) {
      //  caras.push_back(Tupla3i{ i*nvp+j+1, i*nvp+j, ((i+1)%nper)*nvp+j });
      //  caras.push_back(Tupla3i{ i*nvp+j+1, ((i+1)%nper)*nvp+j, ((i+1)%nper)*nvp+j+1 });
      //}
      caras.push_back(Tupla3i{ i*nvp+j+1, i*nvp+j, (i+1)*nvp+j });
      caras.push_back(Tupla3i{ i*nvp+j+1, (i+1)*nvp+j, (i+1)*nvp+j+1 });
    }
  }

  // Crear el último perfil de caras
  if (cerrar_malla) {
    for (int j=0; j<nvp-1; j++) {
      int i = nper-1;
      caras.push_back(Tupla3i{ i*nvp+j+1, i*nvp+j, j });
      caras.push_back(Tupla3i{ i*nvp+j+1, j, j+1 });
    }
  }

  // Crear las tapas (superior e inferior)
  if (crear_tapas) {
    int i_f = vertices.size();
    if (vertices[0](0) != 0) {
      vertices.push_back( Tupla3f{0, vertices[0](1), 0} );
      for (int i=0; i<nper; i++) {
        caras.push_back( Tupla3i{i*nvp, i_f, ((i+1)%nper)*nvp} );
      }
      i_f++;
    }
    if (vertices[nvp-1](0) != 0) {
      vertices.push_back( Tupla3f{0, vertices[nvp-1](1), 0} );
      for (int i=0; i<nper; i++) {
        int i1 =  ( i*nvp-1  + nvp*nper) % (nvp*nper);
        int i2 =  ( (i+1)*nvp-1  + nvp*nper) % (nvp*nper);
        caras.push_back( Tupla3i{i_f, i1, i2} );
      }
    }
  }

  // cout << leerNombre() << " -- vertices -> " << vertices.size() << endl;
  // cout << leerNombre() << " -- caras -> " << caras.size() << endl;
}

// *****************************************************************************

void MallaRevol::asignarCoordenadasTextura( const vector<Tupla3f> &
        perfil_original, const int nperfiles) {

  vector<double> perfil_dist (perfil_original.size(), 0);

  // Creamos el vector de distancias entre el elemento j del perfil y el primero
  double distancia;
  for (int j=1; j<perfil_original.size(); j++) {
    distancia = sqrt( (perfil_original[j] - perfil_original[j-1]).lengthSq() );
    perfil_dist[j] = perfil_dist[j-1] + distancia;
  }

  // Llenamos el vector de coordenadas
  double s_i, t_j, mayor_dist = perfil_dist[ perfil_dist.size() - 1 ];

  for (int i=0; i<nperfiles; i++) {
    s_i = (double) i / (nperfiles - 1);
    for (int j=0; j<perfil_original.size(); j++) {
        t_j = 1.0 - ( (double) perfil_dist[j] / mayor_dist );
        coordenadas_textura.push_back( { s_i, t_j } );
    }
  }
}


// *****************************************************************************

Cilindro::Cilindro( const int num_verts_per, const int nperfiles,
                    const bool crear_tapas, const bool cerrar_malla,
                    const float alt, const float radio) {
  assert (num_verts_per != 0);
  vector<Tupla3f> perfil_original;
  for (int i=0; i<num_verts_per; i++) {
    perfil_original.push_back( Tupla3f{radio, alt*i/(num_verts_per-1), 0} );
  }
  crearMallaRevol (perfil_original, nperfiles, crear_tapas, cerrar_malla );
  asignarCoordenadasTextura(perfil_original, nperfiles);

  ponerNombre("Cilindro");
  asignarColores();
  calcularCentroExplicito();
  calcular_normales();
}

// *****************************************************************************

Cono::Cono (const int num_verts_per, const int nperfiles,
      const bool crear_tapas, const bool cerrar_malla,
      const float alt, const float radio_base) {

  assert (num_verts_per != 0);
  vector<Tupla3f> perfil_original;
  for (int i=0; i<num_verts_per; i++) {
    perfil_original.push_back( Tupla3f{radio_base*i/num_verts_per,
                                      alt*i/num_verts_per, 0} );
  }
  crearMallaRevol (perfil_original, nperfiles, crear_tapas, cerrar_malla );

  asignarCoordenadasTextura(perfil_original, nperfiles);

  ponerNombre("Cono");
  asignarColores();
  calcularCentroExplicito();
  calcular_normales();
}

// *****************************************************************************

Esfera::Esfera (const int num_verts_per, const int nperfiles,
      const bool crear_tapas, const bool cerrar_malla,
      const float radio) {

  assert (num_verts_per != 0);
  vector<Tupla3f> perfil_original;
  for (int i=0; i<=num_verts_per; i++) {
    perfil_original.push_back( Tupla3f{radio*i/num_verts_per,
      -radio*sqrt(1 - (float) i*i/(num_verts_per*num_verts_per)), 0} );
    }
  for (int i=num_verts_per; i>=0; i--) {
    perfil_original.push_back( Tupla3f{radio*i/num_verts_per,
      radio*sqrt(1 - (float) i*i/(num_verts_per*num_verts_per)), 0} );
  }
  crearMallaRevol (perfil_original, nperfiles, false, cerrar_malla );
  asignarCoordenadasTextura(perfil_original, nperfiles);

  ponerNombre("Esfera");
  asignarColores();
  calcularCentroExplicito();
  calcular_normales();
}

// *****************************************************************************

Toroide::Toroide (const int num_verts_per, const int nperfiles,
      const bool cerrar_malla, const float desplazamiento,
      const float radio) {

  assert (num_verts_per != 0);

  vector<Tupla3f> perfil_original;
  float ang_incr = 2*3.14159265 / num_verts_per, ang = 0;
  for (int i=0; i<=num_verts_per+1; i++) {
    perfil_original.push_back( Tupla3f{desplazamiento + cos(ang),
              radio + sin(ang), 0} );
    ang += ang_incr;
  }

  crearMallaRevol (perfil_original, nperfiles, false, cerrar_malla );
  asignarCoordenadasTextura(perfil_original, nperfiles);

  ponerNombre("Toroide");
  asignarColores();
  calcularCentroExplicito();
  calcular_normales();
}
