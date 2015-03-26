rock_library(serialization
    SOURCES typelib_marshalling.cpp
    HEADERS
    typelib_marshalling.hpp
    eigen.hpp
    control_system_lib.hpp
    DEPS_PKGCONFIG typelib base-lib)

#rock_executable(serialization_bin Main.cpp
#    DEPS serialization)

