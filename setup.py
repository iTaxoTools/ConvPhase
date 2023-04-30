"""A setuptools based setup module."""

# Always prefer setuptools over distutils
from setuptools import setup, find_namespace_packages
from pathlib import Path

# Get the long description from the README file
here = Path(__file__).parent.resolve()
long_description = (here / 'README.md').read_text(encoding='utf-8')


setup(
    name='convphase',
    version='0.0.1',
    description='A Python package for ConvPhase',
    long_description=long_description,
    long_description_content_type='text/markdown',
    package_dir={'': 'src'},
    packages=find_namespace_packages(
        include=('itaxotools*',),
        where='src',
    ),
    python_requires='>=3.10.2, <4',
    install_requires=[],
    extras_require={},
    entry_points={
        'console_scripts': [
            'convphase = itaxotools.convphase:run',
        ],
    },
    classifiers=[
        'License :: OSI Approved :: GNU General Public License v3 or later (GPLv3+)',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.10',
        'Programming Language :: Python :: 3 :: Only',
    ],
    include_package_data=True,
)
