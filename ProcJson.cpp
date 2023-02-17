#include <ProcJson.h>
#include <json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

ProcJson::ProcJson() 
{
    ProcJson::size = 0;
    ProcJson::idx = -1;
    ProcJson::currT = 0.0f;
    ProcJson::currX = 0.0f;
    ProcJson::currY = 0.0f;
    ProcJson::currZ = 0.0f;
};

ProcJson::~ProcJson() 
{

};

int ProcJson::read_data()
{
    
    std::ifstream f("trajectory.json");
    json data = json::parse(f);
    
    for (auto& it : data.items())
    {
        // std::cout << "key: " << it.key() << ", value: " << it.value()["t"] << '\n';
        
        ProcJson::t.push_back(it.value()["t"]);
        ProcJson::x.push_back(it.value()["xyz"][0]);
        ProcJson::y.push_back(it.value()["xyz"][1]);
        ProcJson::z.push_back(it.value()["xyz"][2]);
        ProcJson::size++;

    }
    ProcJson::size = ProcJson::t.size();
    std::cout << "Read data done.\n";
    return 0;
};

void ProcJson::get_next()
{
    ProcJson::idx++;
    if (ProcJson::idx == ProcJson::size) {
        ProcJson::idx = 0;
    }
    ProcJson::currT = ProcJson::t[ProcJson::idx];
    ProcJson::currX = ProcJson::x[ProcJson::idx];
    ProcJson::currY = ProcJson::y[ProcJson::idx];
    ProcJson::currZ = ProcJson::z[ProcJson::idx];
    std::cout << "Current values are changed.\n";
};