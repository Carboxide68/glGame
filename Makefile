debugFlags = 

flags = -std=c++17 -g

libs = -lGL -lGLU -lGLEW -lglfw

buildDir = build/

additionalFiles = common/common.cpp camera/camera.cpp glObjects/vao.cpp glObjects/vbo.cpp glObjects/ibo.cpp imgui/imgui.cpp 
additionalFiles += imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui_widgets.cpp imgui/examples/imgui_impl_opengl3.cpp 
additionalFiles += imgui/examples/imgui_impl_glfw.cpp misc/skybox.cpp mesh/mesh.cpp mesh/polygon.cpp
additionalFiles += mesh/model.cpp mesh/group.cpp

objFiles = ${notdir ${additionalFiles:.cpp=.o}}

main: main.cpp ${objFiles}
	g++ -o $@ $< ${objFiles} ${flags} ${libs}

debug: main.cpp ${objFiles}
	g++ -o $@ $< ${objFiles} ${debugFlags} ${flags} ${libs}

%.o: %.cpp
	g++ ${flags} -c $< -o $@

%.o: common/%.cpp
	g++ ${flags} -c $< -o $@

%.o: camera/%.cpp
	g++ ${flags} -c $< -o $@

%.o: glObjects/%.cpp
	g++ ${flags} -c $< -o $@

%.o: imgui/%.cpp
	g++ ${flags} -c $< -o $@

%.o: imgui/examples/%.cpp
	g++ ${flags} -c $< -o $@

%.o: mesh/%.cpp
	g++ ${flags} -c $< -o $@

%.o: misc/%.cpp
	g++ ${flags} -c $< -o $@

%.o: shader/%.cpp
	g++ ${flags} -c $< -o $@

clean:
	rm -f *.o main debug || true

loading: loading.cpp mesh/mesh.cpp mesh/triangle.cpp mesh/node.cpp common/common.cpp glObjects/ibo.cpp glObjects/vao.cpp glObjects/vbo.cpp camera/camera.cpp
	g++ -g -o loading loading.cpp mesh/mesh.cpp mesh/triangle.cpp mesh/node.cpp common/common.cpp glObjects/ibo.cpp\
	glObjects/vao.cpp glObjects/vbo.cpp camera/camera.cpp -lGL -lGLU -lGLEW -lglfw

test: test.cpp common/common.cpp mesh/group.cpp mesh/mesh.cpp mesh/model.cpp mesh/polygon.cpp
	g++ -o test test.cpp common/common.cpp mesh/group.cpp mesh/mesh.cpp mesh/model.cpp mesh/polygon.cpp -std=c++17 \
	-g -lGL -lGLU -lGLEW -lglfw
