# Ustaw minimalną wymaganą wersję CMake.
# 3.15 to bezpieczny, nowoczesny wybór.
cmake_minimum_required(VERSION 3.15)

# Nazwij swój projekt. Ta nazwa pojawi się w VS Code.
project(MojPierwszyProgram)

# To jest najważniejsza linia:
# Mówi CMake, aby stworzył program wykonywalny (exe)
# o nazwie "HelloWorld" na podstawie pliku "test.cpp".
add_executable(HelloWorld test.cpp)