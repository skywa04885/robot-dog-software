project(
    'driver',
    'cpp',
    version: '0.1',
    default_options: ['cpp_std=c++17']
)

eigen = dependency('eigen3')
rplidar = static_library('rplidar', 'vendor/sdk/sdk/output/Linux/Release/librplidar_sdk.a')

executable(
    'main',
    [
        'source/main.cc'
    ],
    install: false,
    dependencies: [
        eigen
    ],
    link_with: rplidar,
    include_directories: [
        'include',
        'vendor/sdk/sdk/sdk/include',
        'vendor/sdk/sdk/sdk/src'
    ]
)