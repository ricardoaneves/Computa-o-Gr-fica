#include "parser.h"

char** parser(char* xmlDoc)
{
    XMLDocument doc;
    XMLError error = doc.LoadFile(xmlDoc);

    int i = 0;

    char** files = (char**)malloc(sizeof(char*) * 15);

    char pre[4] = "../";
    char file[15];

    if (!error) {
        XMLElement* elem = doc.FirstChildElement();
        for (const XMLElement* child = elem->FirstChildElement(); child; i++, child = child->NextSiblingElement()) {

            char* tmp = (char*)child->Attribute("file");
            strcpy(file, tmp);

            char ready[30];
            ready[0] = '\0';

            strcat(ready, pre);
            strcat(ready, file);

            files[i] = (char*)malloc(sizeof(char) * 15);
            strcpy(files[i], ready);

            memset(file, 0, 15);
            memset(ready, 0, 30);
        }
    }

    return files;
}