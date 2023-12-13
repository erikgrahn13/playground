import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout

class TestRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("gtest/1.14.0")

    def build_requirements(self):
        self.tool_requires("cmake/3.27.9")

    def layout(self):
        cmake_layout(self)
        # multi = True if self.settings.get_safe("compiler") == "msvc" else False
        # if multi:
        #     self.folders.generators = os.path.join("build", "generators")
        #     self.folders.build = "build"
        # else:
        #     self.folders.generators = os.path.join("build", str(self.settings.build_type), "generators")
        #     self.folders.build = os.path.join("build", str(self.settings.build_type))
