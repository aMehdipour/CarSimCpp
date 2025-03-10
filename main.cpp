/*#include "BicycleModel.h"*/
/*#include "EulerIntegrator.h"*/
/*#include "Sim.h"*/
#include "MFTireModel.h"
#include <iostream>
#include <json.hpp>
#include <fstream>
#include <LinearTireModel.h>

using json = nlohmann::json;

int main() {
    MFTireModel tireModel;

    std::ifstream configFile("config.json");
    json config;
    configFile >> config;
    json tireParams = config["tire"];
    json carParams = config["car"]; 

    tireModel.configure(tireParams);

    std::cout << "I'm gay" << std::endl;
    return 0;
}

