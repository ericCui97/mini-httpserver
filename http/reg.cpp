
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <vector>


static void getNumberByReg(std::string &str, std::vector<int>& res);
int main()
{
    // std::string s1("{1,2,3}");
    // std::regex e(R"(\d+)");

    // std::cout << s1 << std::endl;

    // std::sregex_iterator iter(s1.begin(), s1.end(), e);
    // std::sregex_iterator end;

    // while (iter != end) {
    //     std::cout << "size: " << iter->size() << std::endl;

    //     for (unsigned i = 0; i < iter->size(); ++i) {
    //         std::cout << "the " << i + 1 << "th match"
    //                   << ": " << (*iter)[i] << std::endl;
    //     }
    //     ++iter;
    //}
    std::vector<int> res;
    std::string s1("Range: bytes=0-1023)cdv2324\r\n");
    getNumberByReg(s1, res);
    std::vector<int>::iterator iter;
//    for(int i=0;i<res.size();i++){
//        std::cout<<res[i]<<std::endl;
//    }
    return 0;
}


static void getNumberByReg(std::string &str, std::vector<int>& res)
{
    std::regex e(R"(\d+)");
    std::sregex_iterator iter(str.begin(), str.end(), e);
    std::sregex_iterator end;

    while (iter != end) {
        for (int i = 0; i < iter->size(); ++i) {
            res.push_back(std::stoi((*iter)[i]));
        }
        ++iter;
    }
}