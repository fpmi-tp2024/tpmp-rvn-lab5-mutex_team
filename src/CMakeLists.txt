add_library(parfume_bazaar_lib parfume_bazaar.cpp) #сюда добавлять новые cpp через пробел!!!
add_library(sqlite3 ../include/sql/sqlite3.c)
target_include_directories(parfume_bazaar_lib PUBLIC ../include)
target_link_libraries(parfume_bazaar_lib PUBLIC sqlite3)
# require at least c++ 20
target_compile_features(parfume_bazaar_lib PUBLIC cxx_std_20)