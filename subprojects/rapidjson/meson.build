project('rapidjson', 'cpp', version: '1.1.0', license: 'MIT')

rapidjson_inc = include_directories('.')
rapidjson_dep = declare_dependency(include_directories: rapidjson_inc)

meson.override_dependency('rapidjson', rapidjson_dep)