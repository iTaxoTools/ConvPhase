#include "fastaconverter.h"

#include <regex>

#define VALID_CHARS "ACGTUIRYKMSWBDHVN-"

FastaConverter::FastaConverter(std::string in, FastaConverterFormat f){
	std::string crlf{"\r\n"};
	if(in.find(crlf) != std::string::npos){
		fprintf(stderr, "Warning: data is in CRLF format\n");
		std::regex re{crlf};
		in = std::regex_replace(in, re, "\n");
	}
	
	switch(f){
		case FCF_NONE:
			parse(in);
			break;
		case FCF_FASTA:
			parseFasta(in);
			break;
		case FCF_MOID_FASTA:
			parseMoIDFasta(in);
			break;
		case FCF_HAPVIEW_FASTA:
			parseHapViewFasta(in);
			break;
		case FCF_NEXUS:
			parseNexus(in);
			break;
		default:
			fprintf(stderr, "Error: Format not supported for parsing\n");
	}
}

FastaConverter::operator std::string(){
	return toString();
}

inline bool fastaCheck(std::string in, std::string sep = ""){
	std::string seqNameRe = ".*";
	if(sep.size())
		seqNameRe += sep + seqNameRe;

	std::string newLine = "\r\n";
	std::string seqIdRe{">" + seqNameRe};
	std::string oneLineSeqDataRe{"[" VALID_CHARS "]+"};
	std::string seqDataRe = oneLineSeqDataRe + "(\n" + oneLineSeqDataRe + ")*";
	std::string seqRe = seqIdRe + "\n" + seqDataRe + "\n*";
	std::string re = seqRe + "(\n" + seqRe + ")*";

	std::regex validRe{re};

	return std::regex_match(in, validRe);
}
FastaConverter& FastaConverter::parse(std::string in){
	if(fastaCheck(in)){
		if(fastaCheck(in, "\\|")){
			return parseMoIDFasta(in);
		}
		if(fastaCheck(in, "\\.")){
			return parseHapViewFasta(in);
		}
		return parseFasta(in);
	}

	fprintf(stderr, "Error: Format not recognized for parsing\n");
	return *this;
}
inline std::string parseFastaSequence(std::string in){
	std::string seq;
	std::regex dataRe{"\n([>" VALID_CHARS "]+)*"};
	std::smatch dataMatch;
	while(std::regex_search(in, dataMatch, dataRe)){
		if(dataMatch[0].str()[1] == '>')
			break;
		seq += dataMatch[1];
		in = dataMatch.suffix();
	}
	return seq;
}
FastaConverter& FastaConverter::parseFasta(std::string in, std::string sep){
	std::string seqNameRe = "(.*)";
	if(sep.size())
		seqNameRe += sep + seqNameRe;
	else
		format = FCF_FASTA;

	if(!fastaCheck(in, sep))
		fprintf(stderr, "Warning: Data not in specified fasta format!\n");

	std::regex idRe{">" + seqNameRe};
	std::smatch idMatch;
	while(std::regex_search(in, idMatch, idRe)){
		Sequence seq;
		seq.seqid = idMatch[1].str();
		if(sep.size())
			seq.taxon = idMatch[2].str();
		in = idMatch.suffix();

		seq.data = parseFastaSequence(in);

		sequences.push_back(seq);
	}
	return *this;
}
FastaConverter& FastaConverter::parseMoIDFasta(std::string in){
	if(!format)
		format = FCF_MOID_FASTA;
	return parseFasta(in, "\\|");
}
FastaConverter& FastaConverter::parseHapViewFasta(std::string in){
	if(!format)
		format = FCF_HAPVIEW_FASTA;
	return parseFasta(in, "\\.");
}
FastaConverter& FastaConverter::parseNexus(std::string in){
	if(!format)
		format = FCF_NEXUS;
	return *this;
	//TODO
}

std::string FastaConverter::toString(){
	switch(format){
		case FCF_NONE:
		case FCF_FASTA:
			return getFasta();
		case FCF_MOID_FASTA:
			return getMoIDFasta();
		case FCF_HAPVIEW_FASTA:
			return getHapViewFasta();
		case FCF_NEXUS:
			return getNexus();
		default:
			fprintf(stderr, "Error: fasta format not supported for toString\n");
			return "";
	}
}
std::string FastaConverter::getFasta(std::string sep){
	std::string out;

	for(Sequence const& seq: sequences){
		out += ">";
		out += seq.seqid;
		if(sep.size()){
			out += seq.allele;
			out += sep;
			out += seq.taxon;
		} else{
			out += seq.taxon;
			out += seq.allele;
		}
		out += "\n";
		out += seq.data;
		out += "\n";
	}

	return out;
}
std::string FastaConverter::getMoIDFasta(){
	return getFasta("|");
}
std::string FastaConverter::getHapViewFasta(){
	return getFasta(".");
}
std::string FastaConverter::getNexus(){
	std::string out;
	return out;
	//TODO
}

void FastaConverter::clear(){
	sequences.clear();
	format = FCF_NONE;
}
