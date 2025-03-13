add_subdirectory(lib)

include(CMakePackageConfigHelpers)

set(ROOT .)

configure_package_config_file(LaravelGateConfig.cmake.in LaravelGateConfig.cmake
    PATH_VARS ROOT
    INSTALL_DESTINATION lib/cmake/LaravelGate
)

write_basic_package_version_file(LaravelGateConfigVersion.cmake
    VERSION ${LaravelGate_VERSION}
    COMPATIBILITY AnyNewerVersion
    ARCH_INDEPENDENT
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/LaravelGateConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/LaravelGateConfigVersion.cmake
    DESTINATION lib/cmake/LaravelGate
)

install(EXPORT LaravelGateTargets
    NAMESPACE LaravelGate::
    DESTINATION lib/cmake/LaravelGate
)
