//
//  main.cpp
//  BeatTrackingEvaluationToolbox
//
//  Created by Adam Stark on 14/02/2016.
//  Copyright © 2016 Adam Stark. All rights reserved.
//

#include <iostream>
#include "../../src/BeatTrackingEvaluationToolbox.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    
    std::vector<float> x = {0.2,1.4,5.4,10.2};
    std::vector<float> y = {1.2,2.4,5.43,6.2};
    
    float fMeasure = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (x, y);
    
    std::cout << fMeasure << std::endl;
    
    return 0;
}
