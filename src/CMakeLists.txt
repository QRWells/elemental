add_executable(elemental)

target_sources(elemental PRIVATE
    main.cpp
)

target_include_directories(elemental PRIVATE
    ${PROJECT_SOURCE_DIR}/lib
)

target_link_libraries(elemental PRIVATE
    runtime
)

add_subdirectory(runtime)