# #D:\Workspace\Sampler_Arena\CMakeLists.txt
# =============================================================================
# Sampler Arena - VSTi Sampling Station / SFZ Patch Creator
# CMakeLists.txt
# =============================================================================

cmake_minimum_required(VERSION 3.22)
project(SamplerArena VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
# Platform Detection
# =============================================================================
if(WIN32)
    set(PLATFORM_WINDOWS TRUE)
elseif(APPLE)
    set(PLATFORM_MAC TRUE)
else()
    set(PLATFORM_LINUX TRUE)
endif()

# =============================================================================
# JUCE Setup
# =============================================================================
add_subdirectory(JUCE)

# =============================================================================
# macOS Icon (only set if the file actually exists)
# =============================================================================
set(MACOS_ICON_FILE "")
if(PLATFORM_MAC AND EXISTS "${CMAKE_SOURCE_DIR}/assets/Icon.icns")
    set(MACOS_ICON_FILE "${CMAKE_SOURCE_DIR}/assets/Icon.icns")
    message(STATUS "macOS icon found: ${MACOS_ICON_FILE}")
endif()

# =============================================================================
# Build juce_add_gui_app argument list (conditional optional args)
# =============================================================================
set(JUCE_APP_ARGS
    PRODUCT_NAME "Sampler Arena"
    COMPANY_NAME "Fanan"
    BUNDLE_ID "com.fanan.samplerarena"
    MICROPHONE_PERMISSION_ENABLED TRUE
    MICROPHONE_PERMISSION_TEXT "Sampler Arena needs access to your microphone for audio input recording."
    HARDENED_RUNTIME_ENABLED TRUE
    HARDENED_RUNTIME_OPTIONS "com.apple.security.cs.allow-jit;com.apple.security.cs.allow-unsigned-executable-memory;com.apple.security.cs.disable-library-validation;com.apple.security.device.audio-input"
    APP_SANDBOX_ENABLED FALSE
)

# Add icon only if it exists
if(MACOS_ICON_FILE)
    list(APPEND JUCE_APP_ARGS ICON_BIG "${MACOS_ICON_FILE}")
endif()

# Add Info.plist merge only if it exists
if(EXISTS "${CMAKE_SOURCE_DIR}/src/Info.plist")
    list(APPEND JUCE_APP_ARGS PLIST_TO_MERGE "${CMAKE_SOURCE_DIR}/src/Info.plist")
    message(STATUS "Merging custom Info.plist")
endif()

# =============================================================================
# Standalone Application Target
# =============================================================================
juce_add_gui_app(SamplerArenaApp ${JUCE_APP_ARGS})

juce_generate_juce_header(SamplerArenaApp)

# =============================================================================
# Source Files
# =============================================================================
target_sources(SamplerArenaApp PRIVATE
    # -------------------------------------------------------------------------
    # PluginProcessor (split into 6 files)
    # -------------------------------------------------------------------------
    src/PluginProcessor_01_Formats.cpp
    src/PluginProcessor_02_CtorDtor.cpp
    src/PluginProcessor_03_SettingsAndListener.cpp
    src/PluginProcessor_04_IOChannels.cpp
    src/PluginProcessor_05_Graph.cpp
    src/PluginProcessor_06_AudioAndPresets.cpp

    # -------------------------------------------------------------------------
    # GraphCanvas (7 files — removed MidiPlayer + Recorder integrations)
    # -------------------------------------------------------------------------
    src/GraphCanvas_Core.cpp
    src/GraphCanvas_Paint.cpp
    src/GraphCanvas_Layout.cpp
    src/GraphCanvas_Connections.cpp
    src/GraphCanvas_Mouse.cpp
    src/GraphCanvas_Tooltips.cpp
    src/GraphCanvas_PluginMenu.cpp
    src/GraphCanvas_PluginWindow.cpp
    src/GraphCanvas_DragDrop.cpp

    # -------------------------------------------------------------------------
    # UI / Editor
    # -------------------------------------------------------------------------
    src/PluginEditor.cpp
    src/UIComponents.cpp
    src/MidiSelectors.cpp

    # -------------------------------------------------------------------------
    # Tabs
    # -------------------------------------------------------------------------
    src/AudioSettingsTab.cpp
    src/StudioTab.cpp
    src/RegistrationTab.cpp
    src/ManualTab.cpp

    # -------------------------------------------------------------------------
    # Utility Processors (System Tools — kept only what Sampler Arena needs)
    # -------------------------------------------------------------------------
    src/MeteringProcessor.cpp
    src/SimpleConnectorProcessor.cpp
    src/StereoMeterProcessor.cpp
    src/MidiMonitorProcessor.cpp
    src/AutoSamplerProcessor.cpp
    src/MidiMultiFilterProcessor.cpp
    src/ContainerProcessor.cpp
    src/SfzPlayerProcessor.cpp

    # -------------------------------------------------------------------------
    # Core / Standalone
    # -------------------------------------------------------------------------
    src/StandaloneMain.cpp
    src/RegistrationManager.cpp
)

# =============================================================================
# Windows Icon via Resource File
# =============================================================================
if(PLATFORM_WINDOWS)
    if(EXISTS "${CMAKE_SOURCE_DIR}/src/resources.rc")
        target_sources(SamplerArenaApp PRIVATE "${CMAKE_SOURCE_DIR}/src/resources.rc")
        message(STATUS "Using resource file: src/resources.rc")
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/resources.rc")
        target_sources(SamplerArenaApp PRIVATE "${CMAKE_SOURCE_DIR}/resources.rc")
        message(STATUS "Using resource file: resources.rc")
    else()
        message(STATUS "No resource file found - building without custom icon")
    endif()
endif()

# =============================================================================
# Compile Definitions
# =============================================================================
set(COMMON_DEFINITIONS
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_REPORT_APP_USAGE=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_PLUGINHOST_VST3=1
)

if(PLATFORM_WINDOWS)
    list(APPEND COMMON_DEFINITIONS
        JUCE_WIN_PER_MONITOR_DPI_AWARE=1
        _CRT_SECURE_NO_WARNINGS
    )
endif()

if(PLATFORM_MAC)
    list(APPEND COMMON_DEFINITIONS
        JUCE_PLUGINHOST_AU=1
    )
endif()

if(PLATFORM_LINUX)
    list(APPEND COMMON_DEFINITIONS
        JUCE_PLUGINHOST_LADSPA=1
    )
endif()

# =============================================================================
# VST2 Support - Manual loading (no background scanning)
# Only enabled when VST2 SDK is available (Windows local builds)
# =============================================================================
set(VST2_SDK_PATH "D:/Workspace/vst2sdk" CACHE PATH "Path to VST2 SDK")

if(EXISTS "${VST2_SDK_PATH}/pluginterfaces/vst2.x/aeffectx.h")
    message(STATUS "VST2 SDK found - enabling VST2 hosting support")
    target_include_directories(SamplerArenaApp PRIVATE ${VST2_SDK_PATH})
    target_compile_definitions(SamplerArenaApp PRIVATE JUCE_PLUGINHOST_VST=1)
else()
    message(STATUS "VST2 SDK not found - VST2 hosting disabled")
    target_compile_definitions(SamplerArenaApp PRIVATE JUCE_PLUGINHOST_VST=0)
endif()

target_compile_definitions(SamplerArenaApp PRIVATE ${COMMON_DEFINITIONS})

# =============================================================================
# JUCE Module Setup
# =============================================================================
target_link_libraries(SamplerArenaApp PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_dsp
    juce::juce_opengl
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# =============================================================================
# Include Paths
# =============================================================================
target_include_directories(SamplerArenaApp PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# =============================================================================
# ASIO Support (Windows)
# =============================================================================
set(ASIO_SDK_PATH "D:/Workspace/ASIOSDK" CACHE PATH "Path to ASIO SDK")

if(PLATFORM_WINDOWS AND EXISTS "${ASIO_SDK_PATH}/common/asio.h")
    message(STATUS "ASIO SDK found - enabling ASIO support")
    target_compile_definitions(SamplerArenaApp PRIVATE JUCE_ASIO=1)
    target_include_directories(SamplerArenaApp PRIVATE
        ${ASIO_SDK_PATH}/common
        ${ASIO_SDK_PATH}/host
    )
else()
    message(STATUS "ASIO SDK not found - building without ASIO support")
endif()

# =============================================================================
# CLAP Support
# =============================================================================
if(TARGET clap-wrapper)
    target_link_libraries(SamplerArenaApp PRIVATE clap-wrapper)
    message(STATUS "CLAP wrapper enabled")
endif()

# =============================================================================
# macOS Post-Build: Copy icon to bundle (only if icon exists)
# =============================================================================
if(PLATFORM_MAC AND MACOS_ICON_FILE)
    add_custom_command(TARGET SamplerArenaApp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MACOS_ICON_FILE}"
            "$<TARGET_BUNDLE_DIR:SamplerArenaApp>/Contents/Resources/Icon.icns"
        COMMENT "Copying macOS icon to bundle"
    )
endif()