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

add_library(
  velox_dwio_dwrf_common
  BufferedInput.cpp
  ByteRLE.cpp
  CachedBufferedInput.cpp
  CacheInputStream.cpp
  Common.cpp
  Compression.cpp
  Config.cpp
  DataBufferHolder.cpp
  DecoderUtil.cpp
  Decryption.cpp
  DirectDecoder.cpp
  Encryption.cpp
  EncryptionSpecification.cpp
  InputStream.cpp
  IntDecoder.cpp
  IntEncoder.cpp
  OutputStream.cpp
  PagedInputStream.cpp
  PagedOutputStream.cpp
  RLEv1.cpp
  RLEv2.cpp
  Range.cpp
  Statistics.cpp
  wrap/dwrf-proto-wrapper.cpp)

target_link_libraries(
  velox_dwio_dwrf_common velox_common_base velox_dwio_common
  velox_dwio_common_compression velox_dwio_dwrf_proto velox_caching)
