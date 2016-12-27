
#include "vcfreader.h"
#include "util.h"
#include <sstream>

VcfReader::VcfReader(string faFile)
{
    // Set locale and disable stdio synchronization to improve iostream performance
    // http://www.drdobbs.com/the-standard-librarian-iostreams-and-std/184401305
    // http://stackoverflow.com/questions/5166263/how-to-get-iostream-to-perform-better
    setlocale(LC_ALL,"C");
    ios_base::sync_with_stdio(false);

    mVcfFile = faFile;
    if (is_directory(mVcfFile)) {
        string error_msg = "There is a problem with the provided vcf file: \'";
        error_msg.append(mVcfFile);
        error_msg.append("\' is a directory NOT a file...\n");
        throw invalid_argument(error_msg);
    }
    mVcfFileStream.open( mVcfFile.c_str(),ios::in);
    // verify that the file can be read
    if (!mVcfFileStream.is_open()) {
        string msg = "There is a problem with the provided vcf file: could NOT read ";
        msg.append(mVcfFile.c_str());
        msg.append("...\n");
        throw invalid_argument(msg);
    }

    char c;
    // seek to first contig
    while (mVcfFileStream.get(c) && c != '>') {
        if (mVcfFileStream.eof()) {
            break;
        }
    }
}

VcfReader::~VcfReader()
{
    if (mVcfFileStream.is_open()) {
        mVcfFileStream.close();
    }
}

bool VcfReader::readNext()
{

    string line = "";
    vector<string> items;

    getline(mVcfFileStream,line,'\n');

    if(line.length() == 0)
        return false;

    split(line, items, "\t");

    if(items.size() < 8)
        return false;

    if(items[0][0]=='#')
        return false;
    
    int pos =  atoi(items[1].c_str());
    Variant var;
    var.chrom = trim(items[0]);
    var.pos = pos;
    var.id = trim(items[2]);
    var.ref = trim(items[3]);
    var.alt = trim(items[4]);
    var.qual = trim(items[5]);
    var.filter = trim(items[6]);
    var.info = trim(items[7]);

    // format is not required
    if(items.size()>=9)
        var.format = trim(items[8]);

    mVariants.push_back(var);

    return true;
}


void VcfReader::readAll() {
    while(!mVcfFileStream.eof()){
        readNext();
    }
}

void VcfReader::printAll() {
    for(int i=0;i<mVariants.size();i++) {
        Variant& v = mVariants[i];

        cout <<v.chrom<<"\t";
        cout <<v.pos<<"\t";
        cout <<v.id<<"\t";
        cout <<v.ref<<"\t";
        cout <<v.alt<<"\t";
        cout <<v.qual<<"\t";
        cout <<v.filter<<"\t";
        cout <<v.info;
        if(v.format !="")
            cout <<"\t"<< v.format;
        cout <<endl;
    }
}

bool VcfReader::test(){
    VcfReader reader("testdata/tinyvcf.vcf");
    reader.readAll();
    //reader.printAll();
    return true;

}



