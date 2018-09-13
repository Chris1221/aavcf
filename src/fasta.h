//
// Created by ccole on 12/09/2018.
//

#ifndef AAVCF_FASTA_H
#define AAVCF_FASTA_H

#include<string>
#include<vector>

/* This class holds a fasta file
 * as a single string stored in
 * sequence
 * And the header element in
 * header
 *
 * Later, I would like to add
 * some error checking, and some helper
 * functions. */
class fasta
{
public:
    std::string header;
    std::string sequence;

    // This is the constructor
    // it is implemented in the .cpp
    fasta(std::string path);

    // Retrieve an allele at a particular position.
    const char& get_allele(unsigned pos);
};

/* This class is used inside the vcf class
 * in order to store each of the entries.
 * That way, a specific entry can be found, or
 * one matching a certain condition.
 *
 * That second part I'm not sure how to do yet.
 * But we'll see. */
class vcf_line
{
public:
    /* Constructor, which takes a line from a VCF and splits it
     * into the important parts. */
    vcf_line(std::string line, char delim = '\t');
    std::string write();

    // The elements of the vcf.
    std::string full;
    int chr;
    int pos;
    std::string id;
    char ref;
    char alt;
    std::string qual;
    std::string filter;
    std::string info;
    std::string format;
    int gen1;
    int gen2;
};

/* This class holds all the information
 * in a single sample VCF.
 *
 * Currently, I'm discarding phasing information
 * because I don't need it. Later it would be
 * nice to add this. */
// TODO Account for phasing
class vcf
{
public:
    // Construct with just the pass.
    vcf(std::string path);

    // Hold the meta information for writing
    std::vector<std::string> meta;

    // A vector of vcf_line
    std::vector<vcf_line> entries;

    // Write out VCF to (gz) file.
    void write(std::string path);

};

#endif //AAVCF_FASTA_H
