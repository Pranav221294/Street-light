#include "pi_regulator.h"

void PI_Float_HandleInit(PI_Float_Handle_t *pHandle)
{
    pHandle->fKpGain = pHandle->fDefKpGain;
    pHandle->fKiGain = pHandle->fDefKiGain;

    pHandle->fIntegralTerm = 0.0f;
    pHandle->fAntiWindTerm = 0.0f;
    if (pHandle->bAntiWindUpActivation)
    {
        pHandle->bAntiWindUpActivation = 1;
    }
}

/**
 * @brief  This function computes the output of a PI regulator sum of its
 *         proportional and integral terms
 * @param  Float_PIReg PI regulator object
 * @param  float Actual process variable error, intended as the reference
 *         value minus the actual process variable value
 * @retval float PI output
 */
float PI_Float_Calc(PI_Float_Handle_t *pHandle, float fProcessVarError)
{
    float fProportional_Term, fOutput, fOutputSat;

    /* Proportional term computation*/
    fProportional_Term = pHandle->fKpGain * fProcessVarError;
    fOutput = fProportional_Term;

    /* Integral term computation */
    if (pHandle->fKiGain == 0.0f)
    {
        pHandle->fIntegralTerm = 0.0f;
    }
    else
    {
        //    pHandle->fIntegralTerm+= ((pHandle->fKiGain *(fProcessVarError)) - (pHandle->fKiGain*pHandle->fAntiWindTerm));
        pHandle->fIntegralTerm += ((pHandle->fKiGain * (fProcessVarError)) / pHandle->fExecFrequencyHz);

        if (pHandle->fIntegralTerm > pHandle->fUpperIntegralLimit)
        {
            pHandle->fIntegralTerm = pHandle->fUpperIntegralLimit;
        }
        else if (pHandle->fIntegralTerm < pHandle->fLowerIntegralLimit)
        {
            pHandle->fIntegralTerm = pHandle->fLowerIntegralLimit;
        }

        fOutput += pHandle->fIntegralTerm;
    }

    fOutputSat = fOutput;

    /* Saturation */
    if (fOutput > pHandle->fUpperLimit)
    {
        fOutputSat = pHandle->fUpperLimit;
    }

    if (fOutput < pHandle->fLowerLimit)
    {
        fOutputSat = pHandle->fLowerLimit;
    }

    if (pHandle->bAntiWindUpActivation == (1))
    {
        pHandle->fAntiWindTerm = (fOutput - fOutputSat) * pHandle->fKs;
    }
    else
    {
        pHandle->fAntiWindTerm = 0.0f;
    }

    fOutput = fOutputSat;

    return (fOutput);
}
