# Conan

If you put [tool_requires] in the profile file all dependencies will also be built by the tools specified, otherwise the dependencies will be built by the system-wide CMake version for example.

This command will get the location of where Conan puts all it's files

`$ conan config home`

To build conan depenencies with release build:

`$ conan install . --output-folder=build --build=missing`

To build conan dependencies with debug build:

`$ conan install . --output-folder=build --build=missing --settings=build_type=Debug`

To use another profile with conan

`$ conan install . --output-folder=build --build=missing --profile=<name of profile file>`

If cmake_layout(self) is used in the conanfile.py this command can be runned to install conan dependencies
`$ conan install .  --build=missing`