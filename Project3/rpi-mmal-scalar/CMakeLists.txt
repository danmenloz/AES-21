cmake_minimum_required(VERSION 2.8)

SET(COMPILE_DEFINITIONS -Werror)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(/opt/vc/include)
include_directories(/opt/vc/include/interface/vcos/pthreads)
include_directories(/opt/vc/include/interface/vmcs_host)
include_directories(/opt/vc/include/interface/vmcs_host/linux)

include_directories(/home/pi/src/raspberrypi/userland)
include_directories(/opt/vc/src/hello_pi/libs/vgfont)
include_directories(.)
include_directories(./src)

link_directories(/opt/vc/lib)
link_directories(/opt/vc/src/hello_pi/libs/vgfont)

# add_executable(mmaldemo main.c)
# add_executable(mmal_buffer_demo buffer_demo.c)
# add_executable(mmal_buffer_demo2 buffer_demo2.c)
add_executable(istool1 keyboard.c is_options.c buffer_demo_base.c yuv.c video_callback.c sccb_bus.c PCA9685_servo_driver.c bcm283x_board_driver.c)

# target_compile_options(mmal_buffer_demo PRIVATE "-O3")
# target_compile_options(mmal_buffer_demo2 PRIVATE "-O3")
target_compile_options(istool1 PRIVATE "-O3" "-g")

# target_link_libraries(mmal_buffer_demo mmal_core mmal_util mmal_vc_client vcos bcm_host rt)
# target_link_libraries(mmal_buffer_demo2 mmal_core mmal_util mmal_vc_client vcos bcm_host rt)
target_link_libraries(istool1 m mmal_core mmal_util mmal_vc_client vcos bcm_host rt)
