#include<string>
using namespace std;
class Application
{
public:
	static const std::string& data_path() { return data_path_; }
private:
	static std::string data_path_;//×ÊÔ´Ä¿Â¼
};