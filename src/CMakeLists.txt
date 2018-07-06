add_library(ada ADAFRUIT_9DOF.cpp L3GD20.cpp LSM303.cpp MadgwickAHRS.c)
target_compile_options(ada PUBLIC "$<$<CONFIG:DEBUG>:-ggdb -std=c++11 -pthread>")
target_compile_options(ada PUBLIC "$<$<CONFIG:RELEASE>:-O3 -std=c++11 -pthread>")