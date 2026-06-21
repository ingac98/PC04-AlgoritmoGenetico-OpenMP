#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include "genetic.h"

/*--------------------------------------------------
    Lee el archivo de configuración
---------------------------------------------------*/

GAConfig loadConfiguration(
    const std::string& filename);

#endif