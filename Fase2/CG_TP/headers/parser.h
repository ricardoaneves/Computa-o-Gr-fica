#ifndef CG_TP_PARSER_H
#define CG_TP_PARSER_H

#include <iostream>
#include "tinyxml2.h"
#include <string.h>
#include <vector>

using namespace tinyxml2;
using namespace std;

typedef struct aux {
    char* transf;
    double params[4];
    struct aux* next;
} Aux;

typedef struct second {
    char* transf;
    double params[4];
} Second;

typedef struct first {
    char* file;
    vector<Second*> transfs;
} First;

typedef Aux* Group[20];

void init(Group g);

void addAux(Group g, int i, char transf[], double x, double y, double z, double angle);

void addFile(char* file, Group g, vector<First*>& files);

void debug(vector<First*>& files);

void groupParser(const XMLElement* scene, Group g, int i, vector<First*>& files);

void parser(const char* config, vector<First*>& files);

#endif
