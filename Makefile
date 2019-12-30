debugFlags = -g

flags = -std=c++17

libs = -lGL -lGLU -lGLEW -lglfw

additionalFiles = common/common.cpp

objFiles = ${notdir ${additionalFiles:.cpp=.o}}

main: main.cpp ${objFiles}
	g++ -o $@ $< ${objFiles} ${flags} ${libs}

%.o: %.cpp
	g++ -c $< -o $@ 

%.o: common/%.cpp
	g++ -c $< -o $@

clean:
	rm -f *.o main debug || true