//
//  BeatTrackingEvaluationToolbox.cpp
//  BeatTrackingEvaluationToolbox
//
//  Created by Adam Stark on 14/02/2016.
//  Copyright © 2016 Adam Stark. All rights reserved.
//

#include "BeatTrackingEvaluationToolbox.h"

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::evaluate (std::vector<double> beats, std::vector<double> annotations)
{
    std::vector<double> results;
    
    double cemgilAccuracy = evaluateBeatsCemgilAccuracy (beats, annotations);
    double amlCemgilAccuracy = evaluateBeatsAmlCemgilAccuracy (beats, annotations);
    double fMeasure = evaluateBeatsFMeasure (beats, annotations);
    ContinuityResult continuityResult = evaluateBeatsContinuity (beats, annotations);
    
    results.push_back (cemgilAccuracy);
    results.push_back (amlCemgilAccuracy);
    results.push_back (fMeasure);
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
    if (maxElement (beats) > 10000.  || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }
    
    // calculate double tempo annotations
    std::vector<double> doubleTempoAnnotations = createSetOfAnnotationsAtDoubleTempo (annotations);
    
    // get off beats
    std::vector<double> offBeatAnnotations = getEveryOtherAnnotationStartingAtIndex (doubleTempoAnnotations, 1);
    
    // get half tempo odd beats (i.e. 1,3,1,3)
    std::vector<double> halfTempOddBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 0);
    
    // get half tempo even beats (i.e. 2,4,2,4)
    std::vector<double> halfTempoEvenBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 1);
    
    std::vector<double> totalScores;
    std::vector<double> continuityScores;

    ContinuityEvaluationScores normalAnnotationScores = continutityEvaluation(beats, annotations);
    totalScores.push_back (normalAnnotationScores.totalAccuracy);
    continuityScores.push_back (normalAnnotationScores.continuityAccuracy);
    
    ContinuityEvaluationScores doubleTempoAnnotationScores = continutityEvaluation(beats, doubleTempoAnnotations);
    totalScores.push_back (doubleTempoAnnotationScores.totalAccuracy);
    continuityScores.push_back (doubleTempoAnnotationScores.continuityAccuracy);
    
    ContinuityEvaluationScores offBeatAnnotationScores = continutityEvaluation(beats, offBeatAnnotations);
    totalScores.push_back (offBeatAnnotationScores.totalAccuracy);
    continuityScores.push_back (offBeatAnnotationScores.continuityAccuracy);
    
    ContinuityEvaluationScores halfTempOddBeatsAnnotationScores = continutityEvaluation(beats, halfTempOddBeatsAnnotations);
    totalScores.push_back (halfTempOddBeatsAnnotationScores.totalAccuracy);
    continuityScores.push_back (halfTempOddBeatsAnnotationScores.continuityAccuracy);
    
    ContinuityEvaluationScores halfTempoEvenBeatsAnnotationScores = continutityEvaluation(beats, halfTempoEvenBeatsAnnotations);
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
    if (maxElement(beats) > 10000.  || maxElement(annotations) > 10000.)
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
    }
    
    // add any remaining beats to the number of false positives
    numFalsePositives += std::accumulate (beatUnused.begin(),beatUnused.end(), 0);

    // calculate precision
    double precision = 0;
    
    if ((numCorrectDetections + numFalsePositives) > 0)
    {
        precision = 100. * (numCorrectDetections/(numCorrectDetections+numFalsePositives));
    }
    
    // calculate recall
    double recall = 0;
    if ((numCorrectDetections + numFalseNegatives) > 0)
    {
        recall = 100. * (numCorrectDetections/(numCorrectDetections+numFalseNegatives));
    }
    
    // now calculate the f-measure
    double fMeasure = 0;
    
    if ((precision+recall) > 0)
    {
        fMeasure = (2.0*precision*recall)/(precision+recall);
    }
    
    // this is Dixon's related accuracy measure from (Dixon, 2001)
    double accuracy = 0;
    
    if ((numCorrectDetections + numFalsePositives + numFalseNegatives) > 0)
    {
        accuracy = 100.0*numCorrectDetections/(numCorrectDetections+numFalsePositives+numFalseNegatives);
    }
    
    return fMeasure;
}

