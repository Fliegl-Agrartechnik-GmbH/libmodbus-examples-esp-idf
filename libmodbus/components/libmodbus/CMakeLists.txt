# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS                # list the source files of this component
        "src/modbus/modbus.c"
        "src/modbus/modbus-data.c"
        "src/modbus/modbus-rtu.c"
        "src/modbus/modbus-tcp.c"
    INCLUDE_DIRS        # optional, add here public include directories
        "include/modbus"
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
        "priv_include/modbus"
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
    	driver
)
