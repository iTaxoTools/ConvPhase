rm phased.fasta
../SeqPHASE/cpp/SeqPhase1 -1 samples/sample.fa
../phase/src/PHASE -p0.3 phase.inp phase.out
../SeqPHASE/cpp/SeqPhase2 -c phase.const -i phase.out
rm phase.*
