#ifndef APPLICATION_H
#define APPLICATION_H
#include<string>
using namespace std;
class Application
{
public:
	static const std::string& data_path() { return data_path_; }
private:
	static std::string data_path_;//��ԴĿ¼
};
#endif APPLICATION_H