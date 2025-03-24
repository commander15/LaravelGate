qt_add_library(LaravelGate)

set_target_properties(LaravelGate PROPERTIES
    OUTPUT_NAME LaravelGate
    VERSION     ${PROJECT_VERSION}
)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(LaravelGate
    PUBLIC
        laravelgate.h
        global.h config.h
        logincontroller.h
        datacontroller.h
    PRIVATE
        debug_p.h
        logincontroller_p.h
)

target_sources(LaravelGate
    PRIVATE
        debug.cpp
        logincontroller.cpp
        datacontroller.cpp
)

target_include_directories(LaravelGate 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/LaravelGate>
)

target_compile_definitions(LaravelGate
    PUBLIC
        $<IF:$<STREQUAL:$<TARGET_PROPERTY:TYPE>,STATIC_LIBRARY>,LARAVELGATE_STATIC,LARAVELGATE_SHARED>
    INTERFACE
        LARAVELGATE_LIB
    PRIVATE
        LARAVELGATE_BUILD
)

target_link_libraries(LaravelGate PUBLIC DataGate::DataGate Jsoner::Jsoner RestLink::RestLink)

install(TARGETS LaravelGate
    EXPORT LaravelGateTargets
    ARCHIVE        DESTINATION lib
    LIBRARY        DESTINATION lib
    RUNTIME        DESTINATION bin
    PUBLIC_HEADER  DESTINATION include/LaravelGate
    #PRIVATE_HEADER DESTINATION include/LaravelGate/private
)

generate_target_headers(LaravelGate FOLDER LaravelGate)
