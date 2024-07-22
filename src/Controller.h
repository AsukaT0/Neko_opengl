//
// Created by asuka on 19.04.2024.
//

#ifndef MIRAAI_CONTROLLER_H
#define MIRAAI_CONTROLLER_H


#include "Graphics/Window.h"

class Controller {
private:
    static std::map<std::string,std::string> stringRes;
public:
    static Windower *mainWindows;
    static ConfigLoader config;

};


#endif //MIRAAI_CONTROLLER_H
