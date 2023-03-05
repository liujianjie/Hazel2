workspace "Hazel"		-- sln文件名
	architecture "x64"	
	configurations{
		"Debug",
		"Release",
		"Dist"
	}
-- https://github.com/premake/premake-core/wiki/Tokens#value-tokens
-- 组成输出目录:Debug-windows-x86_64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 包含相对解决方案的目录
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"		--Hazel项目
	location "Hazel"--在sln所属文件夹下的Hazel文件夹
	kind "SharedLib"--dll动态库
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}") -- 输出目录
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")-- 中间目录

	-- 预编译头 
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"

	-- 包含的所有h和cpp文件
	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	-- 包含目录
	includedirs{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}
	links { 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	-- 如果是window系统
	filter "system:windows"
		cppdialect "C++17"
		-- On:代码生成的运行库选项是MTD,静态链接MSVCRT.lib库;
		-- Off:代码生成的运行库选项是MDD,动态链接MSVCRT.dll库;打包后的exe放到另一台电脑上若无这个dll会报错
		staticruntime "On"	
		systemversion "latest"	-- windowSDK版本
		-- 预处理器定义
		defines{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"HZ_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE" -- 让GLFW不包含OpenGL
		}
		-- 编译好后移动Hazel.dll文件到Sandbox文件夹下
		postbuildcommands{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
	-- 不同配置下的预定义不同
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MD"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	-- 同样包含spdlog头文件
	includedirs{
		"Hazel/vendor/spdlog/include",
		"Hazel/src"
	}
	-- 引用hazel
	links{
		"Hazel"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines{
			"HZ_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "HZ_DIST"
		buildoptions "/MD"
		optimize "On"