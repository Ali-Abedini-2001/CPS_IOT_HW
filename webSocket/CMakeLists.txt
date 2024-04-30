cmake_minimum_required(VERSION 3.14)

project(SimpleWebSocket LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core WebSockets HttpServer)

add_executable(SimpleWebSocket
  main.cpp
  websocketserver.h websocketserver.cpp
  request.h request.cpp
  response.h response.cpp
  employee.h employee.cpp
  systemadmin.h systemadmin.cpp
  systemadminshandler.h systemadminshandler.cpp
  employeeshandler.h employeeshandler.cpp
  loginhistory.h loginhistory.cpp
  loginhistoryhandler.h loginhistoryhandler.cpp
  httpserver.h httpserver.cpp


)
target_link_libraries(SimpleWebSocket Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets Qt${QT_VERSION_MAJOR}::HttpServer)

include(GNUInstallDirs)
install(TARGETS SimpleWebSocket
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
