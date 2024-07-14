//
// Created by asuka on 28.05.2024.
//

#ifndef MIRAI_APPLICATION_H
#define MIRAI_APPLICATION_H


class Application {
public:
    virtual void onCreate() = 0;
    virtual void onRender() = 0;
    virtual void onExit() = 0;
};


#endif //MIRAI_APPLICATION_H
