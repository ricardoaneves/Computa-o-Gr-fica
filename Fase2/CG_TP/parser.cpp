#include "parser.h"

void debug(vector<First*>&  files)
{
    for (vector<First*>::iterator it = files.begin(); it != files.end(); it++) {

        First* tmp = *it;
        cout << "File name: " << tmp->file << endl;

        vector<Second*> aux = tmp->transfs;

        for (vector<Second*>::iterator it2 = aux.begin(); it2 != aux.end(); it2++) {
            Second* tmp2 = *it2;
            cout << "Transformation: " << tmp2->transf << endl;
            cout << "X = " << tmp2->params[0] << " " << "Y = " << tmp2->params[1] << " " << "Z = " << tmp2->params[2] << " " << "Angle = " << tmp2->params[3] << "\n" << endl;

        }
    }
}

void init(Group g)
{
    for (int i = 0; i < 20; i++)
        g[i] = NULL;

}

void addFile(char* file, Group g, vector<First*>& files)
{
    First* first = new First();
    first->file = new char[15];

    vector<Second*> transfs;

    for (int i = 0; g[i]; i++) {

        Aux* aux;
        for (aux = g[i]; aux; aux = aux->next) {

            Second* second = new Second();

            second->params[0] = aux->params[0];
            second->params[1] = aux->params[1];
            second->params[2] = aux->params[2];
            second->params[3] = aux->params[3];

            second->transf = new char[15];
            strcpy(second->transf, aux->transf);

            transfs.push_back(second);

        }
    }

    strcpy(first->file, file);
    first->transfs = transfs;

    files.push_back(first);

}

void addAux(Group g, int i, char transf[], double x, double y, double z, double angle)
{
    Aux* old = g[i];

    Aux* aux = (Aux*)malloc(sizeof(struct aux));

    aux->transf = (char*)malloc(sizeof(char) * 15);
    strcpy(aux->transf, transf);
    aux->params[0] = x;
    aux->params[1] = y;
    aux->params[2] = z;
    aux->params[3] = angle;
    aux->next = NULL;

    if (g[i] == NULL)
        g[i] = aux;

    else {

        while (old->next != NULL)
            old = old->next;

        old->next = aux;

    }
}

void groupParser(const XMLElement* scene, Group g, int i, vector<First*>& files)
{

    double x;
    double y;
    double z;
    double angle;

    for (const XMLElement* group = scene->FirstChildElement(); group; group = group->NextSiblingElement()) {

        if (strcmp((char*)group->Value(), "translate") == 0) {

            char transf[15];
            strcpy(transf, (char*)group->Value());

            x = 0;
            y = 0;
            z = 0;

            char* translateX = (char*)group->Attribute("X");
            if (translateX != NULL)
                x = atof(translateX);

            char* translateY = (char*)group->Attribute("Y");
            if (translateY != NULL)
                y = atof(translateY);

            char* translateZ = (char*)group->Attribute("Z");
            if (translateZ != NULL)
                z = atof(translateZ);

            addAux(g, i, transf, x, y, z, 0);

        }

        if (strcmp((char*)group->Value(), "rotate") == 0) {

            char transf[10];
            strcpy(transf, (char*)group->Value());

            x = 0;
            y = 0;
            z = 0;
            angle = 0;

            char* rotateX = (char*)group->Attribute("X");
            if (rotateX != NULL)
                x = atof(rotateX);

            char* rotateY = (char*)group->Attribute("Y");
            if (rotateY != NULL)
                y = atof(rotateY);

            char* rotateZ = (char*)group->Attribute("Z");
            if (rotateZ != NULL)
                z = atof(rotateZ);

            char* rotateAngle = (char*)group->Attribute("angle");
            if (rotateAngle != NULL)
                angle = atof(rotateAngle);

            addAux(g, i, transf, x, y, z, angle);

        }

        if (strcmp((char*)group->Value(), "scale") == 0) {

            char transf[10];
            strcpy(transf, (char*)group->Value());

            x = 0;
            y = 0;
            z = 0;

            char* scaleX = (char*)group->Attribute("X");
            if (scaleX != NULL)
                x = atof(scaleX);

            char* scaleY = (char*)group->Attribute("Y");
            if (scaleY != NULL)
                y = atof(scaleY);

            char* scaleZ = (char*)group->Attribute("Z");
            if (scaleZ != NULL)
                z = atof(scaleZ);

            addAux(g, i, transf, x, y, z, 0);

        }

        if (strcmp((char*)group->Value(), "color") == 0) {

            char transf[10];
            strcpy(transf, (char*)group->Value());

            x = 0;
            y = 0;
            z = 0;

            char* colorR = (char*)group->Attribute("R");
            if (colorR != NULL)
                x = atof(colorR);

            char* colorG = (char*)group->Attribute("G");
            if (colorG != NULL)
                y = atof(colorG);

            char* colorB = (char*)group->Attribute("B");
            if (colorB != NULL)
                z = atof(colorB);

            addAux(g, i, transf, x, y, z, 0);

        }

        if (strcmp((char*)group->Value(), "models") == 0) {

            for (const XMLElement* models = group->FirstChildElement(); models; models = models->NextSiblingElement()) {

                char dir[4] = "../";
                char file[15];

                char* fileName = (char*)models->Attribute("file");
                strcpy(file, fileName);

                char file3d[40];
                file3d[0] = '\0';
                strcat(file3d, dir);
                strcat(file3d, file);

                addFile(file3d, g, files);

            }
        }

        if (strcmp((char*)group->Value(), "group") == 0) {

            groupParser(group, g, i + 1, files);

            for (int j = i+1; j < 20; j++)
                g[j] = NULL;

        }
    }
}

void parser(const char* config, vector<First*>& files)
{
    int i = 0;
    Group g;
    init(g);

    XMLDocument doc;
    XMLError err = doc.LoadFile(config);

    if (!err) {

        XMLElement* elem = doc.FirstChildElement();
        for (const XMLElement* scene = elem->FirstChildElement(); scene; scene = scene->NextSiblingElement()) {

            if (strcmp(scene->Value(), "group") == 0) {

                for (int j = i; j < 20; j++)
                    g[j] = NULL;

                groupParser(scene, g, i, files);

            }
        }
    }
}