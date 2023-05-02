"""A setuptools based setup module."""

# Always prefer setuptools over distutils
from setuptools import setup, find_namespace_packages, Command, msvc
from subprocess import check_call, check_output, CalledProcessError
from distutils import sysconfig
from pathlib import Path
import sys
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

        self.hxcpp_includes = []
        self.python_includes = []
        self.python_libs = []
        self.plat_name = None
        self.windows = False

    def finalize_options(self):
        self.set_undefined_options(
            'build',
            ('plat_name', 'plat_name'),
        )

        self.find_hxcpp_includes()
        self.find_python_includes()
        self.find_python_libs()

    def run(self):
        self.set_environ()
        self.update_git_submodules()
        self.premake()
        self.build()

    def set_environ(self):
        plat_name = self.plat_name
        if plat_name.startswith('win-'):
            plat_name = plat_name[len('win-'):]
            env = msvc.msvc14_get_vc_env(plat_name)
            os.environ.update(env)
            self.windows = True

    def git_submodules_initialized(self):
        try:
            output = check_output(['git', 'submodule', 'status'])
        except OSError as e:
            raise Exception('Couldn\'t find git, is git installed?') from e
        except CalledProcessError as e:
            raise Exception('Couldn\'t check git submodule status') from e

        submodules = output.decode('utf-8').split('\n')
        for submodule in submodules:
            if submodule.strip().startswith('-'):
                return False
        return True

    def update_git_submodules(self):
        if not self.git_submodules_initialized():
            try:
                check_call(['git', 'submodule', 'update', '--init', '--recursive'])
            except OSError as e:
                raise Exception('Couldn\'t find git, is git installed?') from e
            except CalledProcessError as e:
                raise Exception('Couldn\'t initialize git submodules') from e

    def find_hxcpp_includes(self):
        try:
            output = check_output(['haxelib', 'libpath', 'hxcpp'])
        except OSError as e:
            raise Exception('Couldn\'t find haxelib, is haxe installed?') from e
        except CalledProcessError as e:
            raise Exception('Couldn\'t find hxcpp libpath, is hxcpp installed? (haxelib install hxcpp)') from e
        hxcpp = output.decode('utf-8').strip()
        path = Path(hxcpp) / 'include'
        if not path.exists() or not any(path.iterdir()):
            raise Exception(f'Bad hxcpp include: {str(path)}')
        self.hxcpp_includes.append(path)

    def find_python_includes(self):
        includes = sysconfig.get_python_inc()
        self.python_includes.append(Path(includes))

        if sys.exec_prefix != sys.base_exec_prefix:
            includes = Path(sys.exec_prefix) / 'include'
            self.python_includes.append(Path(includes))

        plat_includes = sysconfig.get_python_inc(plat_specific=1)
        if plat_includes != includes:
            self.python_includes.append(Path(plat_includes))

    def find_python_libs(self):
        libs = Path(sys.base_exec_prefix) / 'libs'
        self.python_libs.append(libs)

    def premake(self):
        plat_name = self.plat_name
        if self.windows:
            version = os.environ.get('VisualStudioVersion', None)
            if version == '16.0':
                target = 'vs2019'
            elif version == '17.0':
                target = 'vs2022'
            else:
                raise Exception('Cannot determine Visual Studio version')
        else:
            target = 'gmake2'

        try:
            output = check_call(['premake5', target])
        except OSError as e:
            raise Exception('Couldn\'t find premake5, is premake installed?') from e
        except CalledProcessError as e:
            raise Exception(f'premake failed for target: {target}') from e

    def build(self):
        if self.windows:
            tool = 'msbuild'
        else:
            tool = 'make'

        try:
            output = check_call([tool])
        except OSError as e:
            raise Exception(f'Couldn\'t find build tool: {tool}') from e
        except CalledProcessError as e:
            raise Exception(f'Building with {tool} failed!') from e


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
