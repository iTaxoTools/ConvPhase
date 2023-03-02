ConvPhase - A convinient phase program

The purpose of this program is to combine the phase and SeqPHASE project into one program and make most of the configurations available. The project is intended to be used as library, but can also be used as standalone executable

Unfortunately the communication between the programs is done by using files, because phase only accept an input and an output file and SeqPHASE also accepts only input files and has fixed output files. The final output after executing both SeqPHASE steps and phase will be in phased.fasta.

Requirements:
haxe
hxcpp
