Beat Tracking Evaluation Toolbox
================================

This repository implements a number of beat tracking evaluation methods, described in a summary paper:

* Evaluation Methods for Musical Audio Beat Tracking Algorithms, Matthew E. P. Davies, Norberto Degara and Mark D. Plumbley, Technical Report C4DM-TR-09-06, 2009

The toolbox can be used in one of three ways 
* Python implementation
* C++ implementation
* Python module that wraps the C++ implementation

Python software implementation by Matthew Davies and Adam Stark.
C++ implementation and Python module by Adam Stark

Running the code (Python)
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

Running the code (Python module)
----------------

Go to the `python-module` directory and build the module:

```
python setup.py build
sudo python setup.py install
```
Then in your Python script, import the module:

```
import beat_tracking_evaluation_toolbox as be
```

Next, ensure your beats and annotations are in `numpy` arrays and get evaluation scores by:

```
results = be.evaluate (beats, annotations)
```

You might want to loop over a number of files, so if you had the beats and annotations each in a list of numpy arrays (`allBeats` and `allAnnotations` in the below example), you might choose to do the following:

```
for i in range(len(allBeats)):
    print("Evaluating file " + str(i + 1))
    if (results.size == 0):
        results = be.evaluate (allBeats[i], allAnnotations[i])
    else:
        results = np.vstack( (results, be.evaluate (allBeats[i], allAnnotations[i])) )

meanResults = results.mean(0)

print (" ")
print ("--------------- Results ------------------")                
print ("fMeasure:  %.2f" % meanResults[2])
print ("cemgilAcc:  %.2f" % meanResults[0])
print ("gotoAcc:  %.2f" % meanResults[4])
print ("pScore:  %.2f" % meanResults[3])

# print out continuity based measures
r_cmlC = meanResults[6]
r_cmlT = meanResults[7]
r_amlC = meanResults[8]
r_amlT = meanResults[9]      
print ("cmlC:  %.2f   cmlT:  %.2f   amlC:  %.2f   amlT:  %.2f" % (r_cmlC, r_cmlT, r_amlC, r_amlT))

print ("infoGain:  %.2f" % meanResults[5])                    
print ("amlCem:  %.2f" % meanResults[1])
print ("------------------------------------------")
print (" ")
```



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