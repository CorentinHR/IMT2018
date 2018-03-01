#include "constantblackscholesprocess.hpp"



namespace QuantLib {

	constantBlackScholesProcess::constantBlackScholesProcess(
		const Handle<Quote> x0,
		const Date exerciceDate,
		const Real strike,
		const Handle<YieldTermStructure>& riskFreeTS,
		const Handle<BlackVolTermStructure>& blackVolTS,
		const Handle<YieldTermStructure>& dividendYieldTS,
		boost::shared_ptr<discretization>& disc)
		:StochasticProcess1D(disc), x0_(x0), strike_(strike),
		riskFreeRate_(riskFreeTS), dividendYield_(dividendYieldTS), blackVolatility_(blackVolTS) {
		exerciceDate_ = exerciceDate;
		constDrift_ = riskFreeRate_->zeroRate(exerciceDate_, riskFreeRate_->dayCounter(), Continuous,
			NoFrequency, true) - dividendYield_->zeroRate(exerciceDate_, riskFreeRate_->dayCounter(), Continuous,
				NoFrequency, true);
		constDiffusion_ = blackVolatility_->blackVol(exerciceDate_,strike_);
	}



	Real constantBlackScholesProcess::drift(Time t, Real x) const {
		return constDrift_*x;
	}

	Real constantBlackScholesProcess::diffusion(Time t, Real x) const {
		return constDiffusion_*x;
	}

	Real constantBlackScholesProcess::variance(const StochasticProcess1D&,
		Time t0, Real x0, Time dt) const {
		return discretization_->variance(*this, t0, x0, dt);
	}

	Real constantBlackScholesProcess::x0()const {
		return x0_->value();
	}

}
