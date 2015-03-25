rock_library(serialization
    SOURCES Dummy.cpp
    HEADERS Dummy.hpp)

rock_executable(serialization_bin Main.cpp
    DEPS serialization)

