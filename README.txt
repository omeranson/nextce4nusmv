These instructions are based on NuSMV's installation instructions. Please refer
to nusmv/README for more information.
These instructions were tested on the server nova, as well as on Fedora Core Linux.
However, they should work on any Linux installation, provided the required 
libraries are installed.

1. Extract the source tarball.
2. Run: cd cudd-2.4.1.1
3. Run one of the following commands:
	a. For x86 linux run: make
	b. For 64-bit linux run: make -f Makefile_64bit
4. Run: cd ../nusmv
5. Run: ./configure 
	Note, nextce was not tested with Minisat solvers. Additionally, it does
	not use bounded model checking, so these should not affect it.
6. (Optionally), run the following command to generate help files:
	a. For tcsh and similar: setenv NuSMV_LIBRARY_PATH $cwd/share
	b. For bash and similar: NuSMV_LIBRARY_PATH='pwd'/share; export NuSMV_LIBRARY_PATH
7. Run: make

NuSMV should compile. Once compiled, a NuSMV binary is created and can bea executed.

Usage:
1. Run: ./NuSMV -int <filename.smv>
2. (Optionally) Set ce_equivalence class (1-4): set ce_equivalence <class>
3. Run: go
4. Run: next_ce to get the next counter-example
4. (Alternatively) Run: compute_all to get all counterexamples.

To add debug information, run: export NEXTCE_DEBUG=5 (for bash-like shells)
