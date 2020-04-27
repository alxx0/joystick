//   Copyright 2020 Alexander Poulson
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#pragma once

class Console;
class ConsoleUI;

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <chrono>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "console.h"
#include "macros.h"
#include "joyfileproject.h"
#include "errors.h"
#include "executeprocess.h"

class JoyfileParser {
public:

    // Constructors
    JoyfileParser() {}
    JoyfileParser(std::string _path): path(_path) {}
    JoyfileParser(const JoyfileParser&) {}
    JoyfileParser& operator=(const JoyfileParser& jp);

    std::string getPath();
    std::string getError();

    bool parse(Console *parentConsole);

    // For installing the project
    std::map<std::string, std::string> getInstallLocation();
    std::map<std::string, std::string> getFileName();
private:
    std::ifstream stream;

    std::string path, error;

    std::vector<std::string> keywords = {
        "use"
    };

    std::vector<std::string> instructionList, sourcesInstructionList;
    
    bool isKeyword(std::string instruction) {
        return std::find(keywords.begin(), keywords.end(), instruction)
               == keywords.end();
    }

    int getPlatform() {
        #if defined(__APPLE__) || defined(__MACH__)
            return 0;
        #elif defined(_WIN32) || defined(_WIN64)
            return 1;
        #elif defined(__linux__) || defined(linux) || defined(__linux)
            return 2;
        #endif

        return 3;
    }

    bool functionActions(std::string& instruction, Console* parentConsole);

    bool strAction(std::string& instruction);

    std::string line, instruction, finalInstruction, projectInstruction, platformInstruction, functionInstruction, otherInstruction, pathSimple;
    unsigned int linePlace = 0, openedPlatformSpecifics = 0;
    bool isFunction, isComment, isVariable, isParameter, isProject,
        isDarwin, isWindows, isLinux, isPlatformSpecific, event, hasStartedNewLine;
    std::string variable;

    JoyfileProject joyfileProject;

    std::ifstream lastCompileTimeStreamRead;
    std::time_t lastCompTime;

    std::map<std::string, std::string> installLocation, fileName;
};

struct JoystickTotemLibrary {
    static std::map<std::string, int> functions;
    static std::map<std::string, int> projectFunctions;

    static bool isFunction(std::string instruction);
    static bool isProjectFunction(std::string instruction);
};
