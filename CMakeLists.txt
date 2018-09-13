cmake_minimum_required      (VERSION 3.10)
project                     (aavcf)

set                         (CMAKE_CXX_STANDARD 11)
include_directories         (lib)

# Use zlib for zstr
find_package                (ZLIB REQUIRED )
if ( ZLIB_FOUND )
    include_directories( ${ZLIB_INCLUDE_DIRS} )
    link_libraries( ${ZLIB_LIBRARIES} )
endif( ZLIB_FOUND )

FIND_PACKAGE                (Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_test(NAME example_call
        COMMAND aavcf -f ../test/human_ancestor_22.fa.gz -v ../test/S_Yoruba-2_22.vcf.gz)

add_executable              (aavcf src/main.cpp src/fasta.cpp src/fasta.h lib/string_split.cpp)