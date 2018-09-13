//
// Created by ccole on 12/09/2018.
//

#include<string>
#include "fasta.h"
#include "zstr.hpp"
#include "string_split.hpp"

using namespace std;

// This is the constructor
fasta::fasta(std::string path)
{
    std::string line;
    //std::string sequence;

    // Open a gzipped stream
    zstr::ifstream in (path);
    std::getline (in, line);

    // Take the first one and call it header...
    // because its the header
    header=line;

    /* Now go through the whole fasta file,
     * create a string which holds the whole thing
     *
     * To access an element, I'll define a seperate`
     * function. */

    while ( std::getline (in, line) ){

        sequence+=line;
    }

}

/* Now this is a member function.
 * Retrieve an allele at a point in the sequence. */
const char& fasta::get_allele(unsigned pos)
{
    return sequence.at(pos);
}

/* Construct the vcf_line class from
 * the read line in the vcf file. */
vcf_line::vcf_line(std::string line, char delim)
{
    full = line;
    vector<std::string> raw = split(line, delim);

    chr     =   stoi(raw[0]);
    pos     =   stoi(raw[1]);
    id      =   raw[2];
    ref     =   raw[3].at(0);
    alt     =   raw[4].at(0);
    qual    =   raw[5];
    filter  =   raw[6];
    info    =   raw[7];
    format  =   raw[8];

    vector<std::string> tmp = split(raw[9], '|');
    //vector<std::string> tmp2 = split(tmp[0], '|');
    gen1 = stoi(tmp[0]);
    gen2 = stoi(tmp[1]);


   // int geno = gen1+gen2;
}

std::string vcf_line::write()
{
    return std::string() + '\t' + to_string (chr)  + '\t' + to_string (pos) + '\t' + id + '\t'  + to_string (ref) + '\t' + to_string (alt) + '\t' + qual + '\t' + filter + '\t' + info + '\t' + format + '\t' + to_string(gen1) + '|' + to_string(gen2);
}

/* This is the constructor for vcf
 * file format. It just strings together
 * some vcf_line classes */
vcf::vcf(std::string path)
{
    std::string line;
    zstr::ifstream in (path);

    while ( std::getline (in, line) ){

       if (line.at(0) == '#') {
           meta.push_back(line);
           continue;
       }
       vcf_line vline = vcf_line(line);

       entries.push_back(vline);

    }

}

void vcf::write(std::string path)
{
    zstr::ofstream out (path);

    for(int i = 0; i < meta.size(); i++){
        out << meta[i] << endl;
    }

    for(int i = 0; i < entries.size(); i++) {
        out << entries[i].write() << endl;
    }
}
