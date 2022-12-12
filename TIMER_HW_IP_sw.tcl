# Creates new driver named timer_driver:
create_driver timer_driver

# Associates new driver with hardware named TIMER_HW_IP:
set_sw_property hw_class_name TIMER_HW_IP

# Sets driver version to 1 (i.e. first version):
set_sw_property version 1

# Sets minimum compatile hardware version to 1.0 (i.e. same as current):
set_sw_property min_compatible_hw_version 1.0

# Creates new BSP subdirectory named drivers:
add_sw_property bsp_subdirectory drivers

# Includes timer.c in source directory:
add_sw_property c_source HAL/src/timer.c

# Includes timer.h in include directory:
add_sw_property include_source HAL/inc/timer.h

# Sets driver to support HAL type:
add_sw_property supported_bsp_type HAL