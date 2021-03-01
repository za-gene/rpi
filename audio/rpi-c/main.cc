#include <fstream>
#include <sstream>

std::string slurp(const char *filename)
{
        std::ifstream in;
        in.open(filename, std::ifstream::in | std::ifstream::binary);
        std::stringstream sstr;
        sstr << in.rdbuf();
        in.close();
        return sstr.str();
}

int main()
{
	return 0;
}

