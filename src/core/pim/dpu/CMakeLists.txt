#------------------------------------------------------------------------------
# ToDo:
# 1. The current cmake file would not include the appropriate files upon 
#    installation of the library to the system.
#    However the current configuration enables runing programs within the library 
#    and not for production
#------------------------------------------------------------------------------


# Get all dpu programs files to compiles
file(GLOB DPU_SOURCE_FILES "./*_dpu.c")

# set the C compiler used to compile the DPU programs
set(DPU_CLANG_COMPILER "dpu-upmem-dpurte-clang")

if(RUN_ON_DPU)
# Create a custom target to compile all programs
    add_custom_target(
        build_dpu_programs
        COMMENT "Compiling C DPU programs..."
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

# Loop through each source file
    foreach(source_file ${DPU_SOURCE_FILES})
        # Extract the filename without extension
        get_filename_component(program_name ${source_file} NAME_WE)

        # Add a custom command for each program to compile
        add_custom_command(
            TARGET build_dpu_programs
            COMMAND ${CMAKE_COMMAND} -E env ${DPU_CLANG_COMPILER} -DNR_TASKLETS=12 -o ${CMAKE_BINARY_DIR}/src/core/pim/dpu/${program_name} ${source_file}
            COMMENT "Compiling DPU Program ${program_name}..."
            DEPENDS ${source_file}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    endforeach()
endif(RUN_ON_DPU)