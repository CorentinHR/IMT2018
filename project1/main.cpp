#include "constantblackscholesprocess.hpp"
#include "mceuropeanengine.hpp"
#include <ql/pricingengines/vanilla/mceuropeanengine.hpp>
#include <ql/quantlib.hpp>
#include <time.h>
using namespace QuantLib;
int main() {

	try {
		// Specify (1) calendar for specific market
		//         (2) date count convention
		//         (3) current date
		//         (4) maturity date

		Calendar calendar = TARGET();
		DayCounter dayCounter = Actual365Fixed();//Business252();
		Date t0(28, February, 2018);
		//Settings::instance().evaluationDate() = t0;
		Date T(28, February, 2019);

		// Specify (5) option type
		//         (6) current stock price
		//         (7) strike price

		Option::Type type(Option::Call);
		Real S0 = 90;
		Real K = 100;

		// Specify (8) dividend yield
		//         (9) interest rate
		//         (10) volatility

		Spread q = 0.02;
		Rate r = 0.05;
		Volatility sigma = 0.20;

		// Construct the stock quote object, the pointer
		// and the handle of the object
		Handle<Quote> underlying(
			boost::shared_ptr<Quote>(
				new SimpleQuote(S0)
				));

		// Construct the dividend TS object, the pointer
		// and the handle of the object
		Handle<YieldTermStructure> flatDividendTS(
			boost::shared_ptr<YieldTermStructure>(
				new FlatForward(t0, q, dayCounter)
				));

		// Construct the interest rate TS object, the pointer
		// and the handle of the object
		Handle<YieldTermStructure> flatRateTS(
			boost::shared_ptr<YieldTermStructure>(
				new FlatForward(t0, r, dayCounter)
				));

		// Construct the volatility TS object, the pointer
		// and the handle of the object
		Handle<BlackVolTermStructure> flatVolTS(
			boost::shared_ptr<BlackVolTermStructure>(
				new BlackConstantVol(t0, calendar, sigma, dayCounter)
				));

		// Construct the Black Scholes process object and
		// the pointer of the object
		boost::shared_ptr<GeneralizedBlackScholesProcess> bsmProcess(
			new GeneralizedBlackScholesProcess(
				underlying, flatDividendTS, flatRateTS, flatVolTS)
		);

		// Construct the European option exercise object and
		// the pointer of the object
		boost::shared_ptr<Exercise> europeanExercise(
			new EuropeanExercise(T)
		);


		// Construct the option payoff object and
		// the pointer of the object

		boost::shared_ptr<StrikedTypePayoff> payoff(
			new PlainVanillaPayoff(type, K)
		);


		//Construct the Options

		VanillaOption eurOption(payoff, europeanExercise);
		VanillaOption eurOption2(payoff, europeanExercise);

		//Pricing Engine

		eurOption.setPricingEngine(
			boost::shared_ptr<PricingEngine>(
				new AnalyticEuropeanEngine(bsmProcess)
				));
		eurOption2.setPricingEngine(
			boost::shared_ptr<PricingEngine>(
				new AnalyticEuropeanEngine(bsmProcess)
				));
		std::cout << "" << std::endl;
		std::cout << "" << std::endl;
		Real OptionPrice = eurOption.NPV();
		std::cout << "OptionPrice = eurOption.NPV()=" << OptionPrice << std::endl;
		std::cout << "" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "MCEuropeanEngine with GeneralizedBlackScholesProcess" << std::endl;


		eurOption.setPricingEngine(
			boost::shared_ptr<PricingEngine>(
				new MCEuropeanEngine<PseudoRandom>(
					bsmProcess,
					10,
					Null<Size>(),
					true,
					false,
					10000,
					Null<Real>(),
					Null<Size>(), SeedGenerator::instance().get(),
					))
		);
		clock_t tStart = clock();
		std::cout << "Option Price " << eurOption.NPV() << std::endl;
		printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
		std::cout << "Error Estimation " << eurOption.errorEstimate() << std::endl;
		std::cout << "" << std::endl;
		std::cout << "" << std::endl;
		std::cout << "MCEuropeanEngine with constantBlackScholesProcess" << std::endl;
		eurOption2.setPricingEngine(
			boost::shared_ptr<PricingEngine>(
				new MCEuropeanEngine_2<PseudoRandom>(
					bsmProcess,
					10,
					Null<Size>(),
					true,
					false,
					10000,
					Null<Real>(),
					Null<Size>(), SeedGenerator::instance().get(),
					true
					))
		);
		clock_t tStart2 = clock();
		std::cout << "Option Price " << eurOption2.NPV() << std::endl;
		printf("Time taken: %.2fs\n", (double)(clock() - tStart2) / CLOCKS_PER_SEC);
		std::cout << "Error Estimation " << eurOption2.errorEstimate() << std::endl;
		std::cout << "" << std::endl;
		system("pause");
		return 0;

	}
	catch (std::exception& e) {

		std::cerr << e.what() << std::endl;
		system("pause");
		return 1;
	}
	catch (...) {
		std::cerr << "unknown error" << std::endl;
		system("pause");
		return 1;
	}
}
