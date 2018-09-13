#include <string>
#include <sstream>
#include <vector>
#include <iterator>

// This comes from https://stackoverflow.com/questions/236129/the-most-elegant-way-to-iterate-the-words-of-a-string

using namespace std;

vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

//std::ostringstream vec_to_string(std::vector<std::string> vec, char delim) {

//	std::ostringstream oss;

//	std::copy(vec.begin(), vec.end()-1,
  //      std::ostream_iterator<std::string>(oss, delim));

	// Now add the last element with no delimiter
//	oss << vec.back();

//	return oss;
//}


