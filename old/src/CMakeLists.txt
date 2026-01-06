file(GLOB_RECURSE c_files "*.c")
file(GLOB_RECURSE cpp_files "*.cpp")
addlib(lumos ${c_files} ${cpp_files})
tgtlib(lumos gmp gmpxx)
