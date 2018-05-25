LOCAL_PATH := $(call my-dir)/../bullet3-2.87
PHYVR_SRC := $(call my-dir)/../app/src/main
GLM_PATH := $(call my-dir)/../glm

include $(CLEAR_VARS)

LOCAL_CFLAGS := $(LOCAL_C_INCLUDES:%=-I%) -DUSE_PTHREADS -mfpu=neon -mfloat-abi=softfp -pthread -DSCE_PFX_USE_SIMD_VECTORMATH

# apply these flags if needed
# -ffast-math -funsafe-math-optimizations

# apply this to disable optimization
TARGET_CFLAGS += -O3

# use c++11 and fix stof etc.
TARGET_CFLAGS += -std=c++11

# link openGLES 2.0
LOCAL_LDLIBS    := -lGLESv2 -ldl -llog -landroid

# apply these 2 to turn on assembly output (*.c/*.cpp to *.s file)
#compile-cpp-source = $(eval $(call ev-compile-cpp-source,$1,$(1:%$(LOCAL_CPP_EXTENSION)=%.s)))
#TARGET_CFLAGS := $(TARGET_CFLAGS) -S

# Enable or disable NEON. Don't forget to apply, or not apply, -mfpu=neon and -mfloat-abi=softfp
# flags in addition, e.g., if this is true both of those need to be included in LOCAL_CFLAGS
# to avoid the possibility that ndk-build will "forget" to add them on some files
LOCAL_ARM_NEON := true

TARGET_CFLAGS := $(filter-out -ffpu=vfp,$(TARGET_CFLAGS))

# setup to build static library
LOCAL_MODULE := bullet

LOCAL_C_INCLUDES :=  $(LOCAL_PATH)/src

# add glm
LOCAL_C_INCLUDES += $(GLM_PATH)

#find all the file recursively under jni/
FILE_LIST := $(wildcard \
		$(LOCAL_PATH)/src/LinearMath/*.cpp \
		$(LOCAL_PATH)/src/Bullet3Common/*.cpp \
		$(LOCAL_PATH)/src/BulletCollision/BroadphaseCollision/*.cpp \
		$(LOCAL_PATH)/src/BulletCollision/CollisionDispatch/*.cpp \
		$(LOCAL_PATH)/src/BulletCollision/CollisionShapes/*.cpp \
		$(LOCAL_PATH)/src/BulletCollision/NarrowPhaseCollision/*.cpp \
		$(LOCAL_PATH)/src/BulletDynamics/ConstraintSolver/*.cpp \
		$(LOCAL_PATH)/src/BulletDynamics/Dynamics/*.cpp \
		$(LOCAL_PATH)/src/BulletDynamics/Featherstone/*.cpp \
		$(LOCAL_PATH)/src/BulletDynamics/MLCPSolvers/*.cpp \
		$(LOCAL_PATH)/src/BulletDynamics/Vehicle/*.cpp \
		$(LOCAL_PATH)/src/BulletDynamics/Character/*.cpp \
		$(LOCAL_PATH)/src/BulletSoftBody/*.cpp \
		$(LOCAL_PATH)/src/BulletInverseDynamics/*.cpp \
		$(LOCAL_PATH)/src/BulletInverseDynamics/details/*.cpp \
		$(PHYVR_SRC)/cpp/*.cpp \
		$(PHYVR_SRC)/cpp/utils/*.cpp \
		$(PHYVR_SRC)/cpp/level/*.cpp \
		$(PHYVR_SRC)/cpp/drawable/*.cpp \
		)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_SHARED_LIBRARY)