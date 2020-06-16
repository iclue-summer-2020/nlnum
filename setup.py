from setuptools import setup
try:
    import pybind11_cmake
except ImportError:
    print("pybind11-cmake must be installed."
          "Try \n \t pip install pybind11_cmake")
    import sys
    sys.exit()

from pybind11_cmake import CMakeExtension, CMakeBuild

setup(
    name='nlcalc',
    version='0.0.1',
    author='ICLUE @ UIUC',
    author_email='',
    description='',
    long_description='',
    setup_requires=['pybind11_cmake'],
    ext_modules=[CMakeExtension('nlcalc')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
)