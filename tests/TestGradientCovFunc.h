#ifndef TESTGRADIENTCOVFUNC_H_
#define TESTGRADIENTCOVFUNC_H_

#include "Test.h"
#include "covariance_functions/GaussianCF.h"
#include "covariance_functions/WhiteNoiseCF.h"
#include "covariance_functions/ExponentialCF.h"
#include "covariance_functions/Matern3CF.h"
#include "covariance_functions/SumCF.h"
#include "covariance_functions/ConstantCF.h"
#include "covariance_functions/Matern5CF.h"
#include "covariance_functions/NeuralNetCF.h"


using namespace std;
using namespace itpp;

class TestGradientCovFunc : public Test
{
public:
  TestGradientCovFunc();
  virtual ~TestGradientCovFunc();
  
  /**
   * Test gradient of Gaussian covariance function
   */
  static bool testGradientWhiteNoiseCF();
  
  /**
   * Test gradient of WhiteNoise covariance function
   */
  static bool testGradientGaussianCF();
    
  /**
     * Test gradient of Constant covariance function
     */
  static bool testGradientConstantCF();
  
  /**
   * Test gradient of sum of 2 covariance functions
   */ 
  static bool testGradientSumCF();

  /**
   * Test gradient of Matern3 covariance function
   */
  static bool testGradientMatern3CF();
  
  /**
   * Test gradient of Matern5 covariance function
   */
  static bool testGradientMatern5CF();
  
  /**
   * Test gradient of neural network covariance function
   */
  static bool testGradientNeuralNetCF();

  /**
    * Test gradient of anisotropic exponential covariance function
    */
   static bool testGradientExponentialCF();
  
  /**
   * Computes the error between analytic gradient matrix and finite differences
   * estimate. For each parameter of the covariance function, the mean and variance 
   * of the error (between the elements of the two gradient matrices) are displayed.
   * Returns true if mean, var and max below fixed tolerance (1e-3)
   */
  static bool matGradCheck(CovarianceFunction *cf);

private:
  
  /**
   * Computes the gradient matrix using finite differences
   * Pass in a covariance function and its parameters
   */
  static mat gradFiniteDifferences(CovarianceFunction *cf, mat X, int paramIndex);
  
};

#endif /*TESTGRADIENTCOVFUNC_H_*/
