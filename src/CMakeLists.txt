set(SU1_SOURCES
    main.cpp
    managers.cpp
)

add_executable(su1 ${SU1_SOURCES})

target_link_libraries(su1
    Threads::Threads
)

if(MSVC)
    target_compile_options(su1 PRIVATE /O2 /DNDEBUG)
else()
    target_compile_options(su1 PRIVATE -O3 -march=native -mtune=native)
endif()
