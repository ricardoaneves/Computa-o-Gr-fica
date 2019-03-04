#include "extractor.h"

void extractor(char** files, vector<coords>& triangles)
{
    int it = 0;
    double triple[3];

    for (int i = 0; files[i] != nullptr; i++) {

        ifstream file;
        file.open(files[i]);
        string line;

        if (file.is_open()) {
            while (getline(file, line)) {

                string temp;
                for (int i = 0; i <= line.length(); i++) {

                    if (line[i] == ' ' || line[i] == '\0') {
                        triple[it] = stod(temp);
                        temp.clear();
                        it++;
                    }
                    else
                        temp.push_back(line[i]);

                }

                coords aux = make_tuple(triple[0], triple[1], triple[2]);
                triangles.push_back(aux);
                it = 0;
            }
        }
    }
}