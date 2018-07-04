add_library(ada ADAFRUIT_9DOF.cpp L3GD20.cpp LSM303.cpp)
target_compile_options(ada PUBLIC "$<$<CONFIG:DEBUG>:-ggdb>")
target_compile_options(ada PUBLIC "$<$<CONFIG:RELEASE>:-O3>")