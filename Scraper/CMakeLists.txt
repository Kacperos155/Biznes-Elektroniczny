cmake_minimum_required (VERSION 3.15)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(WIN32)
    set(VCPKG_TARGET_TRIPLET  "x64-windows-static" CACHE STRING "" FORCE)
    set(GUMBO_LIB gumbo.lib CACHE STRING "Gumbo library file")
endif()

if(UNIX)
    set(GUMBO_LIB libgumbo.a CACHE STRING "Gumbo library file")
endif()

message("Triplet: ${VCPKG_TARGET_TRIPLET}")

project ("Biznes Elektroniczny" CXX)

find_package(fmt REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

add_executable (Scraper
	"main.cpp"
	"Converter.h"
	"Converter.cpp"
	"Movie.h"
	"Scraper.h"
	"Scraper.cpp"
)

set_target_properties(Scraper PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    VS_DEBUGGER_COMMAND_ARGUMENTS  "-G -I -C"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

target_link_libraries(Scraper 
    PRIVATE
    cpr
    fmt::fmt
    nlohmann_json::nlohmann_json
    "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib/${GUMBO_LIB}"
)
