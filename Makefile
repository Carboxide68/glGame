CC = g++

debugFlags = 

flags = -std=c++17 -g

libs = -Llib -lGL -lGLU -lGLEW -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp

includes = includes glm

includes := $(addprefix -I , $(includes))

buildDir = build/

additionalFiles = common/common.cpp camera/camera.cpp glObjects/vao.cpp glObjects/vbo.cpp glObjects/ibo.cpp imgui/imgui.cpp 
additionalFiles += imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui_widgets.cpp imgui/imgui_impl_opengl3.cpp 
additionalFiles += imgui/imgui_impl_glfw.cpp misc/skybox.cpp mesh/mesh.cpp mesh/polygon.cpp
additionalFiles += mesh/model.cpp mesh/group.cpp

objFiles = $(addprefix ${buildDir}, $(notdir $(additionalFiles:%.cpp=$(buildDir)%.o)))

main: main.cpp $(objFiles)
	$(CC) -o $@ $< $(includes) $(objFiles) $(flags) $(libs)

debug: main.cpp $(objFiles)
	$(CC) -o $@ $< $(includes) $(objFiles) $(debugFlags) $(flags) $(libs)

$(buildDir)%.o: %.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(buildDir)%.o: common/%.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(buildDir)%.o: camera/%.cpp
	$(CC) $(flags)  $(includes) -c $< -o $@

$(buildDir)%.o: glObjects/%.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(buildDir)%.o: imgui/%.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(buildDir)%.o: mesh/%.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(buildDir)%.o: misc/%.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

$(buildDir)%.o: shader/%.cpp
	$(CC) $(flags) $(includes) -c $< -o $@

clean:
	rm -f build/*.o main debug || true

loading: loading.cpp mesh/mesh.cpp mesh/triangle.cpp mesh/node.cpp common/common.cpp glObjects/ibo.cpp glObjects/vao.cpp glObjects/vbo.cpp camera/camera.cpp
	g++ -g -o loading loading.cpp mesh/mesh.cpp mesh/triangle.cpp mesh/node.cpp common/common.cpp glObjects/ibo.cpp\
	glObjects/vao.cpp glObjects/vbo.cpp camera/camera.cpp -lGL -lGLU -lGLEW -lglfw

test: test.cpp common/common.cpp mesh/group.cpp mesh/mesh.cpp mesh/model.cpp mesh/polygon.cpp
	g++ -o test test.cpp common/common.cpp mesh/group.cpp mesh/mesh.cpp mesh/model.cpp mesh/polygon.cpp -std=c++17 \
	-g -lGL -lGLU -lGLEW -lglfw
