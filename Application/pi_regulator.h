#ifndef __PI_REGULATOR_H
#define __PI_REGULATOR_H
typedef struct
{
  
  float   fDefKpGain;           /**< Default @f$K_{pg}@f$ gain */
  float   fDefKiGain;           /**< Default @f$K_{ig}@f$ gain */
  
  float fKpGain;
  float fKiGain;
  float fIntegralTerm;

  
  float fUpperIntegralLimit;
  float fLowerIntegralLimit;
  
  float fLowerLimit;
  float fUpperLimit; 
  
  float fKs;
  float fAntiWindTerm;
  
  int bAntiWindUpActivation;
  float fExecFrequencyHz;
  
} PI_Float_Handle_t;

void PI_Float_HandleInit(PI_Float_Handle_t *pHandle);
float PI_Float_Calc(PI_Float_Handle_t *pHandle, float fProcessVarError);

#endif /* __PI_FLOAT_REGULATOR_H */