//==========================================================================================
double BeatTrackingEvaluationToolbox:: evaluateBeatsCemgilAccuracy (std::vector<double> beats, std::vector<double> annotations, double sigma)
{
    // remove beats and annotations that are within the first 5 seconds
    beats = removeIfLessThanValue (beats, 5);
    annotations = removeIfLessThanValue (annotations, 5);
    
    // if there are no beats, score zero
    if (beats.size() == 0)
        return 0.0;
    
    // if the beats contain very large values
    if (maxElement(beats) > 10000.  || maxElement(annotations) > 10000.)
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
            double timeDifference = fabs(beat - annotation);
            
            if (timeDifference < timeToNearestBeat)
                timeToNearestBeat = timeDifference;
        }
        
        // sum the outputs of the Gaussian error function
        cemgilAccuracy += exp(-(timeToNearestBeat*timeToNearestBeat) / (2.*(sigma*sigma)));
    }
    
    // normalise by the mean of the number of annotations and beats
    cemgilAccuracy = cemgilAccuracy/(0.5*((double)beats.size() + (double)annotations.size()));
    
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
    if (maxElement (beats) > 10000.  || maxElement (annotations) > 10000.)
    {
        // !
        // looks like the beat times are in samples, not seconds
        assert (false);
    }
    
    // calculate double tempo annotations
    std::vector<double> doubleTempoAnnotations = createSetOfAnnotationsAtDoubleTempo (annotations);
    
    // get off beats
    std::vector<double> offBeatAnnotations = getEveryOtherAnnotationStartingAtIndex (doubleTempoAnnotations, 1);
    
    // get half tempo odd beats (i.e. 1,3,1,3)
    std::vector<double> halfTempOddBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 0);
    
    // get half tempo even beats (i.e. 2,4,2,4)
    std::vector<double> halfTempoEvenBeatsAnnotations = getEveryOtherAnnotationStartingAtIndex (annotations, 1);
    
    std::vector<double> scores;
    
    scores.push_back ( evaluateBeatsCemgilAccuracy (beats,  annotations,                    sigma) );
    scores.push_back ( evaluateBeatsCemgilAccuracy (beats,  doubleTempoAnnotations,         sigma) );
    scores.push_back ( evaluateBeatsCemgilAccuracy (beats,  offBeatAnnotations,             sigma) );
    scores.push_back ( evaluateBeatsCemgilAccuracy (beats,  halfTempOddBeatsAnnotations,    sigma) );
    scores.push_back ( evaluateBeatsCemgilAccuracy (beats,  halfTempoEvenBeatsAnnotations,  sigma) );
    
    double amlCemgilAccuracy = maxElement (scores);
    
    return amlCemgilAccuracy;
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
            double timeDifference = fabs(beats[i] - annotations[j]);
            
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
            if ( i == 0 ||  indexOfNearestAnnotation == 0)
            {
                proportionOfTimeToAnnotationGivenInterAnnotationInterval = fabs(timeToNearestAnnotation / (annotations[indexOfNearestAnnotation+1] - annotations[indexOfNearestAnnotation]));
                
                proportionalDifferenceOfInterBeatIntervals = fabs(1.-((beats[i+1]-beats[i])/(annotations[indexOfNearestAnnotation+1]-annotations[indexOfNearestAnnotation])));
            }
            else // not first beat so we can look backwards
            {
                proportionOfTimeToAnnotationGivenInterAnnotationInterval = fabs(timeToNearestAnnotation / (annotations[indexOfNearestAnnotation] - annotations[indexOfNearestAnnotation-1]));
                
                proportionalDifferenceOfInterBeatIntervals = fabs(1.-((beats[i]-beats[i-1])/(annotations[indexOfNearestAnnotation]-annotations[indexOfNearestAnnotation-1])));
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
    
    
    double numCorrectBeats = std::accumulate(beatsCorrect.begin(), beatsCorrect.end(), 0);
    
    double longestTrackedSegment = (double) maxElement(lengthsOfContinuouslyCorrectlyTrackedBeats);
    
    ContinuityEvaluationScores scores;
    scores.totalAccuracy = 100. * (numCorrectBeats / (double) beatsCorrect.size());
    scores.continuityAccuracy = 100. * (longestTrackedSegment / (double) beatsCorrect.size());

    return scores;
}

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::createSetOfAnnotationsAtDoubleTempo (std::vector<double> annotations)
{
    std::vector<double> doubleTempoAnnotations;
    
    for (int i = 0; i < annotations.size()-1; i++)
    {
        doubleTempoAnnotations.push_back (annotations[i]);
        
        double offBeatAnnotation = (annotations[i] + annotations[i+1]) / 2;
        doubleTempoAnnotations.push_back (offBeatAnnotation);
    }
    
    doubleTempoAnnotations.push_back (annotations[annotations.size()-1]);
    
    return doubleTempoAnnotations;
}

//==========================================================================================
std::vector<double> BeatTrackingEvaluationToolbox::getEveryOtherAnnotationStartingAtIndex (std::vector<double> annotations, int startIndex)
{
    std::vector<double> newAnnotations;
    
    for (int i = startIndex; i < annotations.size();i += 2)
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
double BeatTrackingEvaluationToolbox::meanOfVector (std::vector<double> vector)
{
    if (vector.size() > 0)
    {
        return std::accumulate(vector.begin(), vector.end(), 0.0) / (double) vector.size();
    }
    
    return 0.;
}

//==========================================================================================
template<typename T>
double BeatTrackingEvaluationToolbox::maxElement (std::vector<T> array)
{
    return *std::max_element(array.begin(), array.end());
}