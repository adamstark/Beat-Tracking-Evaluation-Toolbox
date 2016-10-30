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

#include <iostream>
#include <vector>
#include <assert.h>
#include <numeric>
#include <math.h>

//==========================================================================================
/** */
struct ContinuityResult
{
    double cmlC = 0.;
    double cmlT = 0.;
    double amlC = 0.;
    double amlT = 0.;
};

//==========================================================================================
/** */
class BeatTrackingEvaluationToolbox
{
public:
    //==========================================================================================
    /** Evaluates a given beat sequence against some ground annotation on all evaluation measures
     * and returns the results in a vector
     * @param beats sequence of estimated beat times (in seconds)
     * @param annotations sequence of ground truth beat annotations (in seconds)
     */
    static std::vector<double> evaluate (std::vector<double> beats, std::vector<double> annotations);

    //==========================================================================================
    /** Calculates continuity based accuracy values as used in (Hainsworth, 2004) and (Klapuri et al, 2006)
     * @param beats sequence of estimated beat times (in seconds)
     * @param annotations sequence of ground truth beat annotations (in seconds)
     *
     * @returns a ContinuityResult struct containing scores for:
     *
     *      cmlC - beat tracking accuracy, continuity required at the correct metrical level
     *      cmlT - beat tracking accuracy, continuity not required at the correct metrical level
     *      amlC - beat tracking accuracy, continuity required at allowed metrical levels
     *      amlT - beat tracking accuracy, continuity not required at allowed metrical levels
     *
     * References:
     * -----------
     *  - S. Hainsworth, "Techniques for the automated analysis of musical audio," Ph.D. dissertation, Department of Engineering, Cambridge University, 2004.
     *  - A. P. Klapuri, A. Eronen, and J. Astola, "Analysis of the meter of acoustic musical signals," IEEE Transactions on Audio, Speech and Language Processing, vol. 14, no. 1, pp. 342-355, 2006.
     */
    static ContinuityResult evaluateBeatsContinuity (std::vector<double> beats, std::vector<double> annotations);

    //==========================================================================================
    /** Calculates the F-Measure as used in (Dixon, 2006) and (Dixon, 2007).
     * @param beats sequence of estimated beat times (in seconds)
     * @param annotations sequence of ground truth beat annotations (in seconds)
     *
     * @Returns the F-Measure score for the beat sequence given the annotations
     *
     * References:
     * ------------
     * - S. Dixon, "Automatic extraction of tempo and beat from expressive performances," Journal of New Music Research, vol. 30, pp. 39-58, 2001.
     * - S. Dixon, "Onset detection revisited," in Proceedings of 9th International Conference on Digital Audio Effects (DAFx), Montreal, Canada, pp. 133-137, 2006.
     * - S. Dixon, "Evaluation of audio beat tracking system beatroot," Journal of New Music Research, vol. 36, no. 1, pp. 39-51, 2007.
     */
    static double evaluateBeatsFMeasure (std::vector<double> beats, std::vector<double> annotations, double toleranceWindowInSeconds = 0.07);

    //==========================================================================================
    /** Calculate the PScore accuracy value as used in McKinney et al, 2007.
     * @param beats sequence of estimated beat times (in seconds)
     * @param annotations sequence of ground truth beat annotations (in seconds)
     *
     * @returns PScore estimate of beat tracking accuracy
     *
     * References:
     * ------------
     * - M. F. McKinney, D. Moelants, M. E. P. Davies, and A. Klapuri, "Evaluation of audio beat tracking and music tempo extraction algorithms," Journal of New Music Research, vol. 36, no. 1, pp. 1-16, 2007.
     */
    static double evaluateBeatsPScore (std::vector<double> beats, std::vector<double> annotations, double threshold = 0.2);

