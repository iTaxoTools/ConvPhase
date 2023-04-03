To build ConvPhase, you have first to install:

C++ compiler
premake5
haxe
hxcpp
python (optional)



First you have to build SeqPHASE and copy the library to the root folder of this project(this step will be automated in the future with premake)
cd SeqPHASE/haxe
haxe --cpp ../cpp -D static_link SeqPhase1.hx SeqPhase2.hx
cd ../..
cp SeqPHASE/cpp/liboutput.a ./libseqphase.a

Next step is to generate the build files with premake
Windows:
premake5 vs2022
Linux:
premake5 gmake2

On Windows you have to open the project solution and build it with visual studio

For Linux, build with the release configuration:
make config=release



Additional options for premake
--kind=<type>
	exe: Build the executable
	shared: Build a shared library
	static: Build a static library
--fileComm Communication through files similar to early versions of ConvPhase
--phaseCout Enable stdout of phase
--phaseCerr Enable stderr of phase
--noPython Disable Python bindings
