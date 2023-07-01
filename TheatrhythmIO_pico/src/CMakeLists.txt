add_executable(TheatrhythmIO
        main.cpp
        usb_descriptors.c)

target_include_directories(TheatrhythmIO PRIVATE ${CMAKE_CURRENT_LIST_DIR})

#pico_generate_pio_header(TheatrhythmIO ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
target_sources(TheatrhythmIO PRIVATE main.cpp)

#pico_enable_stdio_uart(TheatrhythmIO 1)

target_link_libraries(TheatrhythmIO PRIVATE
        pico_multicore
        pico_stdlib
        tinyusb_device
        tinyusb_board)

pico_add_extra_outputs(TheatrhythmIO)