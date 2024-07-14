cmake_minimum_required(VERSION 3.25)
set(PROJECT_NAME Mirai)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 23)

set(MIRAI_SOURCE
        src/Controller.cpp src/Controller.h
        src/AI/AIModel.cpp src/AI/AIModel.h
        src/Graphics/Window.cpp src/Graphics/Window.h
        src/Graphics/Objects/gl3D/Model/Model3D/Model.cpp src/Graphics/Objects/gl3D/Model/Model3D/Model.h
        src/Graphics/Buffers/FrameBuffer.cpp src/Graphics/Buffers/FrameBuffer.h
        src/Graphics/Math/Structs.h
        src/Graphics/Math/Functionality.cpp src/Graphics/Math/Functionality.h
        src/Graphics/Scene/Scene.cpp src/Graphics/Scene/Scene.h
        src/Graphics/Shaders/ShaderProgram.cpp src/Graphics/Shaders/ShaderProgram.h
        src/Graphics/Objects/Materials/Material.cpp src/Graphics/Objects/Materials/Material.h
        src/Graphics/Objects/Loaders/ObjLoader.cpp src/Graphics/Objects/Loaders/ObjLoader.h
        src/Graphics/Objects/gl3D/Camera/PerspectiveCamera.cpp src/Graphics/Objects/gl3D/Camera/PerspectiveCamera.h
        src/Graphics/Objects/gl3D/ModelBuilder.cpp src/Graphics/Objects/gl3D/ModelBuilder.h
        src/Graphics/Objects/gl3D/Flat/FlatModels.cpp src/Graphics/Objects/gl3D/Flat/FlatModels.h
        src/Graphics/Objects/gl3D/BaseModel.cpp src/Graphics/Objects/gl3D/BaseModel.h
        src/Graphics/Objects/gl3D/Lights/DiffusionLight.cpp src/Graphics/Objects/gl3D/Lights/DiffusionLight.h
        src/Graphics/Objects/gl2D/Texture.cpp src/Graphics/Objects/gl2D/Texture.h
        src/Workers/GraphicsClass.cpp src/Workers/GraphicsClass.h
        src/Workers/Application.cpp src/Workers/Application.h
        src/Graphics/Objects/gl3D/Model/Bones/Bone.cpp src/Graphics/Objects/gl3D/Model/Bones/Bone.h
        src/Graphics/Objects/gl3D/Model/Bones/Skeleton.cpp src/Graphics/Objects/gl3D/Model/Bones/Skeleton.h
        src/Graphics/Objects/gl2D/TextureUVMap.cpp src/Graphics/Objects/gl2D/TextureUVMap.h
        src/Graphics/Objects/gl3D/Model/Model3D/HumaModel.cpp src/Graphics/Objects/gl3D/Model/Model3D/HumaModel.h src/Graphics/Objects/gl3D/Animations/BoneAnimation.cpp src/Graphics/Objects/gl3D/Animations/BoneAnimation.h src/Graphics/Objects/gl3D/Animations/AnimationController.cpp src/Graphics/Objects/gl3D/Animations/AnimationController.h src/Graphics/Objects/Loaders/BALoaders.cpp src/Graphics/Objects/Loaders/BALoaders.h src/Graphics/Buffers/BO.cpp src/Graphics/Buffers/BO.h)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -mpopcnt")
add_executable(${PROJECT_NAME} main.cpp ${MIRAI_SOURCE} )

set(SOIL_INCLUDE_DIRS /home/asuka/Programs/libs/soil/build/include/)
set(SOIL_LIBRARIES /home/asuka/Programs/libs/soil/build/lib/libSOIL.so /home/asuka/Programs/libs/soil/build/lib/libSOIL.a)
include_directories(${SOIL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SOIL_LIBRARIES})

set(Torch_DIR /home/asuka/Programs/libs//libtorch/share/cmake/Torch)
find_package(Torch REQUIRED)
target_link_libraries(${PROJECT_NAME} "${TORCH_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} "${TORCH_CUDA_LIBRARIES}")

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})

find_package(LibArchive REQUIRED)
include_directories(${LIBARCHIVE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LIBARCHIVE_LIBRARIES} -larchive )

set(ONNX_INCLUDE_DIRS /home/asuka/Programs/libs/onnxruntime/include)
set(ONNX_LIBRARIES /home/asuka/Programs/libs/onnxruntime/lib/libonnxruntime.so)
include_directories(${ONNX_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ONNX_LIBRARIES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} GLEW::GLEW -lGL -lGLU -lglut -lX11 -lglfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

include(/home/asuka/Programs/libs/MyModules/MyLibConfig.cmake)