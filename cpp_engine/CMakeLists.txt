# Set the minimum required version of CMake to 3.18
cmake_minimum_required(VERSION 3.18)

# Set the project name
project(cpp_engine)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force CMake to use the Python executable from our virtual environment
set(Python3_EXECUTABLE "${CMAKE_SOURCE_DIR}/../venv/Scripts/python.exe")

# Find both the Python interpreter AND the development libraries
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

# Ask Python where to find the pybind11 CMake files
execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import pybind11; print(pybind11.get_cmake_dir())"
    OUTPUT_VARIABLE pybind11_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Find the pybind11 package. It will now use the path we just found.
find_package(pybind11 REQUIRED)

# This is the core command from pybind11. It creates a Python module
pybind11_add_module(cpp_engine src/engine.cpp)