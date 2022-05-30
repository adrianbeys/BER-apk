#include "pch.h"
#include "stdint.h"

using namespace std;

void createFile1(const string name, const int count, const char value);
void printResult(berResults results);
berResults calculateBer(string fpath1, string fpath2);
uint8_t hammingDistance(uint8_t n1, uint8_t n2);

struct berResults
{
    double total_num; 
    double error_bits; 
    float ber;  
    clock_t t1; 
    clock_t t2; 
};

uint8_t hammingDistance(uint8_t n1, uint8_t n2)
{
    uint8_t x = n1 ^ n2; 
    uint8_t setBits = 0;
    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

uint8_t hammingDistance(uint8_t n1, uint8_t n2)
{
    uint8_t x = n1 ^ n2; 
    uint8_t setBits = 0;
    while (x > 0)
    {
        setBits += x & 1;
        x >>= 1;
    }
    return setBits;
}

void createFile1(const string name, const int count, const char value)
{
    fstream f;
    f.open(name.c_str(), ios::binary | ios::out);
    for (int i = 0; i < count; i++)
    {
        f.write((char*)&value, 1);
    }
    f.close();
}

berResults calculateBer(string fpath1, string fpath2)
{
    fstream f1, f2; 
    berResults results;
    results.t1 = 0;
    results.t2 = 0;
    results.ber = 0;
    results.err = 0;
    results.total_num = 0;

    saveLog("Calculating BER...");
    f1.open(fpath1.c_str(), ios::binary | ios::in);
    f2.open(fpath2.c_str(), ios::binary | ios::in);
    char a = 0x00;
    char b = 0x00;
    results.t1 = clock();

    while (!f1.eof())
    {
        f1 >> a; 
        f2 >> b; 
        if (!f1.eof()) 
        {
            results.err += hammingDistance(a, b);
            results.total_num += 8; 
        }
    }

    results.ber = (float)results.err / results.total_num; 
    results.t2 = clock();
    saveLog("BER calculations are done");
    return results; 
}

int main(int argc, char* argv[])
{
    string fpath1; 
    string fpath2; 
    berResults results;

    openLog("log.log"); 
    if (argc != 3) 
    {
        saveLog("User did not provide path to the files.");
        saveLog("Creating the test files for user...");
        //test 1
        createFile1("test1_file1.bin", 100, 0xFF); 
        createFile1("test1_file2.bin", 100, 0xFE); 
        //test 2
        // change 10 bits - omitting the function
        //test 3
        saveLog("Test files are prepared");
        saveLog("Re-run with correct arguments ie: ./task_iv_ber.exe test1_file1.bin test1_file2.bin");
    }
    else 
    {
        fpath1 = argv[1];
        fpath2 = argv[2];

        saveLog("Processing test files");
        results = calculateBer(fpath1, fpath2);
        printResult(results);
    }

    closeLog();
    return 0;
}

void printResult(berResults results)
{
    stringstream message;
    message << "Bit Error Result: " << results.ber << endl;
    message << "Total_number_of bits: " << results.total_num << endl;
    message << "Error: " << results.err << endl;
    message << "Calc time: " << ((float)results.t2 - results.t1) / CLOCKS_PER_SEC << " sec " << endl;
    saveLog(message.str());
}