cmake_minimum_required(VERSION 3.29)

if (DEFINED ENV{USE_VCPKG_TOOLCHAIN} AND "$ENV{USE_VCPKG_TOOLCHAIN}" STREQUAL "1")
    set(CMAKE_TOOLCHAIN_FILE "D:/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

project(join_server VERSION 1.0.0)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.66.0 REQUIRED COMPONENTS system)

add_executable(join_server 
main.cpp
)

set_target_properties(join_server PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(join_server PRIVATE
    Boost::system
)

if (MSVC)
    target_compile_options(join_server PRIVATE /W4)
else ()
    target_compile_options(join_server PRIVATE -Wall -Wextra -pedantic) 
endif()

#install(TARGETS join_server RUNTIME DESTINATION bin)
# set(CPACK_GENERATOR DEB)
# set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
# set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
# set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
# set(CPACK_PACKAGE_CONTACT example@example.com)
# include(CPack)




