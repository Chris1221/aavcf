#include <iostream>
#include <string>
#include <vector>
#include "cxxopts.hpp"
#include <fstream>
#include <algorithm>
#include "string_split.hpp"
#include "zstr.hpp"
#include <memory>
#include <assert.h>
#include "fasta.h"

using namespace std;

int main(int argc, char* argv[]){

    // Parse the command line options first.
    cxxopts::Options options("Recode a VCF to ancestral alleles", "Take the alleles given in a reference fasta file and use them as the reference in the given VCF.");
    options.add_options()
            ( 	"d,debug",
                 "Show some additional output to help debug.",
                 cxxopts::value<bool>())
            ( 	"v,vcf",
                 "Path to the VCF. Currently only sample sample VCFs are supported.",
                 cxxopts::value<std::string>())
            ( 	"f,fasta",
                 "Migration to population Y",
                 cxxopts::value<std::string>())
            ( 	"o,output",
                 "Path to output file.",
                 cxxopts::value<std::string>())
            ( 	"l,log",
                 "Path to log file.",
                 cxxopts::value<std::string>()
                    -> default_value("log.txt"))
            ;

    // Object of inputs
   auto result = options.parse(argc, argv);
   bool d = result["d"].as<bool>();

   std::ofstream log ( result["l"].as<std::string>());

   bool help = false;
    if (!result.count("f")) {
        std::cout << "Not enough arguments supplied." << std::endl;
        std::cout << "Needed: fasta" << std::endl;
        help = true;
    }
    if (result.count("help") || help) {
        std::cout << options.help({""}) << std::endl;
        exit(0);
    }

    // Read the files into the classes
    fasta ancestor (result["f"].as<std::string>());
    vcf x (result["vcf"].as<std::string>());

    /* Do:
     *  - Check if REF matches the ancestral allele (if it exists)
     *      - Otherwise just keep it, I guess?
     *      - If it does match, keep everything the same
     *      - If it doesn't match, then check the ALT. Is the ALT the same as the ancestor?
     *          - If it is, then swap them. (abs value of -1)
     */

     for(int i = 0; i < x.entries.size(); i++)
     {
         char anc = toupper(ancestor.get_allele((unsigned int) x.entries[i].pos -1));
         char ref = toupper(x.entries[i].ref);
         char alt = toupper(x.entries[i].alt);

         if (d) cout << "anc: " << anc << " ref: " << ref << " alt: " << alt << x.entries[i].gen1 << x.entries[i].gen2 << endl;

         if( ref == anc )
         {
             // pass through
         } else if (alt == anc)
         {
             log << "Flip\t" << to_string ( x.entries[i].pos ) <<
                 "\tAnc:" << anc << " Ref:" << ref << " Alt:" << alt << "\t" << to_string(x.entries[i].gen1) << '|' << to_string(x.entries[i].gen2);

             x.entries[i].gen1 = abs( x.entries[i].gen1 - 1);
             x.entries[i].gen2 = abs( x.entries[i].gen2 - 1);

             log << " -> " << to_string(x.entries[i].gen1) << '|' << to_string(x.entries[i].gen2) << endl;

         } else if (anc == 'N' | anc == '.')
         {
             log << "UnInfo\t" << to_string ( x.entries[i].pos ) <<
                 "\tAnc:" << anc << " Ref:" << ref << " Alt:" << alt << "\t" << to_string(x.entries[i].gen1) << '|' << to_string(x.entries[i].gen2);

             x.entries[i].gen1 = 0;
             x.entries[i].gen2 = 0;

             log << " -> " << to_string(x.entries[i].gen1) << '|' << to_string(x.entries[i].gen2) << endl;
         } else
         {
             log << "DifAlt\t" << to_string ( x.entries[i].pos ) <<
                 "\tAnc:" << anc << " Ref:" << ref << " Alt:" << alt << '\t' << to_string(x.entries[i].gen1) << '|' << to_string(x.entries[i].gen2);
             x.entries[i].gen1 = 0;
             x.entries[i].gen2 = 0;

             log << " -> " << to_string(x.entries[i].gen1) << '|' << to_string(x.entries[i].gen2) << endl;
         }
     }

    std::string output_path = result["o"].as<std::string>();

     x.write(output_path);


//   const char& ref_allele = ref.get_allele(150000);
 //  std::cout << ref_allele << std::endl;
  // std::cout << x.entries[4].gen1 + x.entries[4].gen2 << std::endl;

   return 0;
}

