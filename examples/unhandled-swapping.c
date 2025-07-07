/* test_swap.c - compatible con el C* de Selfie */

/*
  Este test está diseñado para verificar el ciclo completo de swapping
  (page-out y page-in) en Selfie OS.

  FASES DEL TEST:
  1. Escribir en un array 'a' para llenarlo con datos conocidos.
  2. Escribir en un segundo array 'b' para forzar que las páginas de 'a'
     sean enviadas al espacio de swap (page-out).
  3. Leer de vuelta del array 'a' para forzar que sus páginas sean traídas
     desde el swap (page-in) y verificar que los datos son correctos.
*/

// --- VARIABLES GLOBALES ---

// Tamaño de los arrays. Un valor de 500,000 elementos de 8 bytes
// resulta en arrays de ~4MB cada uno. Ejecuta selfie con -m 1 o -m 2
// para asegurar que el swapping sea necesario.
uint64_t size = 500000;

uint64_t* a;
uint64_t* b;

// --- FUNCIÓN PRINCIPAL ---

uint64_t main() {
    // Las variables deben ser declaradas al inicio en C*
    uint64_t i;
    uint64_t errors;
    uint64_t value_read;

    errors = 0;

    write(1, "-> Iniciando test de swapping para Selfie OS\n", 45);

    // --- RESERVA DE MEMORIA ---
    a = malloc(sizeof(uint64_t) * size);
    b = malloc(sizeof(uint64_t) * size);

    // Verificamos si malloc falló usando dos if, en lugar de '||'
    if (a == (uint64_t*)0) {
        write(1, "Error: malloc para 'a' falló.\n", 30);
        return 1;
    }
    if (b == (uint64_t*)0) {
        write(1, "Error: malloc para 'b' falló.\n", 30);
        return 1;
    }

    // -----------------------------------------------------------------
    // FASE 1: Escribir en 'a' (llenar memoria)
    // -----------------------------------------------------------------
    write(1, "\n--- FASE 1: Escribiendo en 'a'.\n", 34);
    i = 0;
    while (i < size) {
        *(a + i) = i + 1; // Usamos i+1 para evitar el valor 0
        i = i + 1;
    }
    write(1, "-> Fase 1 completada.\n", 22);

    // -----------------------------------------------------------------
    // FASE 2: Escribir en 'b' (forzar page-out de 'a')
    // -----------------------------------------------------------------
    write(1, "\n--- FASE 2: Escribiendo en 'b' (forzando swap-out).\n", 54);
    i = 0;
    while (i < size) {
        *(b + i) = i;
        i = i + 1;
    }
    write(1, "-> Fase 2 completada.\n", 22);

    // -----------------------------------------------------------------
    // FASE 3: Leer de 'a' (forzar page-in y verificar)
    // -----------------------------------------------------------------
    write(1, "\n--- FASE 3: Verificando 'a' (forzando swap-in).\n", 51);
    i = 0;
    while (i < size) {
        value_read = *(a + i);
        if (value_read != i + 1) {
            // Si hay un error, lo registramos.
            // Imprimir números es complejo en C*, así que solo contamos los errores.
            errors = errors + 1;
        }
        i = i + 1;
    }
    write(1, "-> Fase 3 completada.\n", 22);


    // -----------------------------------------------------------------
    // VEREDICTO FINAL
    // -----------------------------------------------------------------
    write(1, "\n--- VEREDICTO ---\n", 19);
    if (errors == 0) {
        write(1, "¡ÉXITO! El test se completó sin errores.\n", 41);
    } else {
        // En un C* real, necesitaríamos una función para imprimir 'errors'.
        // Por ahora, solo indicamos que hubo fallos.
        write(1, "FALLO: Se encontraron errores en la verificación.\n", 51);
    }

    return errors;
}
