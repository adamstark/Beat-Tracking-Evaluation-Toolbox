
#include "doctest.h"
#include "TestHelpers.h"
#include <BeatTrackingEvaluationToolbox.h>

//=============================================================
TEST_SUITE ("Continuity")
{
    //------------------------------------------------------------
    TEST_CASE ("Both Empty Sequences")
    {
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (std::vector<double>(), std::vector<double>());
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
    }
     
    //------------------------------------------------------------
    TEST_CASE ("Empty Beat Sequence")
    {
        std::vector<double> annotations;
     
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
     
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (std::vector<double>(), annotations);
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
    }
     
    //------------------------------------------------------------
    TEST_CASE ("Empty Annotation Sequence")
    {
        std::vector<double> beats;

        for (int i = 1; i <= 360; i++)
            beats.push_back (i / 2.);

        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, std::vector<double>());
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("Identical Sequences")
    {
        std::vector<double> beats;
        
        for (int i = 1; i <= 360; i++)
            beats.push_back (i / 2.);
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, beats);
        CHECK_EQ (result.cmlC, 100.);
        CHECK_EQ (result.cmlT, 100.);
        CHECK_EQ (result.amlC, 100.);
        CHECK_EQ (result.amlT, 100.);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats within tolerance window with positive offset and correct period")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        double annotationInterval = 0.5f;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] + annotationInterval * 0.1);
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations);
        CHECK_EQ (result.cmlC, 100.);
        CHECK_EQ (result.cmlT, 100.);
        CHECK_EQ (result.amlC, 100.);
        CHECK_EQ (result.amlT, 100.);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats within tolerance window with negative offset and correct period")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        double annotationInterval = 0.5f;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] - annotationInterval * 0.1);
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations);
        CHECK_EQ (result.cmlC, 100.);
        CHECK_EQ (result.cmlT, 100.);
        CHECK_EQ (result.amlC, 100.);
        CHECK_EQ (result.amlT, 100.);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside of tolerance window with positive offset and correct period")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        double annotationInterval = 0.5f;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] + annotationInterval * 0.2);
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations);
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats outside of tolerance window with negative offset and correct period")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        double annotationInterval = 0.5f;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
            beats.push_back (annotations[i] - annotationInterval * 0.2);
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations);
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
    }
    
    //------------------------------------------------------------
    TEST_CASE ("All beats within tolerance window but violating period condition")
    {
        std::vector<double> annotations;
        std::vector<double> beats;
        double annotationInterval = 0.5f;
        
        for (int i = 1; i <= 360; i++)
            annotations.push_back (i / 2.);
        
        for (int i = 0; i < annotations.size(); i++)
        {
            if (i % 2 == 0)
                beats.push_back (annotations[i] + annotationInterval * 0.1);
            else
                beats.push_back (annotations[i] - annotationInterval * 0.1);
        }
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations);
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
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
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations);
        
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
        
        FMeasureResult result = BeatTrackingEvaluationToolbox::evaluateBeatsFMeasure (beats, annotations);
        
        CHECK (result.fMeasure == doctest::Approx (50).epsilon (0.0001));
        CHECK (result.precision == doctest::Approx (33.333333333).epsilon (0.0001));
        CHECK_EQ (result.recall, 100.0);
        CHECK (result.accuracy == doctest::Approx (33.333333333).epsilon (0.0001));
    }
}
