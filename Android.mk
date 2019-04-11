# Copyright 2013 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH:= $(call my-dir)



############################
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	main.cpp \
	VulkanMain.cpp \
	vulkan_wrapper.cpp \

LOCAL_STATIC_LIBRARIES := \
	libstagefright_color_conversion
	
LOCAL_SHARED_LIBRARIES := \
	libcutils \
	libutils \
	libbinder \
    libui \
    libgui \
	libstagefright\
	libstagefright_foundation \
	libvulkan \
	
LOCAL_C_INCLUDES := \
	frameworks/native/include/media/openmax \
    frameworks/av/media/libstagefright \
	frameworks/native/vulkan \
	frameworks/native/libs/nativewindow/include \
#	external/vulkan-validation-layers/common \

LOCAL_CFLAGS += -Wno-error=unused-parameter

LOCAL_CFLAGS += -DVK_USE_PLATFORM_ANDROID_KHR \
    			-DVK_NO_PROTOTYPES
	
LOCAL_MODULE:= showRGBA

LOCAL_MODULE_TAGS := tests

include $(BUILD_EXECUTABLE)
