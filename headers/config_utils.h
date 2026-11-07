#ifndef CONFIG_UTILS_H
#define CONFIG_UTILS_H

#include "config.pb.h"
#include <string>

namespace ConfigUtils {
    void load(Config& config);
    bool save(Config& config);
    
    void initUnsetPropertiesWithDefaults(Config& config);
}

#endif
