# `aavcf`
## A tiny `C++11` tool for recoding alleles in a VCF. 

This aims to be an easy way to recode alleles from major/minor to ancestral/derived. You only need:

- A `.vcf(.gz)` (Phased, or unphased, but you must specify this).
- A `.fasta(.gz)` with ancestral states of each base. 

Usage is pretty straightforward:

```{sh}
aavcf   -v your.vcf(.gz) \
        -f your.fasta(.gz) \
        -l your.log \
```
  
  
### Installation

*Short version*: Same as any project build on `CMake`.

*Slightly longer version*: 

```{sh}
cd src; cmake
make
```

### Usage Flags

- `-v / --vcf`: The path to a `.vcf(.gz)` file in `4.1-4.3` file format. 
    - Currently only one sample VCFs are acceptable input.
- `-f / --fasta`: The path to a `.fasta(.gz)` file on the same build as the VCF (i.e. the genomic coordinates are the same).
- `-l / --log`: Path to a log file to store all information on actions taken.
    - Defaults to `./log.txt`.

### Actions

`aavcf` will do one of four things for each variant in the VCF:

1. Do nothing: If the ancestral allele is already the reference, no action is taken.
2. Flip alleles (`Flip` in log): If the ancestral is the alternate, the coding is flipped.
3. Uninformative allele (`UnInfo` in log): The ancestral allele is unknown, so derived state cannot be found. Code all alleles to `0`.
4. Different alleles (`DifAlt` in log): The ancestral allele is neither the reference nor the alternative, so no information is known. Code all alleles to `0`. 

### Notes, TODO

There are a few things left to do before release. 

- Properly scan file names to see if its gzipped or not.
- Scan for phased or unphased and use the correct delimiter. 
- Multi sample VCFs. 



