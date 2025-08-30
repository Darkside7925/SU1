# SU1 Display Server Examples

# Demo Display Server Application
add_executable(su1_demo_display_server
    demo_display_server.cpp
)

target_link_libraries(su1_demo_display_server
    PRIVATE
    su1_core
    su1_display_manager
    su1_window_manager
    su1_ui_components
    su1_theming
    su1_renderer
    ${SU1_PLATFORM_LIBS}
)

target_include_directories(su1_demo_display_server
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Set properties for the demo
set_target_properties(su1_demo_display_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Add compile definitions for the demo
target_compile_definitions(su1_demo_display_server
    PRIVATE
    SU1_DEMO_BUILD=1
    $<$<CONFIG:Debug>:SU1_DEBUG=1>
    $<$<CONFIG:Release>:SU1_RELEASE=1>
)

# Platform-specific settings
if(WIN32)
    target_compile_definitions(su1_demo_display_server
        PRIVATE
        SU1_PLATFORM_WINDOWS=1
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
elseif(UNIX)
    target_compile_definitions(su1_demo_display_server
        PRIVATE
        SU1_PLATFORM_LINUX=1
    )
    
    # Link additional Linux libraries
    target_link_libraries(su1_demo_display_server
        PRIVATE
        pthread
        dl
    )
endif()

# Install the demo
install(TARGETS su1_demo_display_server
    RUNTIME DESTINATION bin
    COMPONENT examples
)

# Copy demo data files if they exist
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data)
    install(DIRECTORY data/
        DESTINATION share/su1/examples/
        COMPONENT examples
    )
endif()

# Add a custom target to run the demo
add_custom_target(run_demo
    COMMAND su1_demo_display_server
    DEPENDS su1_demo_display_server
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMENT "Running SU1 Display Server Demo"
)

# Documentation target for the demo
if(DOXYGEN_FOUND)
    set(DEMO_DOCS_INPUT 
        ${CMAKE_CURRENT_SOURCE_DIR}/demo_display_server.cpp
        ${CMAKE_SOURCE_DIR}/include/su1/
    )
    
    configure_file(
        ${CMAKE_SOURCE_DIR}/docs/Doxyfile.demo.in
        ${CMAKE_BINARY_DIR}/docs/Doxyfile.demo
        @ONLY
    )
    
    add_custom_target(demo_docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/docs/Doxyfile.demo
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/docs
        COMMENT "Generating demo documentation with Doxygen"
        VERBATIM
    )
endif()

# Testing target for the demo
if(SU1_BUILD_TESTS)
    add_test(
        NAME demo_display_server_quick_test
        COMMAND su1_demo_display_server --quick-test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    
    set_tests_properties(demo_display_server_quick_test PROPERTIES
        TIMEOUT 60
        LABELS "demo;integration"
    )
endif()

# Packaging information
set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "SU1 Display Server Examples")
set(CPACK_COMPONENT_EXAMPLES_DESCRIPTION "Example applications demonstrating SU1 Display Server capabilities")
set(CPACK_COMPONENT_EXAMPLES_DEPENDS core)
