#include <fstream>

using namespace std;

int main(int argc, char *argv[]) {
    ofstream outfile("pulseload.txt");
    int t = 120;
    int periodHi = 10;
    int periodLow = 20;
    int valueHi = 10;
    int valueLow = 0;
    int mode = 0;   // Low
    for (int i = 0; i < t; i ++) {
        if (mode) {
            outfile << i << " " << valueHi << endl;
            if (i % (periodHi + periodLow) == periodHi + periodLow - 1) {
                mode = 0;
            }
        } else {
            outfile << i << " " << valueLow << endl;
            if (i % (periodHi + periodLow) == periodLow - 1) {
                mode = 1;
            }
        }
    }
    outfile.close();
}