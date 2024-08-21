
#include <string>
#include <vector>
#include <File/File.h>
#include <File/ConfigLoader.h>
#include <Utils/Utils.h>
#include <Utils/StringUtils.h>
#include <File/Archiver.h>
#include "src/AI/AIModel.h"
#include "src/Graphics/Window.h"
#include "src/Graphics/Objects/gl3D/Camera/PerspectiveCamera.h"
#include "src/Graphics/Objects/Loaders/ObjLoader.h"
#include "src/Controller.h"
#include "src/Graphics/Scene/GraphicsClass.h"
#include <Utils/Stopwatch.h>
#include <Net/Server/SServer6.h>
#include <sys/socket.h>
#include <csignal>

const std::string configPath = "config.json";
const std::string startData = "{\n"
                              "\t\"raw_model_path\":\"/media/asuka/YEEES/MiraAiModulator/result_torch_test/model/\",\n"
                              "\t\"restructured_model_path\":\"output_model/model\",\n"
                              "\t\"server\":\"true\",\n"
                              "\t\"port\":\"0\",\n"
                              "\t\"console\":\"true\","
                              "\t\"count\":\"-1\""
                              "\n}";

const std::string TEMP_FILE = ".tmpfile";
const std::string END_PROGRAM_LABEL = "\n\nВремя завершения программы";
const std::string END_RESTRUCTURED_LABEL = "\n\nВремя реструктуризации препроцессора";
Windower window{};

void startupFunc(){
    File configFile(configPath);
    if (configFile.fileExists()){
        Controller::config.loadFromPath(configPath);
        Controller::config.parseConfig();}
    else{
        configFile.write(startData);
        Controller::config.loadFromPath(configPath);
        Controller::config.parseConfig();
    }}

bool isNeedToUpdateData(const std::string& rawModelPath,const std::string& restructuredModelPath){
    if(!File(restructuredModelPath).fileExists()){
        File(File(restructuredModelPath).getAbsolutePath()).makedir();
    }
    File tmp(TEMP_FILE);
    if(tmp.fileExists()){
        std::string readTmp = tmp.read();
        std::vector<std::string> info = StringUtils::split(readTmp,"\n");
        std::vector<File> k = File(rawModelPath).listFiles();
        info.pop_back();

        int ii = 0;
        for(const File& f:k){
            if(f.getName().ends_with(".pt")){
                if(""+std::to_string(f.size())+";"+std::to_string(f.timeCreate()) != info[ii]){
                    auto files = File(restructuredModelPath).listChilds();
                    for(const File& fi:files){
                        fi.remove();}
                    return true;}
                ii+=1;}}
        return false;}
    else{
        auto files = File(restructuredModelPath).listChilds();
        for(const File& f:files){
            f.remove();}
        return true;}}

void modelRestructuringFunc(const std::string& rawModelPath,const std::string& restructuredModelPath){
    Stopwatch stopwatch{};
    stopwatch.start();
    std::string tmpSaver;
    if(!isNeedToUpdateData(rawModelPath,restructuredModelPath)){
        stopwatch.stop(END_RESTRUCTURED_LABEL);
        return;}
    std::vector<File> k = File(rawModelPath).listFiles();
    for(const File& f:k){
        if(f.getName().ends_with(".pt")){
            tmpSaver += ""+std::to_string(f.size())+";"+std::to_string(f.timeCreate())+"\n";
            f.copyTo(restructuredModelPath+f.getName());}}
    File(TEMP_FILE).write(tmpSaver);
    for(const File& f:File(restructuredModelPath).listFiles()) {
        if (f.getName().ends_with("preprocessor.pt")) {
            std::string temp = StringUtils::replace(f.getAbsolutePath(), ".pt", "_temp");
            File(temp).makedir();
            Archiver::extractArchive(f.getAbsolutePath(), temp);
            f.remove();}}
    std::string folderReModeling = File(restructuredModelPath).listFolder()[0].getName();
    std::string pathToFile = restructuredModelPath+folderReModeling+"/inference_preprocessor/code/__torch__/ludwig/models/inference.py";
    File if1(pathToFile);
    std::string readFromFile = StringUtils::replace(if1.read(),"Union[Tensor, List[Tuple[Tensor, int]], List[Tensor], List[str]]","List[str]");
    readFromFile = readFromFile.substr(0,readFromFile.length()-1);
    if1.write(readFromFile);
    Archiver::createArchive(restructuredModelPath+folderReModeling,restructuredModelPath+"inference_preprocessor.pt");
    File(restructuredModelPath+folderReModeling).remove();
    stopwatch.stop(END_RESTRUCTURED_LABEL);
}

void serverProcessing(ConfigLoader config, AIModel& miraAI){
    SServer6::start(0,config.getInt("port"),[&](const int socket_id){
        constexpr int bufferS = 2048*32;char buffer[bufferS] = {0};std::string message;
        read(socket_id, buffer, bufferS);
        message = buffer;
        if (message.empty())return;
        message = miraAI.forward(message);
        send(socket_id, message.c_str(), message.length(), 0);
        close(socket_id);});}

void consoleProcessing(ConfigLoader config,AIModel& miraAI){
    std::string consoleInput;
    unsigned int count = config.getInt("count");
    if(count > 0){
        for (unsigned int ii1 = 0;ii1 < count;ii1++){
            std::getline(std::cin,consoleInput);
            std::string consoleOutput = miraAI.forward(consoleInput);
            std::cout << consoleOutput << "\n";}}
    else{
        while(true){
            std::getline(std::cin,consoleInput);
            std::string consoleOutput = miraAI.forward(consoleInput);
            std::cout << consoleOutput << "\n";}}}

std::thread th3,th4;
void createAnything(){
    startupFunc();
    modelRestructuringFunc(Controller::config.getString("raw_model_path"),Controller::config.getString("restructured_model_path"));
    AIModel miraAI(Controller::config.getString("restructured_model_path"));
    th3 = std::thread([&]{
        if(Controller::config.getSubConfig("server").getBool("active")){
            serverProcessing(Controller::config.getSubConfig("server"), miraAI);
        }});
    th4 = std::thread([&]{
        if(Controller::config.getSubConfig("console").getBool("active")){
            consoleProcessing(Controller::config.getSubConfig("console"), miraAI);
        }});}
void onExit(int signal){
    window.onExit();
    exit(signal);
}
int main() {

    std::signal(SIGINT, onExit);
    std::signal(SIGTERM, onExit);
    Stopwatch t{};
    t.start();
    createAnything();

    GraphicsClass class1;
    window.createWindow(&class1);

    th3.join();
    th4.join();
    t.stop(END_PROGRAM_LABEL);

}
