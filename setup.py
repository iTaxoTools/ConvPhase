"""A setuptools based setup module."""

# Always prefer setuptools over distutils
from setuptools import setup, find_namespace_packages, Command, msvc
from subprocess import check_call
from pathlib import Path
import os

# Get the long description from the README file
here = Path(__file__).parent.resolve()
long_description = (here / 'README.md').read_text(encoding='utf-8')


class BuildConvPhase(Command):
    """Custom command for building ConvPhase"""
    description = 'compile convphase using premake'
    user_options = []

    def initialize_options(self):
        self.name = 'itaxotools.mafftpy.mafft'
        self.git_submodules = [
            'phase',
            'SeqPHASE',
        ]
        self.plat_name = None

    def finalize_options(self):
        self.set_undefined_options(
            'build',
            ('plat_name', 'plat_name'),
        )

    def run(self):
        self.set_environ()
        self.update_git_submodules()

        print('-'*50)
        print(self.plat_name)
        print('-'*50)

    def set_environ(self):
        plat_name = self.plat_name
        if plat_name.startswith('win-'):
            plat_name = plat_name[len('win-'):]
            env = msvc.msvc14_get_vc_env(plat_name)
            os.environ.update(env)

    def git_submodules_exist(self):
        for submodule in self.git_submodules:
            path = Path(submodule)
            if not path.exists() or not any(path.iterdir()):
                return False
        return True

    def update_git_submodules(self):
        if Path('.gitmodules').exists() and not self.git_submodules_exist():
            check_call(['git', 'submodule', 'update', '--init', '--recursive'])



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
    cmdclass={
        'build_convphase': BuildConvPhase,
    },
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
