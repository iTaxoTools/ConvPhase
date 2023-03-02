workspace("ConvPhase")
	configurations({"debug", "release"})

	project("ConvPhase")
		kind("ConsoleApp")
		language("C++")
		cppdialect("C++17")

		files({"src/*.cpp"})
		includedirs({"include", "SeqPHASE/cpp/include", "/home/digitaldragon/.local/share/haxe/lib/hxcpp/4,2,1/include"})
		libdirs({"."})
		location("build")
		--targetdir("build/bin/${cfg.buildcfg}")
		links({"seqphase", "phase"})
		buildoptions({})
		linkoptions({})
		warnings("Extra")
		disablewarnings({"unused-parameter", "reorder", "deprecated-copy", "invalid-offsetof", "sign-compare", "unknown-pragmas"})

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
		includedirs({"phase/src/phase*"})
		location("build")
		links({})
		defines({"CONVPHASE_LIBRARY"})
		buildoptions({})
		linkoptions({})
		warnings("Off")
		disablewarnings({})

		filter("configurations:debug")
			defines({"DEBUG"})
			optimize("Debug")
			symbols("On")

		filter("configurations:release")
			defines({"NDEBUG"})
			optimize("Full")
			--symbols("Off")

	--[[
	project("SeqPhase")
		kind("Makefile")
		--location("build")
		buildmessage("Building SeqPhase")
		buildcommands({
			"cd SeqPHASE/haxe; haxe --cpp ../cpp -D static_link SeqPhase1.hx SeqPhase2.hx",
			"cp SeqPHASE/cpp/liboutput.a build/bin/debug/libseqphase.a"
		})
	]]
