#include "dbscan.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/time.h>
using namespace std;

struct vec4f {
    float data[4];
    float operator[](int idx) const { return data[idx]; }
};

struct vec3f {
    float data[3];
    float operator[](int idx) const { return data[idx]; }
};


std::vector<std::string> splitWithStl(const std::string &str, const std::string &pattern)
{
	std::vector<std::string> resVec;

	if ("" == str)
	{
		return resVec;
	}
	//方便截取最后一段数据
	std::string strs = str + pattern;

	size_t pos = strs.find(pattern);
	size_t size = strs.size();

	while (pos != std::string::npos)
	{
		std::string x = strs.substr(0, pos);
		resVec.push_back(x);
		strs = strs.substr(pos + 1, size);
		pos = strs.find(pattern);
	}

	return resVec;
}

float stringToFloat(const string &i)
{
	using namespace std;
	stringstream sf;
	float score = 0;
	sf << i;
	sf >> score;
	return score;
}

std::vector<vec3f> openFile(char* filename)
{
    std::vector<vec3f> dataset;
	using namespace std;
	fstream file;
	file.open(filename, ios::in);
	if (!file)
	{
		cout << "Open File Failed!" << endl;
	}
    
	int i = 1;
	int idx_pt = 0;
	while (!file.eof())
	{
		string temp;
		file >> temp;
		std::vector<std::string> resVec = splitWithStl(temp, ",");
		stringToFloat(resVec[0]);
        // vec3f data{resVec[0].f,resVec[1].f,resVec[2].f};
        vec3f data{stringToFloat(resVec[0]),stringToFloat(resVec[1]),stringToFloat(resVec[2])};
        // data[0]=resVec[0];
        // data[1]=resVec[1];
        // data[2]=resVec[2];
        dataset.push_back(data);
	}
	file.close();
	// cout << "successful!" << endl;
	return dataset;
}

int main() {

    // auto dbscan = DBSCAN<vec4f, float>();

    // auto data = std::vector<vec4f>{
    //     vec4f{ 0.f, 0.f, 1.f, 1.f }
    //     , vec4f{ 0.f, 0.f, 1.f, 0.9f }
    //     , vec4f{ 0.f, 0.f, 0.9f, 0.9f }
    //     , vec4f{ 0.f, 0.f, 0.f, 0.9f }
    //     , vec4f{ 0.f, 0.f, 0.f, 0.8f }
    //     , vec4f{ 0.f, 0.f, 0.1f, 0.8f }
    // };

    auto data=openFile("feature.txt");
    auto dbscan = DBSCAN<vec3f, float>();

    // auto data = std::vector<vec3f>{
    //     vec3f{ 0.f, 0.f, 1.f}
    //     , vec3f{ 0.f, 0.f, 1.f}
    //     , vec3f{ 0.f, 0.f, 0.9f}
    //     , vec3f{ 0.f, 0.f, 0.f}
    //     , vec3f{ 0.f, 0.f, 0.f }
    //     , vec3f{ 0.f, 0.f, 0.1f }
    // };



    /** 
    * @describe: Run DBSCAN clustering alogrithm
    * @param: V {std::vector<T>} : data
    * @param: dim {unsigned int} : dimension of T (a vector-like struct)
    * @param: eps {Float} : epsilon or in other words, radian
    * @param: min {unsigned int} : minimal number of points in epsilon radian, then the point is cluster core point
    * @param: disfunc {DistanceFunc} :!!!! only used in bruteforce mode.  Distance function recall. Euclidian distance is recommanded, but you can replace it by any metric measurement function
    * @usage: Object.Run() and get the cluster and noise indices from this->Clusters & this->Noise.
    * @pitfall: If you set big eps(search range) and huge density V, then kdtree will be a bottleneck of performance
    * @pitfall: You MUST ensure the data's identicality (TVector* V) during Run(), because DBSCAN just use the reference of data passed in.
    * @TODO: customize kdtree algorithm or rewrite it ,stop further searching when minimal number which indicates cluster core point condition is satisfied
    */
    // int Run(TVector* V, const uint dim, const Float eps, const uint min, const DistanceFunc& disfunc = [](const T& t1, const T& t2)->Float { return 0; });
    struct timeval t1, t2;
    for (int i=0;i<10;i++)
    {
        gettimeofday(&t1, NULL);
        dbscan.Run(&data, 3, 0.25f, 100);
        gettimeofday(&t2, NULL);
        double deltaT = (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;
        std::cout << "time_comsumed:" << deltaT / 1000000 << std::endl;
    }
    
    auto noise = dbscan.Noise;
    auto clusters = dbscan.Clusters;
    int aaa=1;
    return 0;

}