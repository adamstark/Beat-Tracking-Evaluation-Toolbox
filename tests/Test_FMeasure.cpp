
#include "doctest.h"
#include "TestHelpers.h"
#include <BeatTrackingEvaluationToolbox.h>

//=============================================================
TEST_SUITE ("FMeasure")
{
    //------------------------------------------------------------
    TEST_CASE ("Both Empty Sequences")
    {        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (std::vector<double>(), std::vector<double>());
        
        CHECK_EQ (result.fMeasure, 0.0);
        CHECK_EQ (result.precision, 0.0);
        CHECK_EQ (result.recall, 0.0);
        CHECK_EQ (result.accuracy, 0.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Empty Beat Sequence")
    {
        std::vector<double> annotations;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (std::vector<double>(), annotations);
        
        CHECK_EQ (result.fMeasure, 0.0);
        CHECK_EQ (result.precision, 0.0);
        CHECK_EQ (result.recall, 0.0);
        CHECK_EQ (result.accuracy, 0.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Empty Annotation Sequence")
    {
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            beats.push_back (i / 2.);
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, std::vector<double>());
        
        CHECK_EQ (result.fMeasure, 0.0);
        CHECK_EQ (result.precision, 0.0);
        CHECK_EQ (result.recall, 0.0);
        CHECK_EQ (result.accuracy, 0.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Identical Sequences")
    {
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            beats.push_back (i / 2.);
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, beats);
        
        CHECK_EQ (result.fMeasure, 100.0);
        CHECK_EQ (result.precision, 100.0);
        CHECK_EQ (result.recall, 100.0);
        CHECK_EQ (result.accuracy, 100.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats within tolerance window")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] + randomPositiveOrNegativeOffset (0., 0.06));
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations, 0.07, 0.);
        
        CHECK_EQ (result.fMeasure, 100.0);
        CHECK_EQ (result.precision, 100.0);
        CHECK_EQ (result.recall, 100.0);
        CHECK_EQ (result.accuracy, 100.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside tolerance window")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] + randomPositiveOrNegativeOffset (0.071, 0.1));
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations);
        
        CHECK_EQ (result.fMeasure, 0.0);
        CHECK_EQ (result.precision, 0.0);
        CHECK_EQ (result.recall, 0.0);
        CHECK_EQ (result.accuracy, 0.0);
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
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations);
        
        CHECK_EQ (result.fMeasure, 0.0);
        CHECK_EQ (result.precision, 0.0);
        CHECK_EQ (result.recall, 0.0);
        CHECK_EQ (result.accuracy, 0.0);
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
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations);
        
        CHECK (result.fMeasure == doctest::Approx (66.6666666667).epsilon (0.0001));
        CHECK_EQ (result.precision, 50.0);
        CHECK_EQ (result.recall, 100.0);
        CHECK_EQ (result.accuracy, 50.0);
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
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations, 0.07, 0.);
        
        CHECK (result.fMeasure == doctest::Approx (66.6666666667).epsilon (0.0001));
        CHECK_EQ (result.precision, 100.0);
        CHECK_EQ (result.recall, 50.0);
        CHECK_EQ (result.accuracy, 50.0);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Multiple correct beats per annotation")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
        {
            beats.push_back (annotations[i]);
            beats.push_back (annotations[i] + 0.01);
            beats.push_back (annotations[i] - 0.01);
        }
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations, 0.07, 0.);
        
        CHECK (result.fMeasure == doctest::Approx (50).epsilon (0.0001));
        CHECK (result.precision == doctest::Approx (33.333333333).epsilon (0.0001));
        CHECK_EQ (result.recall, 100.0);
        CHECK (result.accuracy == doctest::Approx (33.333333333).epsilon (0.0001));
    }
}
