cmake_minimum_required(VERSION 3.19)

project(SystrayLinks LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()


add_subdirectory(src)
