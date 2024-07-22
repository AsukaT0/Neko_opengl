//
// Created by asuka on 26.01.2024.
//

#include "AIModel.h"

#include <utility>
#include <File/File.h>
#include <Utils/Utils.h>
#include <Utils/StringUtils.h>
#include <random>

AIModel::AIModel(const std::string& index) {
    std::vector<File> modelFiles = File(index).listFiles();
    for(const File& f:modelFiles){
        std::string type = f.getName();
        if(type.find("preprocessor") != std::string::npos){
            preprocessor = torch::jit::load(f.getAbsolutePath());
        }else if(type.find("predictor") != std::string::npos){
            predictor = torch::jit::load(f.getAbsolutePath());
        }else if(type.find("postprocessor") != std::string::npos){
            postprocessor = torch::jit::load(f.getAbsolutePath());
        }
    }
}

std::string AIModel::forward(std::string inputData) {
    c10::Dict<std::string,std::vector<std::string>> rawInputs;
    rawInputs.insert("User1", std::vector<std::string>{std::move(inputData)});
    c10::IValue input{rawInputs};
    auto preprocessedData = preprocessor.forward({input});
    auto predictions = predictor.forward({preprocessedData});
    auto postprocessData = postprocessor.forward({predictions});
    c10::IValue preOutput = postprocessData.toGenericDict().at("Mira01::predictions");
    auto k = createVectorFromList(preOutput.toList().get(0).toList());
    std::stringstream test;
    for(const auto& ii1:k){
        test << ii1 << " ";}
    std::string finalOutput = test.str();
    return postProcDataFunc(finalOutput);
}




std::string AIModel::postProcDataFunc(const std::string &text) {
    std::cout << text << "\n";
    std::string startText = "<SOS>";
    std::string endText = "<EOS>";
    std::string outputText = text.substr(startText.length(),text.find(endText)-endText.length());
    outputText = StringUtils::trim(outputText);
    outputText = StringUtils::multiply_replace(outputText,{{" ,",","},{" .","."},{"]",""},{"[",""}});
    return outputText;
}
