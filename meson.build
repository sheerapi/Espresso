project(
    'espresso',
    'cpp',
    version: '0.0.1',
    default_options: ['cpp_std=c++20'],
)

source = [
    'espresso/components/Camera.cpp',
    'espresso/components/Transform.cpp',

    'espresso/core/Application.cpp',
    'espresso/core/EventManager.cpp',
    'espresso/core/Window.cpp',
    'espresso/core/Entity.cpp',

    'espresso/utils/threading/Scheduler.cpp',
    'espresso/utils/threading/TickSystem.cpp',

    'espresso/utils/assets/AssetManager.cpp',
    'espresso/utils/assets/Asset.cpp',

    'espresso/utils/math/Math.cpp',
    'espresso/utils/EventHandler.cpp',
    'espresso/utils/StringUtils.cpp',
    'espresso/utils/threading/System.cpp'
]

cfg_data = configuration_data()

if get_option('debug')
    cfg_data.set10('DEBUG', true)
endif

configure_file(output: 'config.h', configuration: cfg_data)

dependencies = [
    dependency(
        'spdlog',
        default_options: {'compile_library': true, 'std_format': 'enabled'},
    ),
    dependency('sdl2', default_options: { 'use_audio': 'disabled', 'use_file': 'disabled', 'use_locale': 'disabled', 'use_render': 'disabled', 'use_power': 'disabled', 'use_filesystem': 'disabled', 'test': false }),
    dependency('box2d'),
    dependency('imgui', default_options: { 'dx9': 'disabled', 'dx10': 'disabled', 'dx11': 'disabled', 'dx12': 'disabled', 'metal': 'disabled', 'opengl': 'disabled', 'vulkan': 'disabled', 'sdl_renderer': 'disabled', 'webgpu': 'disabled', 'glfw': 'disabled', 'sdl2': 'disabled', 'osx': 'disabled', 'win': 'disabled', 'allegro5': 'disabled'}),
    dependency('rapidjson'),
    dependency('luajit'),
    dependency('magic_enum'),
    dependency('zlib'),
    dependency('openal'),
    dependency('glm'),
    dependency('glad')
]

inc_dirs = include_directories('espresso')

espresso = library(
    'espresso',
    sources: source,
    include_directories: inc_dirs,
    dependencies: dependencies,
)

espresso_dep = declare_dependency(
    link_with: espresso,
    dependencies: dependencies,
    include_directories: inc_dirs,
)

executable('sandbox', sources: 'sandbox/main.cpp', dependencies: espresso_dep)