# ======================================================================
#  main build file
# ======================================================================


# use cmake 2.8 or later
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

PROJECT(gm2midastoart)

#Check external dependencies are present
if( NOT DEFINED ENV{MIDASSYS} )
  message( FATAL_ERROR "ERROR: MIDASSYS not defined" )
endif()

# cetbuildtools contains our cmake modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()

set( CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules ${CMAKE_MODULE_PATH} )
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

# include the necessary macros
include(CetCMakeEnv)
cet_cmake_env()

# g-2 wants the product name in the library name
set( ART_MAKE_PREPEND_PRODUCT_NAME TRUE)

# set compiler flags
cet_set_compiler_flags( DIAGS CAUTIOUS 
   WERROR
   EXTRA_FLAGS -pedantic
)
cet_report_compiler_flags()

# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff
find_ups_product(cetbuildtools v4_08_01)
find_ups_product(art v1_13_01)
find_ups_product(fhiclcpp v3_03_00)
find_ups_product(messagefacility v1_14_00)
find_ups_product(gm2dataproducts v3_03_00)

# macros for art_make and simple plugins
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# add source subdirectories here
add_subdirectory(sources)
add_subdirectory(example)
#add_subdirectory(calo)
add_subdirectory(ups)

# packaging utility
include(UseCPack)
