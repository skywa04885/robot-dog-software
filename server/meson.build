project('gui', 'cpp')

cpp_comp = meson.get_compiler('c')

sources = [
    'main.c',
    '../shared/source/ssfp.c'
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
        'inc',
        '../shared/include'
    ]
)