cmake_minimum_required(VERSION 3.7.2)

# From sel4test, but moved to top level so the project directory CMakeLists
# are simpler
include(tools/cmake-tool/default-CMakeLists.txt)
include(${KERNEL_HELPERS_PATH})

if(KernelSel4ArchX86_64)
    SetSimulationScriptProperty(MEM_SIZE "3G")
endif()

GenerateSimulateScript()

# Declare a cache variable that enables/disablings the forcing of cache variables to
# the specific test values. By default it is disabled
set(Sel4testAllowSettingsOverride OFF CACHE BOOL "Allow user to override configuration settings")

# Define our top level settings. Whilst they have doc strings for readability here
# they are hiddein the cmake-gui as they cannot be reliably changed after the initial
# configuration, but enterprising users can still change them if they know what they
# are doing through advanced mode
# Users should initialize a build directory by doing something like
# mkdir build_sabre
# cd build_sabre
# ../init-build.sh -DARM=TRUE -DPLATFORM=sabre -DSIMULATION=TRUE
set(SIMULATION OFF CACHE BOOL "Include only simulation compatible tests")
set(RELEASE OFF CACHE BOOL "Performance optimized build")
set(VERIFICATION OFF CACHE BOOL "Only verification friendly kernel features")
set(BAMBOO OFF CACHE BOOL "Enable machine parseable output")
set(DOMAINS OFF CACHE BOOL "Test multiple domains")
set(SMP OFF CACHE BOOL "(if supported) Test SMP kernel")
set(PLATFORM "x86_64" CACHE STRING "Platform to test")
set_property(CACHE PLATFORM PROPERTY STRINGS ${KernelX86Sel4Arch_all_strings} ${KernelARMPlatform_all_strings} ${KernelRiscVPlatform_all_strings})
set(ARM_HYP OFF CACHE BOOL "Hyp mode for ARM platforms")

# We use 'FORCE' when settings these values instead of 'INTERNAL' so that they still appear
# in the cmake-gui to prevent excessively confusing users
if (NOT Sel4testAllowSettingsOverride)
    # Determine the platform/architecture
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

        # Elfloader settings that correspond to how Data61 sets its boards up.
        ApplyData61ElfLoaderSettings(${KernelARMPlatform} ${KernelArmSel4Arch})
    endif()

    if(SIMULATION)
        ApplyCommonSimulationSettings(${KernelArch})
    else()
        if(KernelArchX86)
            set(KernelIOMMU ON CACHE BOOL "" FORCE)
        endif()
    endif()
endif()


# We must build the debug kernel because the tutorials rely on seL4_DebugPutChar
# and they don't initialize a platsupport driver.
ApplyCommonReleaseVerificationSettings(FALSE FALSE)
