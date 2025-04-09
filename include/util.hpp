#pragma once

char *argsShift(int *argc, char ***argv);
std::string readFile(const std::string &filename);
bool replace(std::string &str, const std::string &from, const std::string &to);
