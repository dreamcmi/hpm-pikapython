sdk_inc(tf_core)
sdk_inc(third_party/flatbuffers/include)
sdk_inc(third_party/gemmlowp)
sdk_inc(third_party/ruy)
sdk_inc(third_party/kissfft)
add_subdirectory(tf_core/tensorflow/lite)
add_subdirectory(third_party/kissfft)