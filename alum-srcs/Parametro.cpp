#include <cassert>
#include <cmath>
#include "Parametro.hpp"
#include "Objeto3D.hpp"

// -----------------------------------------------------------------------------

const float Parametro::ac = 0.001;
const float Parametro::incremento = 0.5;
const float Parametro::pi = atan(1)*4;
const float Parametro::v_0 = 0.005;

// crear grado de libertad no acotado
Parametro::Parametro(const std::string &p_descripcion, Matriz4f *p_ptr_mat,
  TFuncionCMF p_fun_calculo_matriz, bool p_acotado,
  float p_c, float p_s, float p_f ) : descripcion (p_descripcion),
  ptr_mat (p_ptr_mat), fun_calculo_matriz (p_fun_calculo_matriz),
  acotado (p_acotado), c (p_c), s (p_s), f (p_f), vel(v_0) {
   // COMPLETAR: práctica 3: inicializar un parámetro
   // ....
}
// -----------------------------------------------------------------------------
void Parametro::ActualizarMatriz() {
  //valor_norm = acotado ? c + s*valor_norm : c + s*sin(f*2*pi*valor_norm);
  *(ptr_mat) = fun_calculo_matriz ( leer_valor_actual() );
}
// -----------------------------------------------------------------------------
void Parametro::siguiente_cuadro() {
   valor_norm += vel;
   ActualizarMatriz();
}
// -----------------------------------------------------------------------------
void Parametro::reset() {
  valor_norm = c;
  vel = v_0;
  ActualizarMatriz();
}
// -----------------------------------------------------------------------------
void Parametro::incrementar() {
  valor_norm += incremento;
  ActualizarMatriz();
}
// -----------------------------------------------------------------------------
void Parametro::decrementar() {
  valor_norm -= incremento;
  ActualizarMatriz();
}
// -----------------------------------------------------------------------------
void Parametro::acelerar() {
  vel += ac;
}
// -----------------------------------------------------------------------------
void Parametro::decelerar() {
  vel -= ac;
  if (vel < 0)
    vel = 0;
}
// -----------------------------------------------------------------------------
float Parametro::leer_valor_actual() {
  return acotado ? c + s*sin(f*2*pi*valor_norm) : c + s*valor_norm;
  //return valor_norm;
}

// -----------------------------------------------------------------------------
float Parametro::leer_velocidad_actual() {
  return vel;
}
// -----------------------------------------------------------------------------

std::string Parametro::leer_descripcion() {
  return descripcion;
}
// -----------------------------------------------------------------------------
