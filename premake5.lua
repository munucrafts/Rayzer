-- premake5.lua
workspace "Rayzer"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Rayzer"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Rayzer"