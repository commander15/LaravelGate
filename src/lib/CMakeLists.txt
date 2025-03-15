qt_add_library(LaravelGate)

configure_file(config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/config.h @ONLY)

target_headers(LaravelGate
    PUBLIC
        global.h config.h
        logincontroller.h
        datacontroller.h
)

target_sources(LaravelGate
    PRIVATE
        logincontroller.cpp
        datacontroller.cpp
)

target_include_directories(LaravelGate PUBLIC $<INSTALL_INTERFACE:include>)

target_link_libraries(LaravelGate PUBLIC DataGate::DataGate RestLink::RestLink)

install(TARGETS LaravelGate
    EXPORT LaravelGateTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION lib
)

generate_target_headers(LaravelGate FOLDER LaravelGate)
