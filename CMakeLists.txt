cmake_minimum_required(VERSION 3.16)
project(HinLIBS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Use Qt5 (the version installed on the course VM) ---
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

# --- Source files ---
set(SRC
    main.cpp
    models/User.cpp
    models/Patron.cpp
    models/Item.cpp
    models/Book.cpp
    models/Movie.cpp
    models/VideoGame.cpp
    models/Magazine.cpp
    models/LibrarySystem.cpp
    gui/LoginWindow.cpp
    gui/PatronWindow.cpp
    gui/CatalogueModel.cpp
)

# --- Header files ---
set(HDR
    models/User.h
    models/Patron.h
    models/Item.h
    models/Book.h
    models/Movie.h
    models/VideoGame.h
    models/Magazine.h
    models/LibrarySystem.h
    gui/LoginWindow.h
    gui/PatronWindow.h
    gui/CatalogueModel.h
)

# --- UI files (Qt Designer .ui forms) ---
set(UI
    gui/LoginWindow.ui
    gui/PatronWindow.ui
)

# --- Generate Qt UI code ---
qt5_wrap_ui(UI_HDRS ${UI})

# --- Create the executable ---
add_executable(${PROJECT_NAME} ${SRC} ${HDR} ${UI_HDRS})

# --- Link against Qt5 libraries ---
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
