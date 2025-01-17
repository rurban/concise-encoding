project(
  'cte',
  'c',
  version : '1.0.0',
  default_options : ['warning_level=3']
)
project_description = 'Concise Binary Encoding'

project_headers = [
  'include/cte/cte.h',
]

ragel = find_program('ragel')

ragel_gen = generator(
  ragel,
  output : '@BASENAME@.c',
  arguments : ['-o', '@OUTPUT@', '@INPUT@'],
)

gen_src = ragel_gen.process('src/decoder.rl.c')

project_source_files = [
  'src/encoder.c',
  'src/library.c',
  gen_src,
]

project_test_files = [
  'tests/src/helpers/cte_decoder.cpp',
  'tests/src/helpers/cte_encoder.cpp',
  'tests/src/helpers/encoding.cpp',
  'tests/src/helpers/test_helpers.cpp',
  'tests/src/helpers/test_utils.cpp',
  'tests/src/binary.cpp',
  'tests/src/comment.cpp',
  'tests/src/string.cpp',
]

project_test_decfloat_files = [
  'tests/src/general.cpp',
]

cc = meson.get_compiler('c')

project_dependencies = [
  dependency('smalltime', fallback : ['smalltime', 'smalltime_dep']),
  dependency('safe16', fallback : ['safe16', 'safe16_dep']),
  dependency('safe85', fallback : ['safe85', 'safe85_dep']),
  cc.find_library('quadmath', required : false),
]

build_args = [
]


# ===================================================================

# ======
# Target
# ======

public_headers = include_directories('include')
private_headers = include_directories('src')

build_args += [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
]

# Only make public interfaces visible
if target_machine.system() == 'windows' or target_machine.system() == 'cygwin'
  build_args += '-DCTE_PUBLIC="__declspec(dllexport)"'
else
  build_args += '-DCTE_PUBLIC=__attribute__((visibility("default")))'
endif

project_target = shared_library(
  meson.project_name(),
  project_source_files,
  install : true,
  c_args : build_args,
  gnu_symbol_visibility : 'hidden',
  include_directories : [public_headers, private_headers],
  dependencies: project_dependencies,
)


# =======
# Project
# =======

# Make this library usable as a Meson subproject.
project_dep = declare_dependency(
  include_directories: public_headers,
  dependencies: project_dependencies,
  link_with : project_target
)
set_variable(meson.project_name() + '_dep', project_dep)

# Make this library usable from the system's
# package manager.
install_headers(project_headers, subdir : meson.project_name())

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : meson.project_name(),
  filebase : meson.project_name(),
  description : project_description,
  subdirs : meson.project_name(),
  libraries : project_target,
)


# ==========
# Unit Tests
# ==========

if not meson.is_subproject()
  add_languages('cpp')
  subdir('tests')

  decfloat_lib = static_library('decfloat_lib',
    project_test_decfloat_files,
    dependencies : [project_dep, test_dep],
    include_directories : private_headers,
    cpp_args : ['-Wno-pedantic'],
  )

  test('all_tests',
    executable(
      'run_tests',
      files(project_test_files),
      dependencies : [project_dep, test_dep],
      install : false,
      include_directories : private_headers,
      link_with : decfloat_lib,
    )
  )
endif
