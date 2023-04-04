# Demonstration of my C Skills

This repository is a compilation of a few projects I've built in C.

* Pool allocator: an implementation of `malloc` that works by keeping track of free
and used blocks, and frequently coalesces to reduce memory fragmentation. View at `mem_alloc/`.

* Networked filesystem: view the repository [here](https://github.com/mattnappo/tfs). It is not a real
filesystem like ext4 or fat32, and is much simpler. However, it emulates a filesystem by keeping track of
file reads and writes to a large buffer. There is also a TCP server that hosts the "filesystem," making it
available to a network. This project was a great educational exercise.

* Set notation parser: a recursive-descent and table-driven parser for basic mathematical set algebra statements
such as `{1,2,3}U{4,5,6}`. View at `parser/`.

* Finite state automaton simulator: Implementations of NFAs and DFAs, and the powerset construction algorithm to convert a NFA into a DFA. There are also a few toy FSAs that recognize simple languages. View at `fsa/`.

Other non-C projects I am proud of include:
* [Spark](https://github.com/mattnappo/spark): A personal keyserver and secret manager
* [Harbor](https://github.com/mattnappo/harbor): A WIP distributed filesystem
* A simplified Paxos implementation: view at `paxos/`
