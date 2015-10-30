# N-Way-Set-Associative-Cache
Direct Mapped and N-Way set associative cache Simulator in C/C++ for L1 cache in Processors
# Creating Binary?

Use the ```make ``` command in linux terminal to get your binaary

# Command Parameters?

Level 1 Data Cache Simulator should accept the following command-line options:

-   -s < split > or < unified >.

-   -c < capacity > with < capacity > in KB: 4, 8, 16, 32, or 64.

-   -b < blocksize > with < blocksize > in bytes: 4, 8, 16, 32, 64, 128, 256, or 512.

-   -a < associativity > where < associativity > is integer size of set: 1, 2, 4, 8 or 16.

for example
```sh 
Unified: $ ./Cache -c8 -b16 -a4 < cc.trace > output.txt
Split  : $ ./Cache -c8 -b16 -a4 -s < cc.trace  > output.txt
```
>-s option specifies a split cache. This option indicates that the L1 cache is split equally
into L1D (Data Cache) and L1I (Instruction Cache). The -c option gives the  combined size of the L1
cache, split equally between L1D and L1I. The block size and associativity is the same for both L1D and
L1I. If the -s option is not given then the cache is unif ied by default, as before (i.e. instruction reads are
also treated as data reads).

# Write Policies on Hit/Miss?
The following functionality is added to handle data write hits and misses with
optional command-line options:
-  ```--wbwa ``` Write Back / Write Allocate [Default]
-  ```--wbwn ``` Write Back / Write No-Allocate
-  ```--wtwa ``` Write Through / Write Allocate
-  ```--wtwn ```Write Through / Write No-Allocate

# Trace Format and Usage?
- The input to the cache simulator is a sequence of memory access traces, one per line, terminated by
end of file. In the following format, with a leading 0 for data loads, 1 for data stores and 2 for instruction
load.
```sh
0 <address>
1 <address> <dataword>
2 <address>
```

# Structure of Program?
The Program was developed in such a way that there were no difference between Direct mapped
and and N-Way set associative cache as per code. The memory was unstructured and linear.
However the cache was structured in such a way that the The class cache was set-up as per the
associativity with the array size of 2^Index-bits. And each cache set had the page replacement
policy implemented within which had the associativity number of cache-lines object as a single
cache line which further contained the basic bits , tags and data like validity, Dirty. The data was
further divided into block-size (in words) number of words where 1 word was equal to 4 bytes. The
policies implemented for page replacement was LRU and was mapped in cache set as K-matrix
LRU algorithm. Figure below shows the bird's eye view of the cache structure.

[<img src="https://lh3.googleusercontent.com/gusy4qxgmFBtmAkG0iGMjbB3Dt6NKFFNYceJKvi2FsJq8IYFm07prxVLgHhW0CcigW_gMRrOLdS0hFsQ_01KdrMdmX926KHmgdVzfz4AZvMF_Kxx9Lf2M6crj2ta8Siu98mNDOEppn1os9MZ_WP_XWS5_f8PBB3UCgE_ZluFwnjpFcU3rsFOxm2ZznXzHJu0b0kLHQjxkvcQDdoYW0pbzdhp0nPZcp7K8NyveIJUIn9DbEXNRvpuXnj7iMva5qd4t_kMk6C8lqBUU_feRJwlIAuxfiV4eN6ODJd0zwjjdmX0uPtcKcTihH3LEJG04aCSnC50x-xAX0hncb0aFrfu4SuSX2Xbqf6M-RoLcw6FLxXBh_OR3z0eJv4I-zMDd4QTNJbcAhTZK0dhnf2-NGzf8ye1Veglx-SJQBOzz8FJMymUIa6j9KS0VjXJArMZqOv9P_aJl3Zp3SyUFYhc9zrRzbqIwHorAF5zb_kYU7e5wAJ9MPNSMv-Xlp9BKTRr3LFHpcMVpm3jAxKoyKJYfhqqEBdWoEDDTOAg2KB11mgl16U=w383-h393-no">](#Image)

License
----

GNU GENERAL PUBLIC LICENSE