    //================================== Goto Accuracy =====================================
    /** Calculate the Goto and Muraoka's accuracy value as used in (Goto and Muraoka, 1997).
     * @param beats sequence of ground truth beat annotations (in seconds)
     * @param annotations sequence of estimated beat times (in seconds)
     *
     * @returns Goto beat tracking accuracy
     *
     * References:
     * ------------
     * - M. Goto and Y. Muraoka, "Issues in evaluating beat tracking systems," in Working Notes of the IJCAI-97 Workshop on Issues in AI and Music - Evaluation and Assessment, 1997, pp. 9-16.
     */
    static double evaluateBeatsGoto (std::vector<double> beats, std::vector<double> annotations, double threshold = 0.35, double mu = 0.2, double sigma = 0.2);

    //==========================================================================================
    /** Calculates the Cemgil et al's accuracy value as used in (Cemgil et al, 2001).
     * @param beats sequence of estimated beat times (in seconds)
     * @param annotations sequence of ground truth beat annotations (in seconds)
     *
     * @Returns the beat tracking accuracy
     *
     * References:
     * -----------
     * - A. T. Cemgil, B. Kappen, P. Desain, and H. Honing, "On tempo tracking: Tempogram representation and Kalman filtering," Journal Of New Music Research, vol. 28, no. 4, pp. 259-273, 2001
     */
    static double evaluateBeatsCemgilAccuracy (std::vector<double> beats, std::vector<double> annotations, double sigma = 0.04);

    //==========================================================================================
    /** Calculates Cemgil accuracy but allowing for continuity-based allowed metrical levels
     * @param beats sequence of estimated beat times (in seconds)
     * @param annotations sequence of ground truth beat annotations (in seconds)
     *
     * @returns Cemgiil beat tracking accuracy given allowed metrical levels
     *
     * References:
     * ------------
     * - "Musicians and Machines: Bridging the Semantic Gap in Live Performance", Adam Stark, PhD Thesis, 2011, Chapter 3
     */
    static double evaluateBeatsAmlCemgilAccuracy (std::vector<double> beats, std::vector<double> annotations, double sigma = 0.04);
    
    //================================== Information Gain ===============================
    /** Calculates the information gain as used in (Davies et al, 2010).
     * @param anns sequence of ground truth beat annotations (in seconds)
     * @param beats sequence of estimated beat times (in seconds)
     *
     * @returns beat tracking information gain
     *
     * References:
     * ------------
     * - M. E. P. Davies, N. Degara and M. D. Plumbley, "Measuring the performance of beat tracking algorithms algorithms using a beat error histogram,"  accepted to IEE Signal Processing Letters.
     */
    static double evaluateBeatsInformationGain (std::vector<double> beats, std::vector<double> annotations, int numHistogramBins = 40);

private:
    
    //==========================================================================================
    struct ContinuityEvaluationScores
    {
        double totalAccuracy = 0.;
        double continuityAccuracy = 0.;
    };

    //==========================================================================================
    static double sumOfCrossCorrelation (std::vector<double> v1, std::vector<double> v2, int maximumLag);
    static ContinuityEvaluationScores continutityEvaluation (std::vector<double> beats, std::vector<double> annotations, double phaseThreshold = 0.175, double periodThreshold = 0.175);
    static std::vector<double> createSetOfAnnotationsAtdoubleTempo (std::vector<double> annotations);
    static std::vector<double> getEveryOtherAnnotationStartingAtIndex (std::vector<double> annotations, int startIndex);
    static std::vector<int> getIndicesOfNonZeroElements (std::vector<double> array);
    static std::vector<double> removeIfLessThanValue (std::vector<double> array, double value);
    static double medianOfVector (std::vector<int> vector);
    static int getIndexOfNearestElement (std::vector<double> array, double value);
    
    //==========================================================================================
    // Information Gain methods
    static std::vector<double> findBeatError (std::vector<double> beats, std::vector<double> annotations);
    static std::vector<double> calculateBeatErrorHistogram (std::vector<double> beatError, std::vector<double> histogramBins);
    static double findEntropy (std::vector<double> beatErrorHistogram);
    
    //==========================================================================================
    template <typename T>
    static double meanOfVector (std::vector<T> vector);
    
    //==========================================================================================
    template <typename T>
    static T maxElement (std::vector<T> array);
    
    //==========================================================================================
    template <typename T>
    static int argMax (std::vector<T> array);
};
