CXX         = g++
CXXFLAGS    = -std=c++17 -I /opt/homebrew/opt/sfml/include -I include
LDFLAGS     = -L /opt/homebrew/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system
SOURCEFILES = src/*.cpp
BUILDFILE   = build/PONG.exe


compile:
	@$(CXX) $(CXXFLAGS) $(SOURCEFILES) -o $(BUILDFILE) $(LDFLAGS)
	@./$(BUILDFILE)