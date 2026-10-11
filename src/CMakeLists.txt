
qt_add_executable(${PROJECT_NAME}
    WIN32 MACOSX_BUNDLE
    main.cpp
    maindialog.cpp
    maindialog.hpp
    maindialog.ui
    tray.cpp
    tray.hpp
    linksmenu.cpp
    linksmenu.hpp
    settings.cpp
    settings.hpp
    sortproxy.hpp
    styles.cpp
    styles.hpp
    gui/appproxystyle.cpp
    gui/appproxystyle.hpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt::Core
        Qt::Widgets
)


target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_NAME="${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_VERSION="${PROJECT_VERSION}")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_FULL_VERSION="${PROJECT_FULL_VERSION}")
target_compile_definitions(${PROJECT_NAME} PRIVATE APPLICATION_NAME="$<IF:$<CONFIG:Debug>,${PROJECT_NAME}_dbg,${PROJECT_NAME}>")
target_compile_definitions(${PROJECT_NAME} PRIVATE ORGANIZATION_NAME="${ORGANIZATION_NAME}")
target_compile_definitions(${PROJECT_NAME} PRIVATE ORGANIZATION_DOMAIN="${ORGANIZATION_DOMAIN}")


install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

set(DEPLOY_OPTIONS "
--no-system-d3d-compiler
--no-system-dxc-compiler
--no-opengl-sw
--no-network
--skip-plugin-types imageformats,iconengines
")
# Install everything under bin dir next to the executable
qt6_generate_deploy_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    CONTENT "
qt_deploy_runtime_dependencies(
    EXECUTABLE \"\${QT_DEPLOY_BIN_DIR}/$<TARGET_FILE_NAME:${PROJECT_NAME}>\"
    BIN_DIR ${CMAKE_INSTALL_BINDIR}
    LIB_DIR ${CMAKE_INSTALL_BINDIR}
    PLUGINS_DIR ${CMAKE_INSTALL_BINDIR}
    NO_TRANSLATIONS
    DEPLOY_TOOL_OPTIONS ${DEPLOY_OPTIONS}
)")
install(SCRIPT ${deploy_script})
