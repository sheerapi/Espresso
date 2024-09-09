project('glad', ['c'], version: '20240811')

sources = [
    'egl.c',
    'gl.c',
    'vulkan.c'
]

include_directories = include_directories('.')

glad = library('glad', sources: sources)
glad_dep = declare_dependency(link_with: glad, include_directories: include_directories)

meson.override_dependency('glad', glad_dep)