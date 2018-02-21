#ifndef constant_blackscholes_process_hpp
#define constant_blackscholes_process_hpp

#include <ql/stochasticprocess.hpp>
#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/termstructures/volatility/equityfx/blackvoltermstructure.hpp>

namespace QuantLib {

	class constantBlackScholesProcess : public StochasticProcess1D {

	private:
		Handle<Quote> x0_;
		Handle<YieldTermStructure> riskFreeRate_;
		Handle<YieldTermStructure> 	dividendYield_;
		Handle<BlackVolTermStructure> blackVolatility_;
		Real constDrift_;
		Real constDiffusion_;
		Date exerciceDate_;
		Real strike_;
	public:

		constantBlackScholesProcess(
			const Handle <Quote> x0,
			const Date exerciceDate,
			const Real strike,
			const Handle<YieldTermStructure>& riskFreeTS,
			const Handle<BlackVolTermStructure>& blackVolTS,
			const Handle<YieldTermStructure>& dividendYieldTS,
			boost::shared_ptr<discretization>& disc);

		Real drift(Time t, Real x) const;

		Real diffusion(Time t, Real x) const;


		Real variance(const StochasticProcess1D&,
			Time t0, Real x0, Time dt) const;

		Real x0() const;
	};


}

#endif
