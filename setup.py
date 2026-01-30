from setuptools import Extension, setup

ext_modules = [
    Extension(
        "tiny_haversine._haversine",
        sources=["src/tiny_haversine/_haversine.c"],
        extra_compile_args=["-O3"],
    )
]

setup(ext_modules=ext_modules)
