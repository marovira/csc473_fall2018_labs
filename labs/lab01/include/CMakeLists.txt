set(INCLUDE_LIST
    "${LAB_INCLUDE_ROOT}/PlanetScene.hpp"
    "${LAB_INCLUDE_ROOT}/Planet.hpp"
    )

set(PATH_INCLUDE "${LAB_INCLUDE_ROOT}/Paths.hpp")
configure_file("${LAB_INCLUDE_ROOT}/Paths.hpp.in" ${PATH_INCLUDE})

set(LAB_INCLUDE_LIST
    ${INCLUDE_LIST}
    ${PATH_INCLUDE}
    PARENT_SCOPE)
