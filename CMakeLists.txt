cmake_minimum_required(VERSION 3.19)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(VERSION_SUFFIX "alpha")
set(VERSION_SUFFIX_I 127) # alpha = 127

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

if(VERSION_SUFFIX)
    set(PROJECT_FULL_VERSION "${PROJECT_VERSION}-${VERSION_SUFFIX}")
else()
    set(PROJECT_FULL_VERSION "${PROJECT_VERSION}")
endif()
message("PROJECT_VERSION ${PROJECT_VERSION}")
message("PROJECT_FULL_VERSION ${PROJECT_FULL_VERSION}")

set(ORGANIZATION_NAME "llahteinen")
set(ORGANIZATION_DOMAIN "llahteinen.com") # NOTE: fake domain


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
project(SystrayLinks VERSION ${PROJECT_VERSION} LANGUAGES CXX)


if (MSVC)
#    add_compile_options($<$<CONFIG:Release>:/O1>) # O2 is MSVC default
    add_compile_options(/W4)     # Enable all normal warnings (Level 1–4)
#    add_compile_options(/Wall)   # /Wall gives warnings in moc generated files and other silly warnings
    add_compile_options(/WX)     # All compiler warnings as errors
    # Selectively enable some warnings or errors
    add_compile_options(/we5262) # switch case implicit fall-through
    # Selectively disable some warnings
    # /wd disables the warning. With MSVC there is no way to selectively treat something only as warning but not error :<
#    add_compile_options(/wd4996) # The [[deprecated]] attribute
else()
    add_compile_options(-Wall)
    add_compile_options(-Wextra)
    add_compile_options(-Wpedantic)
    add_compile_options(-Werror)
    # Selectively treat some warnings as warnings instead of errors
    add_compile_options(-Wno-error=deprecated-declarations) # The [[deprecated]] attribute
    add_compile_options(-Weffc++ -Wno-error=effc++)
endif()


if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install-${CMAKE_BUILD_TYPE}" CACHE PATH "Install prefix" FORCE)
endif()
message("CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()


add_subdirectory(src)
