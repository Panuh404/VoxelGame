-- Function to prepend a header line to a file
function addPrecompiledHeader(filepath, header)
    -- Open file for reading
    local file = io.open(filepath, "r")
    if not file then
        print("Error: File not found: " .. filepath)
        return
    end

    -- Read the content
    local content = file:read("*all")
    file:close()

    -- Check if the header is already included
    if content:find(header, 1, true) then
        print("Header already included in " .. filepath)
        return
    end

    -- Prepend the header
    local updatedContent = header .. "\n" .. content

    -- Write the updated content back to the file
    file = io.open(filepath, "w")
    if not file then
        print("Error: Unable to write to file: " .. filepath)
        return
    end
    file:write(updatedContent)
    file:close()

    print("Added precompiled header to the top of " .. filepath)
end

workspace "VoxelGame"
    architecture "x86_64"
    startproject "VoxelGame"

    configurations{
        "Debug",
        "Release"
    }

    flags{
        "MultiProcessorCompile"
    }

OutputDirectory = "%{cfg.buildcfg}-%{cfg.architecture}"

--include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] =        "VoxelGame/vendor/GLFW/include"
IncludeDir["Glad"] =        "VoxelGame/vendor/glad/include"
IncludeDir["spdlog"] =      "VoxelGame/vendor/spdlog/include"
IncludeDir["ImGui"] =       "VoxelGame/vendor/ImGui"
IncludeDir["glm"] =         "VoxelGame/vendor/glm"
IncludeDir["stb"] =         "VoxelGame/vendor/stb"
IncludeDir["lodepng"] =     "VoxelGame/vendor/lodepng"
IncludeDir["miniaudio"] =   "VoxelGame/vendor/miniaudio"

-- Add the precompiled header to the submodule
addPrecompiledHeader("VoxelGame/vendor/lodepng/lodepng.cpp", "#include \"GamePCH.h\"")

-- Vendor depencies group
group "Dependencies"
    include "VoxelGame/vendor"
group ""

group "Core"
    
    project "VoxelGame"
        location "VoxelGame"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        staticruntime "on"

        targetdir ("Binaries/" .. OutputDirectory .. "/%{prj.name}")
        objdir ("Intermediates/" .. OutputDirectory .. "/%{prj.name}")

        pchheader "GamePCH.h"
        pchsource "VoxelGame/src/GamePCH.cpp"

        files{
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/stb/**.h",
            "%{prj.name}/vendor/stb/**.cpp",
            "%{prj.name}/vendor/glm/glm/**.hpp",
            "%{prj.name}/vendor/glm/glm/**.inl",
            "%{prj.name}/vendor/lodepng/lodepng.h",
            "%{prj.name}/vendor/lodepng/lodepng.cpp",
        }

        includedirs{
            "VoxelGame/src",
            "VoxelGame/vendor",
            "%{IncludeDir.GLFW}",
            "%{IncludeDir.Glad}",
            "%{IncludeDir.spdlog}",
            "%{IncludeDir.ImGui}",
            "%{IncludeDir.glm}",
            "%{IncludeDir.stb}",
            "%{IncludeDir.lodepng}",
            "%{IncludeDir.miniaudio}",
        }

        links{
            "GLFW",
            "Glad",
            "ImGui",
            "opengl32.lib"
        }

        filter "system:windows"
            buildoptions { 
                "/utf-8" 
            }
            systemversion "latest"

            defines{
                "GLFW_INCLUDE_NONE"
            }

            filter "configurations:Debug"
            runtime "Debug"
            symbols "on"

        filter "configurations:Release"
            runtime "Release"
            optimize "on"

group ""