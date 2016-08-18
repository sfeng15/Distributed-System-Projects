// Class splitstring which adds method split()
// define MAIN if this is a standalone program
//#define MAIN 0
#include "spliter.h"
using namespace std;

vector<string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    string work = data();
    string buf = "";
    int i = 0;
    while (i < work.length()) {
        if (work[i] != delim)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        } else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
    return flds;
}

// testing program and sample of how to use the class
/*#ifdef MAIN
main()
{
    // we define a string
    char msg[300]="Humpty Dumpty sat on a wall.   Humpty Dumpty had a great fall";
    splitstring s(msg);
    cout << s << endl;

    // splits and displays the vector of strings
    vector<string> flds = s.split(' ');
    for (int k = 0; k < flds.size(); k++)
        cout << k << " => " << flds[k] << endl;

    // now taking account of repeated delimiters
    cout << endl << "with repeated delimiters:" << endl;
    vector<string> flds2 = s.split(' ', 1);
    for (int k = 0; k < flds2.size(); k++)
        cout << k << " => " << flds2[k] << endl;

}
#endif*/
