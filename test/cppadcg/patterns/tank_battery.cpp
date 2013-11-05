/* --------------------------------------------------------------------------
 *  CppADCodeGen: C++ Algorithmic Differentiation with Source Code Generation:
 *    Copyright (C) 2013 Ciengis
 *
 *  CppADCodeGen is distributed under multiple licenses:
 *
 *   - Eclipse Public License Version 1.0 (EPL1), and
 *   - GNU General Public License Version 3 (GPL3).
 *
 *  EPL1 terms and conditions can be found in the file "epl-v10.txt", while
 *  terms and conditions for the GPL3 can be found in the file "gpl3.txt".
 * ----------------------------------------------------------------------------
 * Author: Joao Leal
 */
#include "CppADCGPatternModelTest.hpp"
#include "../models/tank_battery.hpp"


namespace CppAD {

    const size_t nTanks = 6; // number of stages

    class CppADCGPatternTankBatTest : public CppADCGPatternModelTest {
    public:
        typedef double Base;
        typedef CppAD::CG<Base> CGD;
        typedef CppAD::AD<CGD> ADCGD;
    public:

        inline CppADCGPatternTankBatTest(bool verbose = false, bool printValues = false) :
            CppADCGPatternModelTest("TankBattery",
                                    nTanks, // ns
                                    1, // nm
                                    1, // npar
                                    nTanks * 1, // m
                                    verbose, printValues) {
            this->verbose_ = true;
        }

        virtual std::vector<ADCGD> modelFunc(const std::vector<ADCGD>& x) {
            return tankBatteryFunc(x);
        }

        virtual std::vector<std::set<size_t> > getRelatedCandidates() {
            std::vector<std::set<size_t> > relatedDepCandidates(1);
            for (size_t i = 0; i < nTanks; i++) relatedDepCandidates[0].insert(i);
            return relatedDepCandidates;
        }

    };

}

using namespace CppAD;

/**
 * @test test the usage of loops for the generation of the tank battery model
 *       with the creation of differential information for all variables
 */
TEST_F(CppADCGPatternTankBatTest, tankBatteryAllVars) {
    modelName += "AllVars";
    
    /**
     * Tape model
     */
    std::auto_ptr<ADFun<CGD> > fun;
    this->tape(fun);

    /**
     * test
     */
    this->test(*fun.get());

}

/**
 * @test test the usage of loops for the generation of the tank battery model
 *       with the creation of differential information only for states and
 *       controls
 */
TEST_F(CppADCGPatternTankBatTest, tankBattery) {
    using namespace CppAD::extra;

    /**
     * Tape model
     */
    std::auto_ptr<ADFun<CGD> > fun;
    this->tape(fun);

    /**
     * Determine the relevant elements
     */
    this->defineCustomSparsity(*fun.get());

    /**
     * test
     */
    this->test(*fun.get());

}