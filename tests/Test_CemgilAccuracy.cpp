
#include "doctest.h"
#include "TestHelpers.h"
#include "TestSignals.h"
#include <BeatTrackingEvaluationToolbox.h>

//=============================================================
TEST_SUITE ("Cemgil Accuracy")
{
    //------------------------------------------------------------
    TEST_CASE ("Both Empty Sequences")
    {
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (std::vector<double>(), std::vector<double>());
        CHECK_EQ (result, 0.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Empty Beat Sequence")
    {
        std::vector<double> annotations;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (std::vector<double>(), annotations);
        CHECK_EQ (result, 0.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Empty Annotation Sequence")
    {
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            beats.push_back (i / 2.);
        
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (beats, std::vector<double>());
        CHECK_EQ (result, 0.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Identical Sequences")
    {
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            beats.push_back (i / 2.);
        
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (beats, beats);
        
        CHECK_EQ (result, 100.0);
    }
        
    //------------------------------------------------------------
    double getCemgilAccuracyForBeatsWithOffsetInMilliseconds (double offsetInMilliseconds)
    {
        const double offsetInSeconds = offsetInMilliseconds / 1000.;
        
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] + randomSign() * offsetInSeconds);
        
        return BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (beats, annotations);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 200ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (200.0);
        CHECK (result == doctest::Approx (0.00037266531720786647).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 150ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (150.0);
        CHECK (result == doctest::Approx (0.08838263069350508).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 100ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (100.0);
        CHECK (result == doctest::Approx (4.39369336234074).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 50ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (50.0);
        CHECK (result == doctest::Approx (45.783336177161424).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 25ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (25.0);
        CHECK (result == doctest::Approx (82.25775623986645).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 20ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (20.0);
        CHECK (result == doctest::Approx (88.24969025845955).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 15ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (15.0);
        CHECK (result == doctest::Approx (93.21024923595276).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 10ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (10.0);
        CHECK (result == doctest::Approx (96.92332344763442).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window by 5ms")
    {
        double result = getCemgilAccuracyForBeatsWithOffsetInMilliseconds (5.0);
        CHECK (result == doctest::Approx (99.22179382602435).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats on the offbeat")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] + 0.25);
        
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (beats, annotations);
        
        CHECK (result == doctest::Approx (0.).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Double speed")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
        {
            beats.push_back (annotations[i]);
            beats.push_back (annotations[i] + 0.25);
        }
        
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (beats, annotations);
    
        CHECK (result == doctest::Approx (66.66666667).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Half speed")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i += 2)
            beats.push_back (annotations[i]);
        
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (beats, annotations);
        
        CHECK (result == doctest::Approx (66.66666667).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Sample Sequence 1 - Compare to MADMOM")
    {
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (examples::beats1, examples::annotations1);
        CHECK (result == doctest::Approx (85.82089134179277).epsilon (0.0001));
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Sample Sequence 2 - Compare to MADMOM")
    {
        double result = BeatTrackingEvaluationToolbox::evaluateBeatsCemgilAccuracy (examples::beats2, examples::annotations2);
        CHECK (result == doctest::Approx (70.2781695147737).epsilon (0.0001));
    }
    
    
}
