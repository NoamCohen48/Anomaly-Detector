cmake_minimum_required(VERSION 3.20)
project(Project2)

set(CMAKE_CXX_STANDARD 11)
set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(Project2
        anomaly_detection_util.cpp anomaly_detection_util.h
        SimpleAnomalyDetector.cpp SimpleAnomalyDetector.h
        timeseries.cpp timeseries.h
        MainTrain.cpp
        minCircle.cpp minCircle.h
        HybridAnomalyDetector.cpp HybridAnomalyDetector.h
        CLI.cpp CLI.h
        commands.h
        Server.cpp
        Server.h
        AnomalyDetector.h)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )