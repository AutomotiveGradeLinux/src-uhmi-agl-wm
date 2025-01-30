# SPDX-License-Identifier: Apache-2.0
#
# Copyright @ 2025, Collabora, Ltd.
project('uhmi-agl-wm',
  'c', 'cpp',
  version: '1.0.0',
  default_options: [
    'warning_level=3',
    'c_std=c2x',
    'cpp_std=c++20',
    'werror=true',
  ],
  meson_version: '>= 1.0.0',
  license: 'Apache-2.0',
)

pkgconfig = import('pkgconfig')
cpp = meson.get_compiler('cpp')

grpcpp_reflection_dep = cpp.find_library('grpc++_reflection')
protoc = find_program('protoc')
grpc_cpp = find_program('grpc_cpp_plugin')

protoc_gen = generator(protoc, \
                       output : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
                       arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/app',
                         '--cpp_out=@BUILD_DIR@',
                         '@INPUT@'])

generated_protoc_sources = protoc_gen.process('app/agl_shell.proto')

grpc_gen = generator(protoc, \
                     output : ['@BASENAME@.grpc.pb.cc', '@BASENAME@.grpc.pb.h'],
                     arguments : ['--proto_path=@CURRENT_SOURCE_DIR@/app',
                       '--grpc_out=@BUILD_DIR@',
                       '--plugin=protoc-gen-grpc=' + grpc_cpp.path(),
                       '@INPUT@'])

generated_grpc_sources = grpc_gen.process('app/agl_shell.proto')

grpc_deps = [
    dependency('protobuf'),
    dependency('grpc'),
    dependency('grpc++'),
    grpcpp_reflection_dep,
]

prefix_path = get_option('prefix')
binplugin_dir = join_paths(prefix_path, get_option('bindir'))

deps_uhmi_agl_wm = [
    grpc_deps,
    dependency('jansson')
]

srcs_uhmi_agl_wm = [
    'app/main.c',
    'app/comm_parser.c',
    'app/comm_receiver.c',
    'app/AglShellGrpcClient.cpp',
    'app/AglShellGrpcClient.h',
    generated_protoc_sources,
    generated_grpc_sources,
]

exe = executable(
    'uhmi-agl-wm',
    srcs_uhmi_agl_wm,
    dependencies: deps_uhmi_agl_wm,
    install_rpath: binplugin_dir,
    install: true
)
