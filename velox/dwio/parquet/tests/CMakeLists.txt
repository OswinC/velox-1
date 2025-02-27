# Copyright (c) Facebook, Inc. and its affiliates.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# for generated headers
include_directories(${CMAKE_BINARY_DIR})

set(VELOX_LINK_LIBS
    velox_dwio_common
    velox_dwio_common_exception
    velox_dwio_parquet_reader
    velox_dwio_type_fbhive
    velox_dwio_type_fbhive
    velox_dwrf_test_utils
    velox_vector
    velox_exception
    velox_memory
    velox_process
    velox_serialization
    velox_encode
    velox_type
    velox_caching)

set(TEST_LINK_LIBS ${gflags_LIBRARIES} ${GTEST_BOTH_LIBRARIES} ${GMock} ${GLOG}
                   ${FILESYSTEM})

add_executable(velox_dwio_parquet_reader_test TestReader.cpp)
add_test(
  NAME velox_dwio_parquet_reader_test
  COMMAND velox_dwio_parquet_reader_test
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  velox_dwio_parquet_reader_test
  velox_vector_test_lib
  ${VELOX_LINK_LIBS}
  ${FOLLY_WITH_DEPENDENCIES}
  ${FMT}
  ${LZ4}
  ${LZO}
  ${ZSTD}
  ${ZLIB_LIBRARIES}
  ${TEST_LINK_LIBS})
