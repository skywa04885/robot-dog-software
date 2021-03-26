project('gui', 'cpp')

cpp_comp = meson.get_compiler('cpp')

sources = [
    'main.cc'
]

subdir ('src')

executable(
    'gui', 
    sources,
    dependencies: [
        dependency('gtk+-3.0'),
        dependency('threads')
    ],
    include_directories: [
        '/usr/include/panda3d',
        'inc'
    ]
)