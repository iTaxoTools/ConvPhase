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



	project("ConvPhase")
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
			"SeqPHASE/cpp/include",
			"/home/digitaldragon/.local/share/haxe/lib/hxcpp/4,2,1/include",
			"/usr/include/python*"
		})
		libdirs({"."})
		location("build")
		--targetdir("build/bin/${cfg.buildcfg}")
		targetname("convphase")
		links({
			"seqphase",
			"phase",
			"python3.10"
		})
		defines({"CP_PHASE_LIB", "CP_PHASE_NOFILE"})
		buildoptions({"-fPIC", "-pedantic"})
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
		buildoptions({"-fPIC"})
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



	--[[project("SeqPhase")
		kind("Makefile")
		--location("build")
		buildmessage("Building SeqPhase")
		buildcommands({
			"cd SeqPHASE/haxe; haxe --cpp ../cpp -D static_link SeqPhase1.hx SeqPhase2.hx",
			"cp SeqPHASE/cpp/liboutput.a build/bin/debug/libseqphase.a"
		})
		]]
