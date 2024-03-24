cmake_minimum_required(VERSION 3.10)
project(EngineSimulation)


include_directories(include shared)

file(GLOB libengine_SRC
        "source/*.h"
        "source/*.c"
)

add_library(engine SHARED ${libengine_SRC})

add_executable(test_basic test/basic.c)
target_link_libraries(test_basic engine)

add_executable(test_generate_rabits test/generate_rabits.c)
target_link_libraries(test_generate_rabits engine)

add_executable(test_engine_sample test/engine_sample.c)
target_link_libraries(test_engine_sample engine)
