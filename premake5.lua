workspace("ConvPhase")
	configurations({"debug", "release"})
	newoption({
		trigger = "kind",
		description = "Kind of binary when building",
		default = "shared",
		allowed = {
			{"exe", "Executable"},
			{"static", "Static library"},
			{"shared", "Shared library"},
		}
	})
	newoption({
		trigger = "filecomm",
		description = "Communication with files"
	})
	newoption({
		trigger = "phasecout",
		description = "Enable cout of phase"
	})
	newoption({
		trigger = "phasecerr",
		description = "Enable cerr of phase"
	})
	newoption({
		trigger = "nopython",
		description = "Disable Python bindings"
	})
	newoption({
		trigger = "haxeinclude",
		description = "Haxe include path"
	})
	newoption({
		trigger = "pythoninclude",
		description = "Python include path"
	})
	newoption({
		trigger = "arch",
		description = "Target architecture",
		default = "x86_64",
		allowed = {
			{"x86", "x86/x32, 32 bit architecture"},
			{"x86_64", "x86_64/x64, 64 bit architecture"},
		}
	})
	newoption({
		trigger = "pythonVersion",
		description = "Python version to link against",
		default = "3.10"
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
		architecture(_OPTIONS["arch"])

		files({"src/*.cpp"})
		includedirs({
			"include",
			"build/SeqPHASE_cpp/include",
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
			
		--libdirs({"thirdparty/python"})
		location("build")
		--targetdir("build/bin/${cfg.buildcfg}")
		targetname("convphase")
		links({
			"phase",
			"SeqPHASE",
		})
		filter({"system:windows"})
			links("python3")
		filter({"not system:windows"})
			links("python" .. _OPTIONS["pythonVersion"])
		filter({})
		defines({
			"CP_PHASE_LIB",
			"CP_PHASE_NOFILE",
			"HXCPP_API_LEVEL=400",
			-- "HXCPP_VISIT_ALLOCS",
			-- "HXCPP_DLL_EXPORT",
			-- "HX_SMART_STRINGS",
			-- "_CRT_SERCURE_NO_DEPRECATE",
			-- "_ALLOW_MSC_VER_MISMATCH",
			-- "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH",
		})
		filter("system:windows")
			defines({"HX_WINDOWS"})
		filter({})
		if _OPTIONS["arch"] == "x86" then
			defines("HXCPP_M32")
		elseif _OPTIONS["arch"] == "x86_64" then
			defines("HXCPP_M64")
		end
		pic("On")
		--buildoptions({"-pedantic"})
		linkoptions({})
		warnings("Extra")
		filter({"not system:windows"})
			disablewarnings({
				"unused-parameter",
				"reorder",             --warnings caused by hxcpp
				"deprecated-copy",
				"invalid-offsetof",
				"sign-compare",
				"unknown-pragmas"
			})
		filter({})
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
		architecture(_OPTIONS["arch"])

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
		kind("StaticLib")
		language("C++")
		architecture(_OPTIONS["arch"])
		location("build")
		buildmessage("Building SeqPHASE")
		files("dummy.cpp")

		haxeBuildCmd = {
			"{CHDIR} ../SeqPHASE/haxe &&",
			"haxe --cpp ../../build/SeqPHASE_cpp",
			"-D static_link",
			"-D ABI=-MD",
			"-D HAXE_OUTPUT_FILE=%{cfg.buildtarget.basename}",
			"SeqPhase1.hx",
			"SeqPhase2.hx",
		}
		if _OPTIONS["arch"] == "x86" then
			table.insert(haxeBuildCmd, "-D HXCPP_M32")
		elseif _OPTIONS["arch"] == "x86_64" then
			table.insert(haxeBuildCmd, "-D HXCPP_M64")
		end

		postbuildcommands({
			table.concat(haxeBuildCmd, " ")
		})
		filter("system:windows")
			postbuildcommands({
				"{CHDIR} ../../build",
			})
		filter({})
		postbuildcommands({
			"{COPYFILE} SeqPHASE_cpp/%{cfg.buildtarget.basename}%{cfg.buildtarget.extension} %{cfg.buildtarget.relpath}",
		})

