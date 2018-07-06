add_library(src ADAFRUIT_9DOF.cpp L3GD20.cpp LSM303.cpp)
target_compile_options(src PUBLIC "$<$<CONFIG:DEBUG>:-ggdb>")
target_compile_options(src PUBLIC "$<$<CONFIG:RELEASE>:-O3>")