### A tiny tool for recoding alleles in a VCF. 

This aims to be an easy way to recode alleles from major/minor to ancestral/derived. You only need:

- A `.vcf(.gz)` (Phased, or unphased, but you must specify this).
- A `.fasta(.gz)` with ancestral states of each base. 

Usage is pretty straightforward:

```{sh}
aavcf   -v your.vcf(.gz) \
        -f your.fasta(.gz) \
        -l your.log \
```
 
I've also provided `vcf`, `vcf_line`, and `fasta` classes in `fasta.cpp/.h` for extensibility. Each of these have a `read`,`write`, and `get_allele` (either for a particular entry or position) methods and are just abstractions of `std::vector` and `std::string` read in directly from data.
  
#### Installation

*Short version*: Same as any project build on `CMake`.

*Slightly longer version*: 

```{sh}
cd src; cmake
make
```

#### Usage Flags

- `-v / --vcf`: The path to a `.vcf(.gz)` file in `4.1-4.3` file format. 
    - Currently only one sample VCFs are acceptable input.
- `-f / --fasta`: The path to a `.fasta(.gz)` file on the same build as the VCF (i.e. the genomic coordinates are the same).
- `-l / --log`: Path to a log file to store all information on actions taken.
    - Defaults to `./log.txt`.
- `-h / --help`: Will unsurprisingly show you the help menu. 

### Actions

`aavcf` will do one of four things for each variant in the VCF:

1. Do nothing: If the ancestral allele is already the reference, no action is taken.
2. Flip alleles (`Flip` in log): If the ancestral is the alternate, the coding is flipped.
3. Uninformative allele (`UnInfo` in log): The ancestral allele is unknown, so derived state cannot be found. Code all alleles to `0`.
4. Different alleles (`DifAlt` in log): The ancestral allele is neither the reference nor the alternative, so no information is known. Code all alleles to `0`. 


#### C++ Class usage

To construct a `fasta`:

```{c++}
fasta new (std::string path_to_fasta);
```

- `new.get_allele(unsigned int pos)` will retrieve an allele at a position.

To construct a `vcf`

```{c++}
vcf new (std::string path_to_vcf, char delim = '\t')
```

- `new.entries` is a `std::vector<vcf_line>`, so the `n`th variant is retrieved with `new.entries[i]`.
- Each entry in the VCF for each variant is mutable and can be accessed by its standard name: `new.entries[i].id = "rs1"`.
- To write the VCF out, including meta data, simply call the `new.write(std::string path)` method. 



