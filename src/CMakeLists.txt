
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

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
