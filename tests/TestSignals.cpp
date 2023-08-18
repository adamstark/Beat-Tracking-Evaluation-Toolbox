//
//  TestSignals.cpp
//  Tests
//
//  Created by Adam Stark on 03/11/2020.
//

#include "TestSignals.h"

namespace examples
{
    std::vector<double> beats1 = {0.47600907, 0.91718821, 1.4860771,   2.20589569,  2.79800454,  3.42494331,
                        4.04027211,  4.65560091,  5.25931973,  5.85142857, 6.45514739,  7.05886621,
                        7.65097506,  8.26630385,  8.87002268,  9.46213152, 10.06585034, 10.66956916,
                       11.27328798, 11.8770068,  12.4923356,  13.08444444, 13.68816327, 14.31510204,
                       14.93043084, 15.51092971, 16.07981859, 16.70675737, 17.29886621, 17.90258503,
                       18.50630385, 19.11002268, 19.69052154, 20.30585034, 20.90956916, 21.51328798,
                       22.10539683, 22.69750567, 23.28961451, 23.87011338, 24.46222222, 25.07755102,
                       25.69287982, 26.28498866, 26.90031746, 27.4924263,  28.09614512, 28.67664399,
                       29.28036281, 29.89569161, 30.48780045, 31.0799093,  31.6952381,  32.28734694,
                       32.89106576, 33.47156463, 34.06367347, 34.65578231, 35.27111111, 35.87482993,
                       36.47854875, 37.08226757, 37.68598639, 38.26648526, 38.87020408, 39.49714286,
                       40.10086168, 40.69297052, 41.30829932, 41.90040816, 42.50412698, 43.08462585,
                       43.67673469, 44.29206349, 44.90739229, 45.51111111, 46.10321995, 46.70693878,
                       47.32226757, 47.90276644, 48.51809524, 49.13342404, 49.74875283, 50.34086168,
                       50.9445805,  51.5599093,  52.16362812, 52.77895692, 53.32462585, 53.89351474,
                        54.50884354, 55.10095238, 55.7046712,  56.32};

    std::vector<double> annotations1 = {0.45496599,  1.04421769,  1.61614512,  2.21405896,  2.82716553,  3.44712018,
        4.05657596,  4.64040816,  5.25564626,  5.85358277,  6.44283447,  7.04941043,
        7.6646712,   8.2799093,   8.86049887,  9.44108844, 10.05895692, 10.63086168,
       11.28501134, 11.88736961, 12.47662132, 13.06587302, 13.68113379, 14.27038549,
       14.89430839, 15.49222222, 16.09879819, 16.69673469, 17.30331066, 17.90124717,
       18.50782313, 19.10573696, 19.68893424, 20.2955102,  20.89344671, 21.50002268,
       22.07195011, 22.68718821, 23.25045351, 23.84836735, 24.46362812, 25.08755102,
       25.67680272, 26.27471655, 26.88997732, 27.49580499, 28.08861678, 28.67036281,
       29.28408163, 29.88217687, 30.47142857, 31.08666667, 31.67594104, 32.25653061,
       32.85444444, 33.44369615, 34.05027211, 34.67419501, 35.26344671, 35.88736961,
       36.47662132, 37.07453515, 37.64646259, 38.26648526, 38.88823129, 39.47811791,
       40.06278912, 40.6646712,  41.2885941,  41.89517007, 42.46709751, 43.07367347,
       43.68893424, 44.28684807, 44.8847619,  45.49136054, 46.09793651, 46.72185941,
       47.29378685, 47.87437642, 48.50378685, 49.13521542, 49.74569161, 50.32578231,
       50.9399093,  51.5385034,  52.12151927, 52.68902494, 53.28759637, 53.8861678,
        54.46920635, 55.09111111};
    
    
    std::vector<double> beats2 = {0.5456689342403628, 0.9984580498866213, 1.4628571428571429, 1.9272562358276644, 2.380045351473923, 2.8444444444444446, 3.1927437641723357, 3.599092970521542, 4.028662131519274, 4.46984126984127, 4.911020408163266, 5.340589569160998, 5.7817687074829935, 6.211337868480726, 6.640907029478458, 7.082086167800454, 7.5232653061224495, 7.964444444444444, 8.40562358276644, 8.846802721088435, 9.276371882086169, 9.729160997732427, 10.147120181405896, 10.599909297052154, 11.029478458049887, 11.470657596371883, 11.911836734693878, 12.353015873015874, 12.782585034013605, 13.223764172335601, 13.664943310657597, 14.09451247165533, 14.524081632653061, 14.965260770975057, 15.418049886621315, 15.870839002267575, 16.300408163265306, 16.72997732426304, 17.147936507936507, 17.589115646258502, 18.041904761904764, 18.471473922902494, 18.91265306122449, 19.353832199546485, 19.783401360544218, 20.224580498866214, 20.654149659863947, 21.095328798185943, 21.536507936507938, 21.977687074829934, 22.41886621315193, 22.860045351473925, 23.301224489795917, 23.73079365079365, 24.160362811791384, 24.589931972789117, 25.042721088435375};
    
    
    std::vector<double> annotations2 = {0.06458049886621316, 0.5213832199546485, 0.9483900226757369, 1.3753968253968254, 1.8222675736961451, 2.269138321995465, 2.6862131519274377, 3.113219954648526, 3.579954648526077, 3.9970521541950115, 4.443922902494331, 4.87092970521542, 5.33766439909297, 5.7746031746031745, 6.171814058956916, 6.618684807256236, 7.085419501133787, 7.502494331065759, 7.959297052154195, 8.386326530612244, 8.813333333333333, 9.260204081632653, 9.657414965986394, 10.100816326530612, 10.567551020408164, 10.984625850340136, 11.451360544217687, 11.88829931972789, 12.33517006802721, 12.762176870748299, 13.209070294784581, 13.626145124716553, 14.073015873015873, 14.490090702947846, 14.936961451247166, 15.420408163265305, 15.84063492063492, 16.267641723356007, 16.69467120181406, 17.111746031746033, 17.568548752834467, 18.00548752834467, 18.460521541950115, 18.915555555555557, 19.316303854875283, 19.755714285714287, 20.172448979591838, 20.626485260770973, 21.080521541950112, 21.50968253968254, 21.951292517006802, 22.399115646258505, 22.83448979591837, 23.263650793650793, 23.692834467120182, 24.14063492063492, 24.56981859410431};
}
