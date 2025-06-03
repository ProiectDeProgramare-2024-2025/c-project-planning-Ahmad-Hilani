cmake_minimum_required(VERSION 3.30)
project(BanatBanking)

set(CMAKE_CXX_STANDARD 20)

add_executable(BanatBanking main.cpp
        BankAccount.cpp
        BankAccount.h
        BankUtils.cpp
        BankUtils.h
)
set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++"
)
