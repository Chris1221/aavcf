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

using namespace std;

int main(int argc, char* argv[]){

    // Parse the command line options first.
    cxxopts::Options options("Identify Migration Events", "Find sequences which have had migration events from a population to another population.");
    options.add_options()
            ( 	"d,debug",
                 "Show some additional output to help debug.",
                 cxxopts::value<bool>())
            ( 	"T,tree",
                 "Path to tree file (will try to guess if it's gzipped).",
                 cxxopts::value<std::string>())
            ( 	"b,begin",
                 "Looking for migration events after this age range (i.e. 70kya)",
                 cxxopts::value<double>())
            ( 	"e,end",
                 "Looking for migration events before this age range (i.e. 50kya)",
                 cxxopts::value<double>())
            ( 	"f,fp",
                 "Migration from population X",
                 cxxopts::value<std::string>())
            ( 	"t,to",
                 "Migration to population Y",
                 cxxopts::value<std::string>())
            ( 	"o,output",
                 "Path to output file.",
                 cxxopts::value<std::string>())
            ;

    // Object of inputs
    auto result = options.parse(argc, argv);

    bool help = false;
    if( !result.count("T") ){
        if ( !result.count("h") || !result.count("s") || !result.count("o") || !result.count("mode")){
            std::cout << "Not enough arguments supplied." << std::endl;
            std::cout << "Needed: T" << std::endl;
            help = true;
        }
    }
    if(result.count("help") || help){
        std::cout << options.help({""}) << std::endl;
        exit(0);
    }

    // Set debug
    bool d = result["d"].as<bool>();

    // Read in the tree fileA
    zstr::ifstream in (result["tree"].as<std::string>());

    vector<string> line_s;
    std::string line;
    char delim = '\t';
    string start;
    double start_time;
    string end;
    double end_time;

    /* File format of each line is:
     * line_s[0] : R, C, or M for which event type
     * line_s[1] : BP position
     * line_s[2] : Time of event (in generations)
     * line_s[3] : From population
     * line_s[4] : To population
     * line_s[5] : Terminal branch? this shows which group its talking about.
    */

    while ( std::getline (in, line) ){

        line_s = split(line, delim);

        // Is it a migration event?
        if (line_s[0] == "M") {
            // Is it from the right population?
            if(stoi(line_s[3]) == stoi(result["f"].as<string>())) {
                // Is it to the right population?
                //  This is currently redundant but will not always be.
                if(stoi(line_s[4]) == stoi(result["t"].as<string>())){
                    //Is it in the right time range?
                    //  Make this more flexible eventually
                    if(stod(line_s[2])*29 > result["e"].as<double>() && stod(line_s[2])*29 < result["b"].as<double>()){

                    // Print line for debug
                    if(d) std::cout << line << std::endl;

                    // Take the start of the sequence
                    start = line_s[1];
                    start_time = stod(line_s[2])*29;

                    bool done = false;


                    //Now try to find the end... i.e. the recombionation event which brings this back to the population.
                    do {
                        getline(in, line);
                        line_s = split(line, delim);

                        //if(d) cout << line << endl;
                        if (line_s[0] == "R" &&
                            //stoi(line_s[3]) == stoi(result["to"].as<string>()) &&
                            //stoi(line_s[4]) == stoi(result["from"].as<string>()) &&
                            stod(line_s[2])*29 < start_time) {
                                end = line_s[1];
                                end_time = stod(line_s[2]) * 29;
                                done = true;
                        }
                    } while( !done);

                    cout << start << "\t" << end << "\t" << start_time << "\t" << end_time << endl;



        }}}}


    }
}
