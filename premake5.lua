workspace("ConvPhase")
	configurations({"release", "debug"})
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
	newoption({
		trigger = "pythonInclude",
		description = "Python include path"
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
		architecture("x64")

		files({"src/*.cpp"})
		includedirs({
			"include",
			--"SeqPHASE/cpp/include",
			"build/SeqPHASE_cpp/include",
			--"/home/digitaldragon/.local/share/haxe/lib/hxcpp/4,2,1/include",
			--"/usr/include/python*",
			-- "C:/Program Files/Haxe/haxe/lib/hxcpp/4,2,1/include",
			--"C:/Program Files/Python311/include",
		})
		if _OPTIONS["haxeInclude"] then
			includedirs({_OPTIONS["haxeInclude"]})
		elseif os.getenv("HAXEPATH") then
			includedirs({os.getenv("HAXEPATH") .. "/lib/hxcpp/4,2,1/include"})
		else
			print("Warning: No haxe include path set, consider setting it manually with --haxeInclude=<PATH>")
		end
		if _OPTIONS["pythonInclude"] then
			includedirs({_OPTIONS["pythonInclude"]})
		end
			
		--print(_OPTIONS["haxeInclude"])
		libdirs({"thirdparty/python"})
		location("build")
		--targetdir("build/bin/${cfg.buildcfg}")
		targetname("convphase")
		links({
			"SeqPHASE",
			--"seqphase:shared",
			--"seqphase:static",
			"phase",
			"python3"
		})
		defines({
			"CP_PHASE_LIB",
			"CP_PHASE_NOFILE",
			"HXCPP_M64",
			"HX_WINDOWS",
			"HXCPP_API_LEVEL=400",
			-- "HXCPP_VISIT_ALLOCS",
			-- "HXCPP_DLL_EXPORT",
			-- "HX_SMART_STRINGS",
			-- "_CRT_SERCURE_NO_DEPRECATE",
			-- "_ALLOW_MSC_VER_MISMATCH",
			-- "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH",
		})
		pic("On")
		--buildoptions({"-pedantic"})
		linkoptions({})
		warnings("Extra")
		--[[disablewarnings({
			"unused-parameter",
			"reorder",             --warnings caused by hxcpp
			"deprecated-copy",
			"invalid-offsetof",
			"sign-compare",
			"unknown-pragmas"
		})
		]]
		filter({"options:noPython or options:fileComm"})
			removefiles({"src/python_wrapper.cpp"})
			removelinks({"python3"})
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
		kind("SharedLib")
		language("C++")
		cppdialect("C++14")
		architecture("x64")

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
		--buildoptions({"-pedantic"})
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
		kind("SharedLib")
		language("C++")
		architecture("x64")
		--kind("Makefile")
		location("build")
		buildmessage("Building SeqPHASE")
		files("dummy.cpp")
		postbuildcommands({
			"{CHDIR} ../SeqPHASE/haxe && haxe --cpp ../../build/SeqPHASE_cpp -D static_link -D dll_export -D HXCPP_M64 -D ABI=-MD -D HAXE_OUTPUT_FILE=%{cfg.buildtarget.basename} SeqPhase1.hx SeqPhase2.hx",
			"{CHDIR} ../../build",
			"{COPYFILE} SeqPHASE_cpp/%{cfg.buildtarget.basename}.lib %{cfg.buildtarget.directory}",
			-- "{COPYFILE} SeqPHASE_cpp/%{cfg.buildtarget.basename}.dll %{cfg.buildtarget.directory}",
			-- "{COPYFILE} SeqPHASE_cpp/obj/lib/%{cfg.buildtarget.basename}.lib %{cfg.buildtarget.directory}",
			--"echo %{cfg.buildtarget.relpath}"
		})
		--buildinputs({"SeqPHASE/haxe/*.hx"})
		--buildoutputs({"libseqphase.so"})
		--linkbuildoutputs("On")
