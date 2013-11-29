Beat Tracking Evaluation Toolbox
================================

This is a Python implementation of a number of beat tracking evaluation methods, described in a summary paper:

* Evaluation Methods for Musical Audio Beat Tracking Algorithms, Matthew E. P. Davies, Norberto Degara and Mark D. Plumbley, Technical Report C4DM-TR-09-06, 2009

Software implementation by Matthew Davies and Adam Stark


Running the code
----------------

First, import the library:

    import beat_evaluation_toolbox as be

Then, evaluate beat estimations on database of beat annotaions as follows:

    R = be.evaluate_db(annotations,beats,measures,doCI)

where the arguments are as follows:

* 'annotations' is a list of numpy arrays containing the beat annotations for the database
* 'beats' is a list of numpy arrays containing the beat estimations for the database
* 'measures' is a list of strings indicating which evaluation measures to use. Setting measures='all' calculates all measures. Specific measures can be chosen by setting measures = ['fMeasure','cemgilAcc','gotoAcc','pScore','continuity','infoGain','amlCem'] or a subset thereof.
* 'doCI' is a boolean indicating whether or not to calculate confidence intervals

and the returned value is:

* 'R' is a dictionary containing all results

All results will be printed to the terminal once the evaluation is complete.

License
-------

The MIT License (MIT)

Copyright (c) 2012 Matthew Davies and Adam Stark

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.