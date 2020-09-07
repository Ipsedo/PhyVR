# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

set(GLM_PATH ${ROOT_PATH}/glm)
set(BULLET_PATH ${ROOT_PATH}/bullet3)
set(SOIL2_PATH ${ROOT_PATH}/SOIL2)
set(libs_dir ${ROOT_PATH}/app/build/libraries)

# Bullet 3
set(CMAKE_C_FLAGS "-DUSE_PTHREADS -mfpu=neon -mfloat-abi=softfp -pthread -DSCE_PFX_USE_SIMD_VECTORMATH")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -O3")

set(CMAKE_ANDROID_ARM_NEON "true")

list(FILTER CMAKE_C_FLAGS EXCLUDE REGEX "-ffpu=vfp")

file(GLOB BULLET_SRC
        ${BULLET_PATH}/src/LinearMath/*.cpp
        ${BULLET_PATH}/src/Bullet3Common/*.cpp
        ${BULLET_PATH}/src/BulletCollision/BroadphaseCollision/*.cpp
        ${BULLET_PATH}/src/BulletCollision/CollisionDispatch/*.cpp
        ${BULLET_PATH}/src/BulletCollision/CollisionShapes/*.cpp
        ${BULLET_PATH}/src/BulletCollision/NarrowPhaseCollision/*.cpp
        ${BULLET_PATH}/src/BulletDynamics/ConstraintSolver/*.cpp
        ${BULLET_PATH}/src/BulletDynamics/Dynamics/*.cpp
        ${BULLET_PATH}/src/BulletDynamics/Featherstone/*.cpp
        ${BULLET_PATH}/src/BulletDynamics/MLCPSolvers/*.cpp
        ${BULLET_PATH}/src/BulletDynamics/Vehicle/*.cpp
        ${BULLET_PATH}/src/BulletDynamics/Character/*.cpp
        ${BULLET_PATH}/src/BulletSoftBody/*.cpp
        ${BULLET_PATH}/src/BulletInverseDynamics/*.cpp
        ${BULLET_PATH}/src/BulletInverseDynamics/details/*.cpp
        )

include_directories(${BULLET_PATH}/src)

add_library(bullet3 STATIC ${BULLET_SRC})

# SOIL2
file(GLOB SOIL2_SRC ${SOIL2_PATH}/src/SOIL2/*.c)

include_directories(${SOIL2_PATH}/src)

add_library(soil2 STATIC ${SOIL2_SRC})

# PhyVR
file(GLOB_RECURSE PHYVR_SRC ./*.cpp)

add_library(phyvr SHARED ${PHYVR_SRC})

find_library(
        log-lib
        log
        GLESv2
        soil2
        bullet3
        android
)

target_link_libraries(
        phyvr
        android
        ${log-lib}
        GLESv2
        soil2
        bullet3
        ${libs_dir}/jni/${ANDROID_ABI}/libcardboard_api.so
)

# GLM
target_include_directories(phyvr PRIVATE ${libs_dir})
target_include_directories(phyvr PRIVATE ${GLM_PATH})
