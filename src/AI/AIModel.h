//
// Created by asuka on 26.01.2024.
//

#ifndef MIRAAI_AIMODEL_H
#define MIRAAI_AIMODEL_H

#include <torch/script.h>
#include <torch/torch.h>
#include <iostream>
#include <vector>
#include <string>
#include <torch/torch.h>
#include <torch/script.h>
#include <torch/data/datasets/base.h>
#include <torch/data/example.h>
#include <torch/data/dataloader.h>
#include <torch/types.h>


class AIModel {
    torch::jit::script::Module preprocessor;
    torch::jit::script::Module predictor;
    torch::jit::script::Module postprocessor;
private:
    static std::string postProcDataFunc(const std::string& text);
public:
    AIModel(const std::string& index);
    std::string forward(std::string inputData);
};


#endif //MIRAAI_AIMODEL_H
