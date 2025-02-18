cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(dump C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED WIFI_SSID)
    message(FATAL_ERROR "add -DWIFI_SSID=SSID to cmake.")
endif()

if (NOT DEFINED WIFI_PASSWD)
    message(FATAL_ERROR "add -DWIFI_PASSWD=PASS to cmake.")
endif()

pico_sdk_init()

add_subdirectory(pico-ultrasonic-lib)

add_executable(dump
    mqtt_client.c
)

pico_enable_stdio_usb(dump 1)
pico_enable_stdio_uart(dump 0)
pico_add_extra_outputs(dump)

target_include_directories(dump PRIVATE ${CMAKE_CURRENT_LIST_DIR}/lwip)

target_compile_definitions(dump PRIVATE
    WIFI_SSID=\"${WIFI_SSID}\"
    WIFI_PASSWD=\"${WIFI_PASSWD}\"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${PICO_SDK_PATH}/lib/lwip/src/apps/mqtt/mqtt.c)

target_link_libraries(dump
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_ultrasonic_lib
)
