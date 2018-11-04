#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ;

// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro {
private:
  static const float ac;
  static const float incremento;
  static const float pi;
  static const float v_0;
  float vel;

  const std::string descripcion ;
  const bool acotado;
  const TFuncionCMF fun_calculo_matriz; // función que produce una nueva matriz a partir de un valor flotante
  const float c, s, // semiamplitud (si acotado), o factor de escala (si no acotado)
                 f; // si acotado: frecuencia (ciclos por unidad del valor normalizado)
  Matriz4f * const ptr_mat ; // puntero a la matriz dentro del modelo
  float valor_norm; // valor actual, normalizado, no acotado (crec

  void ActualizarMatriz(); // Normaliza valor_norm y actualiza la matriz

public:
   Parametro (
      const std::string &  p_descripcion,
      Matriz4f *           p_ptr_mat,
      TFuncionCMF          p_fun_calculo_matriz,
      bool                 p_acotado,
      float p_c, float p_s, float p_f );

   void  siguiente_cuadro();   // actualizar valor y matriz al siguiente frame
   void  reset();        // vuelve al estado inicial
   void  incrementar();  // incrementar el valor
   void  decrementar() ; // decrementar el valor
   void  acelerar();     // acelerar (aumentar velocidad normalizada)
   void  decelerar();    // decelerar (disminuir la velocidad normalizada)
   float leer_valor_actual(); // devuelve el valor actual (escalado, no normalizado)
   float leer_velocidad_actual();    // devuelve velocidad o frecuencia actual
   std::string leer_descripcion();
};

#endif
