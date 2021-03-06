/*
Rename the vectors so they are easier to read and dont take as much space
for multidimensional vectors.

Author: Oliver Jack Wenman
*/
#ifndef MyVectors_h
#define MyVectors_h 1

#include <vector>
#include <string>
#include "G4PVPlacement.hh"
using namespace std;

typedef vector<vector<vector<vector<int> > > > int_vector4D;
typedef vector<vector<vector<int> > >          int_vector3D;
typedef vector<vector<int> >                   int_vector2D;
typedef vector<int>                            int_vector1D;

typedef vector<vector<vector<vector<double> > > > double_vector4D;
typedef vector<vector<vector<double> > >          double_vector3D;
typedef vector<vector<double> >                   double_vector2D;
typedef vector<double>                            double_vector1D;

typedef vector<vector<vector<vector<string> > > > string_vector4D;
typedef vector<vector<vector<string> > >          string_vector3D;
typedef vector<vector<string> >                   string_vector2D;
typedef vector<string>                            string_vector1D;
#endif
