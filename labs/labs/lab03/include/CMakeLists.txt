set(INCLUDE_LIST
    "${LAB_INCLUDE_ROOT}/TrackScene.hpp"
    "${LAB_INCLUDE_ROOT}/TrackBall.hpp"
    "${LAB_INCLUDE_ROOT}/Spline.hpp"
    )

set(PATH_INCLUDE "${LAB_INCLUDE_ROOT}/Paths.hpp")
configure_file("${LAB_INCLUDE_ROOT}/Paths.hpp.in" ${PATH_INCLUDE})

set(LAB_INCLUDE_LIST
    ${INCLUDE_LIST}
    ${PATH_INCLUDE}
    PARENT_SCOPE)
