# setup.py
# build command : python setup.py build build_ext --inplace
from numpy.distutils.core import setup, Extension
import os, numpy

name = 'beat_tracking_evaluation_toolbox'
sources = ['beat_tracking_evaluation_toolbox_python_module.cpp','../src/BeatTrackingEvaluationToolbox.cpp']

include_dirs = [
                numpy.get_include(),'/usr/local/include'
                ]

setup( name = 'BeatTrackingEvaluationToolbox',
      include_dirs = include_dirs,
      ext_modules = [Extension(name, sources,libraries = [],library_dirs = ['/usr/local/lib'])]
      )