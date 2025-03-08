/*#include "BicycleModel.h"*/
/*#include "EulerIntegrator.h"*/
/*#include "Sim.h"*/
#include <iostream>
#include <json.hpp>
#include <fstream>
#include <LinearTireModel.h>

using json = nlohmann::json;

int main() {

    std::ifstream configFile("config.json");
    json config;
    configFile >> config;
    json tireParams = config["tire"];
    json carParams = config["car"]; 

    if (tireParams["model"] == "Linear") {
        LinearTireModel tireModel(tireParams["C_alphaF"], tireParams["C_alphaR"]);
    }

    std::cout << "I'm gay" << std::endl;
    return 0;
}

