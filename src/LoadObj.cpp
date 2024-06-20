#include <bits/stdc++.h>

using namespace std;

struct vertex {
    float a, b, c;
    vertex(float a_ = 0, float b_ = 0, float c_ = 0) {
        a = a_;
        b = b_;
        c = c_;
    }
};

void getMesh(string fname, string outputFName)
{
    
    fstream file(fname.c_str(),std::fstream::in | std::fstream::ate); //ate -> 移到最後面
    int sz = file.tellg();  //回傳當前指針位置( ate在尾巴 -> 檔案大小 )
    file.seekg(0); //移到頭
    //string data;
    //data.resize(sz);
    //file.read((char*)data.data(), sz);
    //file.close();
    vector<float>tp;
    string s;
    float x, y, z;
    vector<vertex> v(1);
    while (file >> s) {
        //cout << fname << "\n";
        if (s == "v") {
            file >> x >> y >> z;
            v.push_back(vertex(x * 100, y*100, z*100));
        }
        else if (s == "f") {
            char x;
            int a, b, c;
            for (int i = 0; i < 3; i++) {
                file >> a >> x >> b >> x >> c;
                if (a > v.size()) {
                    cout << fname << "\n";
                    cout << a << "\n";
                }
                tp.push_back(v[a].a); tp.push_back(v[a].b); tp.push_back(v[a].c);
            }
        }
    }
    file.close();

    // 寫檔案
    std::ofstream outFile(outputFName);

    if (!outFile) {
        std::cerr << "nono。" << std::endl;
        return ;
    }
    outFile << tp.size()/3 << " 3\n"; 
    for (auto num : tp) {
        outFile << num << " ";
    }

    outFile.close();

    cout <<fname << " " <<tp.size() << "\n";
}

int main(){
    string inputF = "D:\\school\\Visualization\\src\\asset\\Surface\\fountain.obj";
    string outputF = "D:\\school\\Visualization\\src\\asset\\Surface\\fountainSurface.txt";
    getMesh(inputF, outputF);
}