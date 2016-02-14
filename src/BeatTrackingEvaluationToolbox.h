

#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>


//==========================================================================================
/** */
class BeatTrackingEvaluationToolbox
{
public:
    
    static float evaluateBeatsFMeasure (std::vector<float> beats, std::vector<float> annotations);
    
private:
    
    static std::vector<float> removeIfLessThanValue (std::vector<float> array, float value);
    static float maxElement (std::vector<float> array);
    
};