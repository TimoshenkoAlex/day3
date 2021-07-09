#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

struct DataList {
    double Uy, Sy, Ux, Sx, t, a;
    DataList* next;
};

void startList(DataList* root) {
    (*root).Uy = 0;
    (*root).Sy = 0;
    (*root).Ux = 0;
    (*root).Sx = 0;
    (*root).t = 0;
    (*root).a = 0;
    (*root).next = NULL;
}

void writeList(DataList* ls, double Ux, double Sx, double Uy, double Sy, double t, double a) {
    DataList* lsEnd = ls;
    while ((*lsEnd).next != NULL)
        lsEnd = (*lsEnd).next;

    (*lsEnd).Uy = Uy;
    (*lsEnd).Sy = Sy;
    (*lsEnd).Ux = Ux;
    (*lsEnd).Sx = Sx;
    (*lsEnd).t = t;
    (*lsEnd).a = a;

    (*lsEnd).next = new DataList;
    startList((*lsEnd).next);
}

const double g = 9.80665;

/*
double deltaT = 0.01;
double Uy = 20;
double Sy = 0;
double Ux = 20;
double Sx = 0;
double Sx_max = 0;
double Sx_min = 1000;
*/
double percent = 0.75;
int rebound = 1;

DataList* simulate(double Ux, double Sx, double Uy, double Sy, double deltaT) {
    DataList* output = new DataList;
    startList(output);

    double t = 0;
    for (int i = 0; i <= rebound; i++) {
        while (Sy >= 0) {
            Sx += deltaT * Ux;
            Sy += Uy * deltaT - (g * deltaT * deltaT) / 2;
            Uy = Uy - (g * deltaT);
            t += deltaT;

            writeList(output, Ux, Sx, Uy, Sy, t, g);

            //cout << Sx << " " << Ux << endl;
            //cout << Sx << endl;
        }
        Uy = Uy * (-1) * percent;
        Sy = Sy * (-1);
    }

    return output;
}

void print(DataList* ls)
{
    cout << "Printing data" << endl;
    cout << "Time\tSx\tUx\tSy\tUy\t\tG" << endl;

    DataList* outLs = ls;
    while ((*outLs).next != NULL) {
        cout << fixed << showpoint;
        cout << setprecision(2);
        cout << (double)(*outLs).t << "\t" << (*outLs).Sx << "\t" << (*outLs).Ux << "\t" << (*outLs).Sy << "\t" << (*outLs).Uy << "\t\t" << (*outLs).a << endl;
        outLs = (*outLs).next;
    }
}

bool testSimulate()
{
    double maxSy = 0;
    double minSy = 100;
    double deltaT = 0.01;
    double Uy = 20;
    double Sy = 0;
    double Ux = 3;
    double Sx = 0;
    DataList* outLs = simulate(Ux, Sx, Uy, Sy, deltaT);
    while ((*outLs).next != NULL) {
        if ((*outLs).Sy > maxSy) {
            maxSy = (*outLs).Sy;
        }
        if ((*outLs).Sy < minSy) {
            minSy = (*outLs).Sy;
        }
        if ((*outLs).Ux != Ux) { // ãîðèçîíòàëüíàÿ ñîñòîâëÿþùàÿ ïîñòîÿííà
            return false;
        }
        outLs = (*outLs).next;
    }
    //cout << maxSy << "\t" << Uy*Uy/2/g - 0.02  << "\t" << Uy*Uy/2/g + 0.02<< endl;
    //cout << minSy << endl;
    if ((maxSy < (Uy * Uy / 2 / g - 0.2)) || (maxSy > (Uy * Uy / 2 / g + 0.2))) { // äîëåòåë äî òåîðåòè÷åñêîãî ìàêñèìóìà
        return false;
    }
    if ((minSy > 0.2) || (minSy < -0.2)) { // óäàð îá çåìëþ
        return false;
    }
    if ((*outLs).Sx != (*outLs).t * Ux) { // ïðîâåðêà íà äàëüíîñòü ïîë¸òà
        return false;
    }

    return true;
}

void runTest(bool (*testFunction)(), const string& testName)
{
    if (testFunction() == true)
        std::cout << "Test " << testName << " - OK" << std::endl;
    else
        std::cout << "Test " << testName << " - FAIL" << std::endl;
}


int main() {
    runTest(testSimulate, "Simulation");
    //DataList* dl = simulate(3, 0, 20, 0, 0.01);
    //print(dl);

    return 0;
}