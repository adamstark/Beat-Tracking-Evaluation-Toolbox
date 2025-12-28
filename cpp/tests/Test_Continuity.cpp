
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
     
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (std::vector<double>(), annotations, 0.);
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

        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, std::vector<double>(), 0.);
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
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, beats, 0.);
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
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations, 0.);
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
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations, 0.);
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
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations, 0.);
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
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations, 0.);
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
        
        ContinuityResult result = BeatTrackingEvaluationToolbox::evaluateBeatsContinuity (beats, annotations, 0.);
        CHECK_EQ (result.cmlC, 0.);
        CHECK_EQ (result.cmlT, 0.);
        CHECK_EQ (result.amlC, 0.);
        CHECK_EQ (result.amlT, 0.);
    }
}
