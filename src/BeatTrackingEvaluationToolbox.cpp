//
//  BeatTrackingEvaluationToolbox.cpp
//  BeatTrackingEvaluationToolbox
//
//  Created by Adam Stark on 14/02/2016.
//  Copyright © 2016 Adam Stark. All rights reserved.
//

#include "BeatTrackingEvaluationToolbox.h"

//==========================================================================================
float BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (std::vector<float> beats, std::vector<float> annotations)
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
    
    float numFalsePositives = 0;
    float numFalseNegatives = 0;
    float numCorrectDetections = 0;
    float toleranceWindowInSeconds = 0.07;
    
    std::vector<float> beatUnused (beats.size(), 1);
    
    for (auto& annotation : annotations)
    {
        float windowMin = annotation - toleranceWindowInSeconds;
        float windowMax = annotation + toleranceWindowInSeconds;
        
        std::vector<float> beatsInToleranceWindow;
        
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
    float precision = 0;
    
    if ((numCorrectDetections + numFalsePositives) > 0)
    {
        precision = 100. * (numCorrectDetections/(numCorrectDetections+numFalsePositives));
    }
    
    // calculate recall
    float recall = 0;
    if ((numCorrectDetections + numFalseNegatives) > 0)
    {
        recall = 100. * (numCorrectDetections/(numCorrectDetections+numFalseNegatives));
    }
    
    // now calculate the f-measure
    float fMeasure = 0;
    
    if ((precision+recall) > 0)
    {
        fMeasure = (2.0*precision*recall)/(precision+recall);
    }
    
    // this is Dixon's related accuracy measure from (Dixon, 2001)
    float accuracy = 0;
    
    if ((numCorrectDetections + numFalsePositives + numFalseNegatives) > 0)
    {
        accuracy = 100.0*numCorrectDetections/(numCorrectDetections+numFalsePositives+numFalseNegatives);
    }
    
    return fMeasure;
}


//==========================================================================================
std::vector<float> BeatTrackingEvaluationToolbox::removeIfLessThanValue (std::vector<float> array, float value)
{
    std::vector<float> result;
    
    for (auto v : array)
    {
        if (v >= value)
            result.push_back (v);
    }
    
    return result;
}

//==========================================================================================
float BeatTrackingEvaluationToolbox::maxElement (std::vector<float> array)
{
    return *std::max_element(array.begin(), array.end());
}