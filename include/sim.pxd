from libcpp.vector cimport vector
import numpy as np

#C++ interface to cython
cdef extern from "Simulation.hh":
   cdef cppclass Simulation:
      Simulation() except +
      
      void pyInitialise(int, int)
      vector[int] pyRun(unsigned long long int, int, int, double)

      vector[int] GetLastImage()
      