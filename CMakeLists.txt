cmake_minimum_required(VERSION 3.7.2)

# From sel4test, but moved to top level so the project directory CMakeLists
# are simpler
include(tools/cmake-tool/default-CMakeLists.txt)
include(${KERNEL_HELPERS_PATH})

set_property(CACHE PLATFORM PROPERTY STRINGS ${KernelX86Sel4Arch_all_strings} ${KernelARMPlatform_all_strings} ${KernelRiscVPlatform_all_strings})

if (${PLATFORM} IN_LIST KernelX86Sel4Arch_all_strings)
	set(KernelArch x86 CACHE STRING "" FORCE)
	set(KernelX86Sel4Arch ${PLATFORM} CACHE STRING "" FORCE)
elseif(${PLATFORM} IN_LIST KernelRiscVPlatform_all_strings)
	set(KernelArch riscv CACHE STRING "" FORCE)
	set(KernelRiscVPlatform ${PLATFORM} CACHE STRING "" FORCE)

	if(RISCV64)
		set(KernelRiscVSel4Arch "riscv64" CACHE STRING "" FORCE)
	else()
		set(KernelRiscVSel4Arch "riscv32" CACHE STRING "" FORCE)
	endif()
else()
	if (NOT ${PLATFORM} IN_LIST KernelARMPlatform_all_strings)
		message(FATAL_ERROR "Unknown PLATFORM. Initial configuration may not work")
	endif()

	set(KernelArch arm CACHE STRING "" FORCE)
	set(KernelARMPlatform ${PLATFORM} CACHE STRING "" FORCE)

	if(ARM_HYP)
		set(KernelArmHypervisorSupport ON CACHE BOOL "" FORCE)
	endif()

	if(AARCH64)
		set(KernelArmSel4Arch "aarch64" CACHE STRING "" FORCE)
	elseif(AARCH32)
		set(KernelArmSel4Arch "aarch32" CACHE STRING "" FORCE)
		if(ARM_HYP)
			set(KernelArmSel4Arch "arm_hyp" CACHE STRING "" FORCE)
		endif()
	endif()
endif()

if(SIMULATION)
	#ApplyCommonSimulationSettings("x86")
	ApplyCommonSimulationSettings(${KernelArch})
else()
        if(KernelArchX86)
                set(KernelIOMMU ON CACHE BOOL "" FORCE)
        endif()
endif()

if(KernelSel4ArchX86_64)
    SetSimulationScriptProperty(MEM_SIZE "3G")
endif()

# We must build the debug kernel because the tutorials rely on seL4_DebugPutChar
# and they don't initialize a platsupport driver.
ApplyCommonReleaseVerificationSettings(FALSE FALSE)

GenerateSimulateScript()
