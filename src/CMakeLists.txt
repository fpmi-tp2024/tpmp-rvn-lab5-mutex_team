add_library(parfume_bazaar_lib parfume_bazaar.cpp
        date.cpp
        admin.cpp
        bargain.cpp
        item.cpp
        makler.cpp) #сюда добавлять новые cpp через пробел!!!
add_library(sqlite3 ../ext/sql/sqlite3.c)
target_include_directories(parfume_bazaar_lib PUBLIC ../include)
target_include_directories(parfume_bazaar_lib PUBLIC ../ext)
target_link_libraries(parfume_bazaar_lib PUBLIC sqlite3)
# require at least c++ 20
target_compile_features(parfume_bazaar_lib PUBLIC cxx_std_20)