cmake_minimum_required(VERSION 3.12)

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(gcwireless_bridge CXX C ASM)

pico_sdk_init()

include_directories(
	include
	)

add_executable(gcwireless_bridge
	src/joybus.cpp
	src/main.cpp
	)

pico_generate_pio_header(project ${CMAKE_CURRENT_LIST_DIR}/pio/joybus.pio)

add_custom_command(OUTPUT ${CMAKE_CURRENT_LIST_DIR}/generated/joybus.pio.h
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/joybus.pio
        COMMAND Pioasm ${CMAKE_CURRENT_LIST_DIR}/joybus.pio ${CMAKE_CURRENT_LIST_DIR}/generated/joybus.pio.h
        )

target_link_libraries(gcwireless_bridge
	PRIVATE
	pico_stdlib
	# pico_bootrom
	# pico_multicore
	pico_platform
	# hardware_adc
	# hardware_dma
	# hardware_flash
	hardware_pio
	# hardware_pwm
	hardware_spi
	# hardware_sync
	# hardware_timer
)

pico_add_extra_outputs(gcwireless_bridge)