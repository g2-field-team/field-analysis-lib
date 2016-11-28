# ======================================================================
#  gm2dataproducts main build file
#
#  . mrb slp   # Run if you have local products to set up
#  . mrb s     # Needed to set up local development products
#  mrb b       # Build
#  mrb t       # Test (if applicable)
#  mrb i       # Install to local products
#  gm2 -c <fhicl_file>  # Run!
#
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

project(gm2dataproducts)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH $ENV{CANVAS_DIR}/Modules
$ENV{CETBUILDTOOLS_DIR}/Modules
${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

set_install_root()

# g-2 wants the product name in the library name
set (ART_MAKE_PREPEND_PRODUCT_NAME TRUE)

if ( ${APPLE} )
cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic

)
else()
cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic

)
endif()

cet_report_compiler_flags()

# these are minimum required versions, not the actual product versions
find_ups_product(cetbuildtools v5_04_03)
find_ups_product(art v2_03_00)
find_ups_product(gm2geom v4_00_01)
find_ups_geant4(v4_10_2_p02a)
find_ups_product(artg4 v4_00_01)

# Macros for art_make and simple plugins
# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Build these directories
add_subdirectory( mc )
add_subdirectory( daq )
add_subdirectory( calo )
add_subdirectory( strawtracker )
add_subdirectory( aux )
add_subdirectory( ups )

# Packaging facility
include(UseCPack)
