#Build directory contents
#Stuff in here is passed to simple_plugin()
art_make( SOURCE_LIBRARIES
          art_Framework_IO_Sources
          fhiclcpp
          $ENV{MIDASSYS}/linux/lib/libmidas-shared.so
)

#Define any additional header dirs for products not in UPS
INCLUDE_DIRECTORIES(
	$ENV{MIDASSYS}/include
)

#Install header files into the products area
install_headers()

