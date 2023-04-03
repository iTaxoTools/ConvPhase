workspace("ConvPhase")
	configurations({"debug", "release"})
	newoption({
		trigger = "kind",
		description = "Kind of binary when building",
		default = "shared",
		allowed = {
			{"exe", "Executable"},
			{"static", "Static library"},
			{"shared", "Shared library"}
		}
	})
	newoption({
		trigger = "fileComm",
		description = "Communication with files"
	})
	newoption({
		trigger = "phaseCout",
		description = "Enable cout of phase"
	})
	newoption({
		trigger = "phaseCerr",
		description = "Enable cerr of phase"
	})
	newoption({
		trigger = "noPython",
		description = "Disable Python bindings"
	})
	newoption({
		trigger = "haxeInclude",
		description = "Haxe include path"
	})



	project("ConvPhase")
		--dependson({"SeqPHASE"})
		filter("options:kind=exe")
			kind("ConsoleApp")
		filter("options:kind=static")
			kind("StaticLib")
		filter("options:kind=shared")
			kind("SharedLib")
		filter("options:*")
		language("C++")
		cppdialect("C++17")

		files({"src/*.cpp"})
		includedirs({
			"include",
			--"SeqPHASE/cpp/include",
			"build/SeqPHASE_cpp/include",
			--"/home/digitaldragon/.local/share/haxe/lib/hxcpp/4,2,1/include",
			"/usr/include/python*"
		})
		includedirs({_OPTIONS["haxeInclude"]})
		print(_OPTIONS["haxeInclude"])

		libdirs({"."})
		location("build")
		--targetdir("build/bin/${cfg.buildcfg}")
		targetname("convphase")
		links({
			"seqphase",
			--"seqphase:shared",
			--"seqphase:static",
			"phase",
			"python3.10"
		})
		defines({"CP_PHASE_LIB", "CP_PHASE_NOFILE"})
		pic("On")
		buildoptions({"-pedantic"})
		linkoptions({})
		warnings("Extra")
		disablewarnings({
			"unused-parameter",
			"reorder",             --warnings caused by hxcpp
			"deprecated-copy",
			"invalid-offsetof",
			"sign-compare",
			"unknown-pragmas"
		})

		filter({"options:noPython or options:fileComm"})
			removefiles({"src/python_wrapper.cpp"})
			removelinks({"python3.10"})
		filter("options:fileComm")
			removedefines({"CP_PHASE_NOFILE"})

		filter("configurations:debug")
			defines({"DEBUG"})
			optimize("Debug")
			symbols("On")

		filter("configurations:release")
			defines({"NDEBUG"})
			optimize("Full")
			--symbols("Off")



	project("phase")
		kind("StaticLib")
		language("C++")
		cppdialect("C++17")

		files({"phase/src/phase*/*.c", "phase/src/phase*/*.cpp"})
		includedirs({"phase/src/phase*", "include"})
		location("build")
		links({})
		defines({
			"CP_PHASE_LIB",
			"CP_PHASE_DISABLE_COUT",
			"CP_PHASE_DISABLE_CERR",
			"CP_PHASE_NOFILE"
		})
		pic("On")
		buildoptions({"-pedantic"})
		linkoptions({})
		warnings("Off")
		disablewarnings({})

		filter("options:fileComm")
			removedefines({"CP_PHASE_NOFILE"})
		filter("options:phaseCout")
			removedefines({"CP_PHASE_DISABLE_COUT"})
		filter("options:phaseCerr")
			removedefines({"CP_PHASE_DISABLE_CERR"})

		filter("configurations:debug")
			defines({"DEBUG"})
			optimize("Debug")
			symbols("On")

		filter("configurations:release")
			defines({"NDEBUG"})
			optimize("Full")
			--symbols("Off")



	project("SeqPHASE")
		kind("StaticLib")
		language("C++")
		--kind("Makefile")
		location("build")
		buildmessage("Building SeqPHASE")
		files("dummy.cpp")
		postbuildcommands({
			--"rm ../dummy.cpp",
			--"touch file",
			"{CHDIR} ../SeqPHASE/haxe; haxe --cpp ../../build/SeqPHASE_cpp -D static_link -D dll_export SeqPhase1.hx SeqPhase2.hx",
			"{COPYFILE} SeqPHASE_cpp/liboutput.a %{cfg.buildtarget.relpath}",
			--"echo %{cfg.buildtarget.relpath}"
		})
		--buildinputs({"SeqPHASE/haxe/*.hx"})
		--buildoutputs({"libseqphase.so"})
		--linkbuildoutputs("On")
