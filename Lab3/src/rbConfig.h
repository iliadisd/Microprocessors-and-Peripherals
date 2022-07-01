//RB_PROC:ALL:HEADER_DATA

#ifndef RB_CONFIG_H_
#define RB_CONFIG_H_

// switch logging on, if not otherwise specified via compiler switch
#ifndef RB_LOG_ON
#define RB_LOG_ON 1
#endif

// the output of logging locations is deactivated, because otherwise the resulting binary size exceed the
// maximum allowed linker size of 32kb in the uVision evaluation version
#define RB_LOG_LOCATION_ON 0

// add project specific configuration options before this include directive
#include <rbConfig-default.h>

#endif //RB_CONFIG_H_
