//=======================================================================
/**
 * Beat Tracking Evaluation Toolbox (C++)
 *
 * @copyright (c) 2016 Adam Stark
 *
 * License: The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
//=======================================================================

#include "BeatTrackingEvaluationToolbox.h"

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::evaluate (std::vector<double> beats, std::vector<double> annotations)
{
    std::vector<double> results;

    double cemgilAccuracy = evaluateBeatsCemgilAccuracy (beats, annotations);
    double amlCemgilAccuracy = evaluateBeatsAmlCemgilAccuracy (beats, annotations);
    double fMeasure = evaluateBeatsFMeasure (beats, annotations);
    double pScore = evaluateBeatsPScore (beats, annotations);
    double gotoAccuracy = evaluateBeatsGoto (beats, annotations);
    double informationGain = evaluateBeatsInformationGain (beats, annotations);
    ContinuityResult continuityResult = evaluateBeatsContinuity (beats, annotations);

    results.push_back (cemgilAccuracy);
    results.push_back (amlCemgilAccuracy);
    results.push_back (fMeasure);
    results.push_back (pScore);
    results.push_back (gotoAccuracy);
    results.push_back (informationGain);
    results.push_back (continuityResult.cmlC);
    results.push_back (continuityResult.cmlT);
    results.push_back (continuityResult.amlC);
    results.push_back (continuityResult.amlT);

    return results;
}

//==========================================================================================
ContinuityResult BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (std::vector<double> beats, std::vector<double> annotations)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);

    // if there are no beats, score zero
    if (beats.size() == 0)
    {
        ContinuityResult result;

        return result;
    }
    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }

    // calculate double tempo annotations
    std::vector<double> doubleTempoAnnotations = createSetOfAnnotationsAtdoubleTempo (annotations);

    // get off beats
    std::vector<double> offBeatAnnotations = getEveryOtherAnnotationStartingAtIndex (doubleTempoAnnotations, 1);

    // get half tempo odd beats (i.e. 1,3,1,3)
    std::vector<double> halfTempOddBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 0);

    // get half tempo even beats (i.e. 2,4,2,4)
    std::vector<double> halfTempoEvenBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 1);

    std::vector<double> totalScores;
    std::vector<double> continuityScores;

    ContinuityEvaluationScores normalAnnotationScores = continutityEvaluation (beats, annotations);
    totalScores.push_back (normalAnnotationScores.totalAccuracy);
    continuityScores.push_back (normalAnnotationScores.continuityAccuracy);

    ContinuityEvaluationScores doubleTempoAnnotationScores = continutityEvaluation (beats, doubleTempoAnnotations);
    totalScores.push_back (doubleTempoAnnotationScores.totalAccuracy);
    continuityScores.push_back (doubleTempoAnnotationScores.continuityAccuracy);

    ContinuityEvaluationScores offBeatAnnotationScores = continutityEvaluation (beats, offBeatAnnotations);
    totalScores.push_back (offBeatAnnotationScores.totalAccuracy);
    continuityScores.push_back (offBeatAnnotationScores.continuityAccuracy);

    ContinuityEvaluationScores halfTempOddBeatsAnnotationScores = continutityEvaluation (beats, halfTempOddBeatsAnnotations);
    totalScores.push_back (halfTempOddBeatsAnnotationScores.totalAccuracy);
    continuityScores.push_back (halfTempOddBeatsAnnotationScores.continuityAccuracy);

    ContinuityEvaluationScores halfTempoEvenBeatsAnnotationScores = continutityEvaluation (beats, halfTempoEvenBeatsAnnotations);
    totalScores.push_back (halfTempoEvenBeatsAnnotationScores.totalAccuracy);
    continuityScores.push_back (halfTempoEvenBeatsAnnotationScores.continuityAccuracy);

    ContinuityResult result;

    result.cmlC = normalAnnotationScores.continuityAccuracy;
    result.cmlT = normalAnnotationScores.totalAccuracy;
    result.amlC = maxElement (continuityScores);
    result.amlT = maxElement (totalScores);

    return result;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (std::vector<double> beats, std::vector<double> annotations, double toleranceWindowInSeconds)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);

    // if there are no beats, score zero
    if (beats.size() == 0)
        return 0.0;

    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }

    double numFalsePositives = 0;
    double numFalseNegatives = 0;
    double numCorrectDetections = 0;

    std::vector<double> beatUnused (beats.size(), 1);

    for (auto& annotation : annotations)
    {
        double windowMin = annotation - toleranceWindowInSeconds;
        double windowMax = annotation + toleranceWindowInSeconds;

        std::vector<double> beatsInToleranceWindow;

        for (int i = 0; i < beats.size(); i++)
        {
            if (beats[i] >= windowMin && beats[i] <= windowMax && beatUnused[i] == 1)
            {
                beatsInToleranceWindow.push_back (i);
                beatUnused[i] = 0;
            }
        }

        // no beats in window, therefore it's a false negative
        if (beatsInToleranceWindow.size() == 0)
        {
            numFalseNegatives += 1;
        }
        // false positive case, more than one beat in a tolerance window
        else if (beatsInToleranceWindow.size() > 1)
        {
            numCorrectDetections += 1;
            numFalsePositives += 1;
        }
        // only one beat in the tolerance window therefore a correct detection
        else
        {
            numCorrectDetections += 1;
        }
    }

    // add any remaining beats to the number of false positives
    numFalsePositives += std::accumulate (beatUnused.begin(), beatUnused.end(), 0);

    // calculate precision
    double precision = 0;

    if ((numCorrectDetections + numFalsePositives) > 0)
    {
        precision = 100. * (numCorrectDetections / (numCorrectDetections + numFalsePositives));
    }

    // calculate recall
    double recall = 0;
    if ((numCorrectDetections + numFalseNegatives) > 0)
    {
        recall = 100. * (numCorrectDetections / (numCorrectDetections + numFalseNegatives));
    }

    // now calculate the f-measure
    double fMeasure = 0;

    if ((precision + recall) > 0)
    {
        fMeasure = (2.0 * precision * recall) / (precision + recall);
    }

    // this is Dixon's related accuracy measure from (Dixon, 2001)
    double accuracy = 0;

    if ((numCorrectDetections + numFalsePositives + numFalseNegatives) > 0)
    {
        accuracy = 100.0 * numCorrectDetections / (numCorrectDetections + numFalsePositives + numFalseNegatives);
    }

    return fMeasure;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::evaluateBeatsPScore (std::vector<double> beats, std::vector<double> annotations, double threshold)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);

    // if there are no beats, score zero
    if (beats.size() == 0)
        return 0.0;

    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }

    // 10 ms resolution
    double pulseTrainSamplingFrequency = 100;

    int endPoint = (int)ceil (std::max (maxElement (annotations), maxElement (beats)));

    // make impulse trains from beat times, take those beats only greater than 5 seconds
    std::vector<double> annotationPulseTrain (endPoint * (int)pulseTrainSamplingFrequency, 0.);
    std::vector<double> beatPulseTrain (endPoint * (int)pulseTrainSamplingFrequency, 0.);

    for (int i = 0; i < annotations.size(); i++)
    {
        int index = ((int)ceil (annotations[i] * pulseTrainSamplingFrequency)) - 1;
        annotationPulseTrain[index] = 1.;
    }

    for (int i = 0; i < beats.size(); i++)
    {
        int index = ((int)ceil (beats[i] * pulseTrainSamplingFrequency)) - 1;
        beatPulseTrain[index] = 1.;
    }

    std::vector<int> indicesOfNonZeroElements = getIndicesOfNonZeroElements (annotationPulseTrain);

    std::vector<int> differenceBetweenIndices;

    for (int i = 1; i < indicesOfNonZeroElements.size(); i++)
    {
        differenceBetweenIndices.push_back (indicesOfNonZeroElements[i] - indicesOfNonZeroElements[i - 1]);
    }

    int maximumLag = (int)round (threshold * medianOfVector (differenceBetweenIndices));

    int numNonZeroBeats = (int)getIndicesOfNonZeroElements (beatPulseTrain).size();

    int numNonZeroAnnotations = (int)getIndicesOfNonZeroElements (annotationPulseTrain).size();

    double pScore = sumOfCrossCorrelation (beatPulseTrain, annotationPulseTrain, maximumLag) / std::max (numNonZeroBeats, numNonZeroAnnotations);

    pScore *= 100.;

    return pScore;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::evaluateBeatsGoto (std::vector<double> beats, std::vector<double> annotations, double threshold, double mu, double sigma)

{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);

    std::vector<int> paired (annotations.size());
    std::vector<int> beatError (annotations.size());
    
    std::fill (paired.begin(), paired.end(), 0);
    std::fill (beatError.begin(), beatError.end(), 1);
    
    // if there are no beats, score zero
    if (beats.size() == 0)
        return 0.0;

    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }
    
    for (int k = 1; k < annotations.size() - 1; k++)
    {
        double previousInterAnnotationInterval = 0.5 * (annotations[k] - annotations[k - 1]);
        double windowMin = annotations[k] - previousInterAnnotationInterval;

        double nextInterAnnotationInterval = 0.5 * (annotations[k + 1] - annotations[k]);
        double windowMax = annotations[k] + nextInterAnnotationInterval;

        std::vector<int> beatsInWindow;

        for (int i = 0; i < beats.size(); i++)
        {
            if (beats[i] >= windowMin && beats[i] < windowMax)
            {
                beatsInWindow.push_back (i);
            }
        }

        if (beatsInWindow.size() == 0) // false negative case
        {
            paired[k] = 0;
            beatError[k] = 1;
        }
        else if (beatsInWindow.size() > 1) // false positive case
        {
            paired[k] = 0;
            beatError[k] = 1;
        }
        else // paired beat, so measure beat error
        {
            paired[k] = 1;
            
            double error = (double) beats[beatsInWindow[0]] - annotations[k];
            
            // if negative use pre_interval
            if (error < 0)
            {
                beatError[k] = error / previousInterAnnotationInterval;
            }
            // else positive so use subsequent inter annotation interval
            else
            {
               beatError[k] = error / nextInterAnnotationInterval;
            }

        }
    }
    
    std::vector<int> sequenceOfCorrectBeats;
    
    // find the index of the beats that are lower than the threshold
    for (int i = 0; i < beatError.size(); i++)
    {
        if (beatError[i] < threshold)
            sequenceOfCorrectBeats.push_back (i);
    }
    
    // if there are less than 2 correct beats, we might
    // run into trouble with indices when doing the calculations
    // but this is clearly bad beat tracking so return zero
    if (sequenceOfCorrectBeats.size() < 2)
    {
        return 0.;
    }
    
    // --------------------------------
    // Condition 1: we must have accurate tracking over a segment of 25% of the piece
    // (note, this is slightly different from the Goto paper and was suggested by
    // Matthew Davies in his technical paper on beat tracking evaluation)
    
    int longestCorrectSequence = 0;
    int longestSequenceStart = 0;
    int longestSequenceEnd = 0;
    int currentSequenceCount = 0;
    int currentSequenceStart = 0;
    
    for (int i = 1; i < sequenceOfCorrectBeats.size(); i++)
    {
        if (sequenceOfCorrectBeats[i] - 1 == sequenceOfCorrectBeats[i - 1])
        {
            currentSequenceCount++;
            
            if (currentSequenceCount > longestCorrectSequence)
            {
                longestSequenceStart = currentSequenceStart;
                longestSequenceEnd = sequenceOfCorrectBeats[i];
                longestCorrectSequence = currentSequenceCount;
            }
        }
        else
        {
            currentSequenceStart = sequenceOfCorrectBeats[i];
            currentSequenceCount = 0;
        }
    }
    
    double longestSequenceLengthInSeconds = annotations[longestSequenceEnd] - annotations[longestSequenceStart];
    
    bool condition1 = longestSequenceLengthInSeconds  / annotations[annotations.size() - 1] > 0.25;
    
    // --------------------------------
    // Condition 2: Mean of beat error of correct beats must be below mu
    
    std::vector<double> beatErrorForCorrectBeats;
    
    double sumVal = 0.;
    for (int  i = 0; i < sequenceOfCorrectBeats.size(); i++)
    {
        sumVal += beatError[sequenceOfCorrectBeats[i]];
    }
    
    double mean = sumVal / (double)sequenceOfCorrectBeats.size();
    
    bool condition2 = mean < mu;
    
    // --------------------------------
    // Condition 3: Standard deviation of beat error of correct beats must be below sigma
    
    double diffSum = 0.0;
    
    for (int i = 0; i < sequenceOfCorrectBeats.size(); i++)
    {
        double diff = beatError[sequenceOfCorrectBeats[i]] - mean;
        diffSum += (diff * diff);
    }
    
    double variance = diffSum / (double) (sequenceOfCorrectBeats.size() - 1);
    double std = sqrt (variance);
    
    bool condition3 = std < sigma;
    
    if (condition1 && condition2 && condition3)
    {
        return 100.00;
    }
    else
    {
        return 0.0;
    }
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (std::vector<double> beats, std::vector<double> annotations, double sigma)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);

    // if there are no beats, score zero
    if (beats.size() == 0)
        return 0.0;

    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }

    double cemgilAccuracy = 0.f;

    for (auto& annotation : annotations)
    {
        // calculate the time to the nearest beat
        double timeToNearestBeat = std::numeric_limits<double>::max();

        for (auto& beat : beats)
        {
            double timeDifference = fabs (beat - annotation);

            if (timeDifference < timeToNearestBeat)
                timeToNearestBeat = timeDifference;
        }

        // sum the outputs of the Gaussian error function
        cemgilAccuracy += exp (-(timeToNearestBeat * timeToNearestBeat) / (2. * (sigma * sigma)));
    }

    // normalise by the mean of the number of annotations and beats
    cemgilAccuracy = cemgilAccuracy / (0.5 * ((double)beats.size() + (double)annotations.size()));

    // put into the range 0 to 100%
    cemgilAccuracy *= 100.f;

    return cemgilAccuracy;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::evaluateBeatsAmlCemgilAccuracy (std::vector<double> beats, std::vector<double> annotations, double sigma)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);

    // if there are no beats, score zero
    if (beats.size() == 0)
        return 0.;

    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }

    // calculate double tempo annotations
    std::vector<double> doubleTempoAnnotations = createSetOfAnnotationsAtdoubleTempo (annotations);

    // get off beats
    std::vector<double> offBeatAnnotations = getEveryOtherAnnotationStartingAtIndex (doubleTempoAnnotations, 1);

    // get half tempo odd beats (i.e. 1,3,1,3)
    std::vector<double> halfTempOddBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 0);

    // get half tempo even beats (i.e. 2,4,2,4)
    std::vector<double> halfTempoEvenBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 1);

    std::vector<double> scores;

    scores.push_back (evaluateBeatsCemgilAccuracy (beats, annotations, sigma));
    scores.push_back (evaluateBeatsCemgilAccuracy (beats, doubleTempoAnnotations, sigma));
    scores.push_back (evaluateBeatsCemgilAccuracy (beats, offBeatAnnotations, sigma));
    scores.push_back (evaluateBeatsCemgilAccuracy (beats, halfTempOddBeatsAnnotations, sigma));
    scores.push_back (evaluateBeatsCemgilAccuracy (beats, halfTempoEvenBeatsAnnotations, sigma));

    double amlCemgilAccuracy = maxElement (scores);

    return amlCemgilAccuracy;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::sumOfCrossCorrelation (std::vector<double> v1, std::vector<double> v2, int maximumLag)
{
    int originalVectorSize = (int)v1.size();

    std::vector<double> zeroPadding1 (maximumLag, 0.);
    std::vector<double> zeroPadding2 (maximumLag, 0.);

    v1.insert (v1.begin(), zeroPadding1.begin(), zeroPadding1.end());
    v2.insert (v2.begin(), zeroPadding2.begin(), zeroPadding2.end());

    for (int i = 0; i < maximumLag; i++)
    {
        v1.push_back (0);
        v2.push_back (0);
    }

    std::vector<double> crossCorrelation (2 * maximumLag + 1, 0.);

    int i = 0;

    for (int lag = -maximumLag; lag < (maximumLag + 1); lag++)
    {
        double sumVal = 0.;

        for (int n = maximumLag; n < maximumLag + originalVectorSize; n++)
        {
            sumVal += (v1[n] * v2[n + lag]);
        }

        crossCorrelation[i] = sumVal;
        i++;
    }

    return std::accumulate (crossCorrelation.begin(), crossCorrelation.end(), 0.0);
}

//==========================================================================================
BeatTrackingEvaluationToolbox::ContinuityEvaluationScores BeatTrackingEvaluationToolbox::continutityEvaluation (std::vector<double> beats, std::vector<double> annotations, double phaseThreshold, double periodThreshold)
{
    std::vector<bool> annotationUsed (annotations.size(), false);
    std::vector<bool> beatsCorrect (beats.size(), false);

    for (int i = 0; i < beats.size(); i++)
    {
        bool beatCorrect = false;

        // calculate the time to the nearest annotation
        double timeToNearestAnnotation = std::numeric_limits<double>::max();
        int indexOfNearestAnnotation = 0;

        for (int j = 0; j < annotations.size(); j++)
        {
            double timeDifference = fabs (beats[i] - annotations[j]);

            if (timeDifference < timeToNearestAnnotation)
            {
                timeToNearestAnnotation = timeDifference;
                indexOfNearestAnnotation = j;
            }
        }

        // if we've already used this annotation for another beat
        if (annotationUsed[indexOfNearestAnnotation])
        {
            beatCorrect = false;
        }
        else
        {
            double proportionOfTimeToAnnotationGivenInterAnnotationInterval;
            double proportionalDifferenceOfInterBeatIntervals;

            // If either first beat or first annotation, look forward on both
            if (i == 0 || indexOfNearestAnnotation == 0)
            {
                proportionOfTimeToAnnotationGivenInterAnnotationInterval = fabs (timeToNearestAnnotation / (annotations[indexOfNearestAnnotation + 1] - annotations[indexOfNearestAnnotation]));

                proportionalDifferenceOfInterBeatIntervals = fabs (1. - ((beats[i + 1] - beats[i]) / (annotations[indexOfNearestAnnotation + 1] - annotations[indexOfNearestAnnotation])));
            }
            else // not first beat so we can look backwards
            {
                proportionOfTimeToAnnotationGivenInterAnnotationInterval = fabs (timeToNearestAnnotation / (annotations[indexOfNearestAnnotation] - annotations[indexOfNearestAnnotation - 1]));

                proportionalDifferenceOfInterBeatIntervals = fabs (1. - ((beats[i] - beats[i - 1]) / (annotations[indexOfNearestAnnotation] - annotations[indexOfNearestAnnotation - 1])));
            }

            if (proportionOfTimeToAnnotationGivenInterAnnotationInterval < phaseThreshold && proportionalDifferenceOfInterBeatIntervals < periodThreshold)
            {
                annotationUsed[indexOfNearestAnnotation] = true;
                beatCorrect = true;
            }
        }

        beatsCorrect[i] = beatCorrect;
    }

    std::vector<int> lengthsOfContinuouslyCorrectlyTrackedBeats;
    int counter = 0;

    for (bool beatWasCorrect : beatsCorrect)
    {
        if (beatWasCorrect)
        {
            counter++;
        }
        else
        {
            lengthsOfContinuouslyCorrectlyTrackedBeats.push_back (counter);
            counter = 0;
        }
    }

    // in case every beat was correct, append the last value of the count
    lengthsOfContinuouslyCorrectlyTrackedBeats.push_back (counter);

    double numCorrectBeats = std::accumulate (beatsCorrect.begin(), beatsCorrect.end(), 0);

    double longestTrackedSegment = (double)maxElement (lengthsOfContinuouslyCorrectlyTrackedBeats);

    ContinuityEvaluationScores scores;
    scores.totalAccuracy = 100. * (numCorrectBeats / (double)beatsCorrect.size());
    scores.continuityAccuracy = 100. * (longestTrackedSegment / (double)beatsCorrect.size());

    return scores;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::evaluateBeatsInformationGain (std::vector<double> beats, std::vector<double> annotations, int numHistogramBins)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);
    
    // if there are no beats, score zero
    if (beats.size() < 2 || annotations.size() < 2)
        return 0.;
    
    // if the beats contain very large values
    if (maxElement (beats) > 10000. || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
        
        return 0.0;
    }
    
    double stepSize = 1. / ((double)numHistogramBins - 1.);
    double binsStart = -0.5 + 0.5 * stepSize;
    double binsEnd = 0.5 - 0.5 * stepSize;
    
    std::vector<double> histogramBinCentres;
    
    histogramBinCentres.push_back (-0.5);
    
    double binCentre = binsStart;
    
    while (binCentre < binsEnd + stepSize)
    {
        histogramBinCentres.push_back (binCentre);
        binCentre += stepSize;
    }
    
    histogramBinCentres.push_back (0.5);
    
    // beats compared to annotations
    std::vector<double> forwardError = findBeatError (beats, annotations);
    std::vector<double> forwardErrorHistogram = calculateBeatErrorHistogram (forwardError, histogramBinCentres);
    double forwardEntropy = findEntropy (forwardErrorHistogram);
    
    // annotations compared to beats
    std::vector<double> backwardsError = findBeatError (annotations, beats);
    std::vector<double> backwardsErrorHistogram = calculateBeatErrorHistogram (backwardsError, histogramBinCentres);
    double backwardsEntropy = findEntropy (backwardsErrorHistogram);
    
    double maxEntropy;
    
    // find higher entropy value (i.e. which is worst)
    if (forwardEntropy > backwardsEntropy)
    {
        maxEntropy = forwardEntropy;
    }
    else
    {
        maxEntropy = backwardsEntropy;
    }
    
    double informationGain = log2((double)numHistogramBins) - maxEntropy;
    
    return informationGain;
}

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::findBeatError (std::vector<double> beats, std::vector<double> annotations)
{
    std::vector<double> beatError (beats.size());
    std::fill (beatError.begin(), beatError.end(), 0.0);
    
    // hangover from old code, probably not necessary - these should already be sorted by definition!
    std::sort (beats.begin(), beats.end());
    std::sort (annotations.begin(), annotations.end());
    
    // calculate relative error for each beat
    for (int i = 0; i < beats.size(); i++)
    {
        int indexOfClosestAnnotation = getIndexOfNearestElement (annotations, beats[i]);
        double absoluteError = beats[i] - annotations[indexOfClosestAnnotation];
        
        double interAnnotationInterval;
        
        // if first annotation is nearest or nearest annotation is on after the current beat
        // then we look at the next inter-annotation interval
        if (indexOfClosestAnnotation == 0 || absoluteError >= 0)
        {
            interAnnotationInterval = 0.5 * (annotations[indexOfClosestAnnotation + 1] - annotations[indexOfClosestAnnotation]);
        }
        // otherwise we look at the previous inter-annotation interval
        else
        {
            interAnnotationInterval = 0.5 * (annotations[indexOfClosestAnnotation] - annotations[indexOfClosestAnnotation - 1]);
        }
        
        beatError[i] = 0.5 * absoluteError / interAnnotationInterval;
        
        // trick to deal with bin boundaries
        beatError[i] = round (10000.0 * beatError[i]) / 10000.0;
    }
    
    return beatError;
}


//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::calculateBeatErrorHistogram (std::vector<double> beatError, std::vector<double> histogramBins)
{
    // make sure all beat errors are in the range [-0.5, 0.5]
    for (int i = 0; i < beatError.size(); i++)
    {
        while (beatError[i] < -0.5)
            beatError[i] += 1.0;
        
        while (beatError[i] > 0.5)
            beatError[i] -= 1.0;
    }
    
    // calculate the histogram bin edges from the centers
    std::vector<double> histogramBinEdges (histogramBins.size());
    std::fill (histogramBinEdges.begin(), histogramBinEdges.end(), 0.0);
    histogramBinEdges[0] = histogramBins[0] - (fabs (histogramBins[0]) - fabs (histogramBins[1]));
    
    for (int h = 1; h < histogramBins.size(); h++)
        histogramBinEdges[h] = (histogramBins[h-1] + histogramBins[h]) / 2.0;
    
    // add final histogram bin edge
    histogramBinEdges.push_back (histogramBinEdges[histogramBinEdges.size() - 1] + (histogramBinEdges[histogramBinEdges.size() - 1] - histogramBinEdges[histogramBinEdges.size() - 2]));
    
    std::vector<double> histogram (histogramBins.size());
    std::fill (histogram.begin(), histogram.end(), 0.0);
    
    for (int i = 0; i < histogramBinEdges.size() - 1; i++)
    {
        for (int j = 0; j < beatError.size(); j++)
        {
            if (beatError[j] >= histogramBinEdges[i] && beatError[j] < histogramBinEdges[i+1])
            {
                histogram[i] += 1.;
            }
        }
    }
    
    // add the last bin height to the first bin and remove last element
    histogram[0] = histogram[0] + histogram[histogram.size() - 1];
    histogram.pop_back();
    
    // make the bins heights sum to unity
    double sumOfHistorgram = std::accumulate (histogram.begin(), histogram.end(), 0.0);
    for (int i = 0; i < histogram.size(); i++)
    {
        histogram[i] = histogram[i] / sumOfHistorgram;
        
        // set zero valued binvals to 1, this makes the entropy calculation well-behaved.
        if (histogram[i] == 0.0)
            histogram[i] = 1.0;
    }
    
    return histogram;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::findEntropy (std::vector<double> beatErrorHistogram)
{
    double sumValue = 0.0;
    for (int b = 0; b < beatErrorHistogram.size(); b++)
        sumValue += (beatErrorHistogram[b] * log2(beatErrorHistogram[b]));
    
    double entropy = -1.0 * sumValue;
    
    return entropy;
}


//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::createSetOfAnnotationsAtdoubleTempo (std::vector<double> annotations)
{
    std::vector<double> doubleTempoAnnotations;

    for (int i = 0; i < annotations.size() - 1; i++)
    {
        doubleTempoAnnotations.push_back (annotations[i]);

        double offBeatAnnotation = (annotations[i] + annotations[i + 1]) / 2;
        doubleTempoAnnotations.push_back (offBeatAnnotation);
    }

    doubleTempoAnnotations.push_back (annotations[annotations.size() - 1]);

    return doubleTempoAnnotations;
}

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::getEveryOtherAnnotationStartingAtIndex (std::vector<double> annotations, int startIndex)
{
    std::vector<double> newAnnotations;

    for (int i = startIndex; i < annotations.size(); i += 2)
    {
        newAnnotations.push_back (annotations[i]);
    }

    return newAnnotations;
}

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::removeIfLessThanValue (std::vector<double> array, double value)
{
    std::vector<double> result;

    for (auto v : array)
    {
        if (v >= value)
            result.push_back (v);
    }

    return result;
}

//==========================================================================================
std::vector<int> BeatTrackingEvaluationToolbox::getIndicesOfNonZeroElements (std::vector<double> array)
{
    std::vector<int> indices;

    for (int i = 0; i < array.size(); i++)
    {
        if (array[i] != 0)
            indices.push_back (i);
    }

    return indices;
}

//==========================================================================================
template <typename T>
double BeatTrackingEvaluationToolbox::meanOfVector (std::vector<T> vector)
{
    if (vector.size() > 0)
    {
        return std::accumulate (vector.begin(), vector.end(), 0) / (double)vector.size();
    }

    return 0.;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox::medianOfVector (std::vector<int> vector)
{
    double median;

    sort (vector.begin(), vector.end());

    if (vector.size() % 2 == 0)
    {
        median = (vector[vector.size() / 2 - 1] + vector[vector.size() / 2]) / 2;
    }
    else
    {
        median = vector[vector.size() / 2];
    }

    return median;
}

//==========================================================================================
template <typename T>
T BeatTrackingEvaluationToolbox::maxElement (std::vector<T> array)
{
    return *std::max_element (array.begin(), array.end());
}

//==========================================================================================
template <typename T>
int BeatTrackingEvaluationToolbox::argMax (std::vector<T> array)
{
    return std::distance (array.begin(), std::max_element(array.begin(), array.end()));
}

//==========================================================================================
int BeatTrackingEvaluationToolbox::getIndexOfNearestElement (std::vector<double> array, double value)
{
    if (array.size() == 0)
    {
        // seems there is nothing in the array
        assert (false);
    }
    
    int minIndex = 0;
    double minDifference = std::numeric_limits<double>::max();
    
    for (int i = 0; i < array.size(); i++)
    {
        double difference = fabs (value - array[i]);
        
        if (difference < minDifference)
        {
            minDifference = difference;
            minIndex = i;
        }
    }
    
    return minIndex;
}


