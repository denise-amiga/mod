//#include "id/commit.h"

//#define ANGLE_VERSION_STRING "axe.angle.mod v0.01"
#define ANGLE_COMMIT_HASH "axe.angle.mod v0.01"

#define ANGLE_MAJOR_VERSION 2
#define ANGLE_MINOR_VERSION 1

#define ANGLE_STRINGIFY(x)       #x
#define ANGLE_MACRO_STRINGIFY(x) ANGLE_STRINGIFY(x)

#define ANGLE_VERSION_STRING                       \
    ANGLE_MACRO_STRINGIFY(ANGLE_MAJOR_VERSION) "." \
    ANGLE_MACRO_STRINGIFY(ANGLE_MINOR_VERSION) "." \
    ANGLE_COMMIT_HASH