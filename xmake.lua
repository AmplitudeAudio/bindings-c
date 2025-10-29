-- Copyright (c) 2025-present Sparky Studios. All rights reserved.
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

-- Project definition
set_project("Amplitude")
set_version("1.0.0")
set_license("Apache-2.0")
set_languages("c++20")
set_description("A powerful and cross-platform audio engine, optimized for games.")
set_xmakever("3.0.0")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

add_requireconfs("*", { debug = is_mode("debug") })
add_requires("amplitudeaudiosdk fix/release-stabilization")

target("amplitude_c")
  set_kind("shared")
  add_files("src/**.cpp")
  add_includedirs("include")
  add_packages("amplitudeaudiosdk")
  add_defines("AM_BUILDSYSTEM_BUILDING_AMPLITUDE")

  add_headerfiles("include/**.h")
target_end()
