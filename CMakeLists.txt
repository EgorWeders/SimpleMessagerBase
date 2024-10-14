cmake_minimum_required(VERSION 3.14)

project(MessagerProj LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)

add_executable(TestMsgPing
	main.cpp
	spanmutator.h spanmutator.cpp
	base/message.h base/message.cpp
	base/client.h base/client.cpp
	network/udpclient.h network/udpclient.cpp
	base/mutator.h base/mutator.cpp
	base/idgenerator.h base/idgenerator.cpp
        base/mutatorloader.h base/mutatorloader.cpp

)
target_link_libraries(TestMsgPing Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network)
install(TARGETS TestMsgPing
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
