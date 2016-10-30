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
    
    
    std::vector<double> x = {5.5,6.,6.5,7.,7.5};
    std::vector<double> y = {5.6,6.55,7.51};
    
    std::vector<double> z;
    
    for (int i = 0; i < 120; i++)
    {
        z.push_back ((double)i / 2.0);
    }
    
//    
//    
//    std::vector<float> z (3, 0);
//    x.insert(x.end(),z.begin(),z.end());
//    x.insert(x.begin(),z.begin(),z.end());
//    
//    for (int i = 0;i < x.size();i++)
//    {
//        std::cout << x[i] << std::endl;
//    }
//    
    
    double result = BeatTrackingEvaluationToolbox::evaluateBeatsGoto (z, z);
    std::cout << result << std::endl;
    //ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (x, y);
    
    /*
    std::cout << result.cmlC << std::endl;
    std::cout << result.cmlT << std::endl;
    std::cout << result.amlC << std::endl;
    std::cout << result.amlT << std::endl;
    
    */
    return 0;
}
