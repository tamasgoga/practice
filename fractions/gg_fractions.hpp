#ifndef FRACTIONS_GG_FRACTIONS_HPP
#define FRACTIONS_GG_FRACTIONS_HPP

/** Fractions library by Goga Tamás
Fractions can be of any arithmetic type. */

/** To circumvent the fact that Sublime Text's syntax highlighter shits itself when friendships are involved,
the following macro is going to be used instead. The macro is undefined within this file. */
#define GG_FRACTIONS_FRIEND friend
; // semicolon to align subsequent lines properly (yup, sublime text)

#include <cmath>
#include <exception>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>


namespace gg {

	//------------------------------------------------------------------
	// Helper functions & classes
	//------------------------------------------------------------------

	/** Euclidean greatest common divisor algorithm:
	Practically returns gcd(max(a,b), min(a,b)), to avoid throwing unnecessary exceptions.

	Corner cases:
		gcd(0, 0):
			Returns 0. This is mathematically correct, so I do not consider it an exception.
		gcd(real_with_fract_part, real_with_fract_part):
			Returns 1 (check specializations). I'll probably eliminate this case sometime in the future.
	*/
	template <typename Arithmetic>
	Arithmetic gcd(Arithmetic a, Arithmetic b) noexcept {
		static_assert(std::is_arithmetic<Arithmetic>::value, "gg::gcd() can only be used on arithmetic types.");
		static constexpr auto ZERO = static_cast<Arithmetic>(0);

		a = std::abs(a);
		b = std::abs(b);

		if (a < b)
			std::swap(a, b);
		if (a == ZERO)
			return ZERO;

		Arithmetic rem;
		do {
			rem = a % b;
			a = b;
			b = rem;
		} while (b != ZERO);

		return a;
	}


	/** GCD for floating points:
	Performs the original gcd if if both arguments' fractional part is 0.
	Otherwise returns 1 or 0, if the any argument == 0. */
	template <typename FloatingPoint>
	FloatingPoint gcd_fp(FloatingPoint a, FloatingPoint b) noexcept {
		static_assert(std::is_floating_point<FloatingPoint>::value, "gg::gcd_fp() can only be used on floating point types.");
		static constexpr auto ZERO = static_cast<FloatingPoint>(0);

		FloatingPoint iprt;
		if(std::modf(a, &iprt) != ZERO || std::modf(b, &iprt) != ZERO)
			return static_cast<FloatingPoint>(1);

		return static_cast<FloatingPoint>(gcd<long long>(static_cast<long long>(a), static_cast<long long>(b)));
	}


	/** Template specializations of gcd() for floating point values which calls gcd_fp(). */
	template<> inline float       gcd<>(float       a, float       b) noexcept { return gcd_fp(a, b); }
	template<> inline double      gcd<>(double      a, double      b) noexcept { return gcd_fp(a, b); }
	template<> inline long double gcd<>(long double a, long double b) noexcept { return gcd_fp(a, b); }


	/** Exception that is generally thrown when there's a 0 in the denominator, or other undefined case is encountered. */
	class IllegalFraction: public std::exception {
	public:
		std::string msg;
		IllegalFraction(const std::string& msg) : msg(msg) {;}
		virtual const char* what() const noexcept {return msg.c_str();}
		virtual ~IllegalFraction() {;}
	};



	//------------------------------------------------------------------
	// class: fraction
	//------------------------------------------------------------------

	/** Fraction defined on arithmetic types */
	template <typename Arithmetic>
	class fraction {
	public:
		/** Definitions */
		using type = Arithmetic;


		/** Attributes */
		Arithmetic numerator;
		Arithmetic denominator;


		/** Common values casted to Arithmetic */
		static constexpr auto ZERO = static_cast<Arithmetic>(0);
		static constexpr auto ONE  = static_cast<Arithmetic>(1);


		/** Default constructor (1/1): */
		fraction() noexcept
			: numerator(ONE)
			, denominator(ONE)
		{;}


		/** Whole number constructor */
		fraction(Arithmetic numerator) noexcept
			: numerator(numerator)
			, denominator(ONE)
		{;}


		/** Future plans */
		// fraction(double realNum) noexcept {
		// 	double integ;
		// 	double fract = std::modf(realNum, &integ);
		// 	// ...
		// }


		/** Proper fraction constructor:
			The type of the arguments can either be any arithmetic type or gg::fraction<T>. */
		fraction(Arithmetic numerator, Arithmetic denominator) noexcept
			: numerator(numerator)
			, denominator(denominator)
		{
			static_assert(std::is_arithmetic<Arithmetic>::value, "gg::fraction-s are only defined on arithmetic types.");
		}


		/** Constructs a fraction from two fractions of the same arithmetic type by dividing them (a/b). */
		fraction(const fraction& a, const fraction& b)
			: numerator(a.numerator * b.denominator)
			, denominator(a.denominator * b.numerator)
		{;}


		/** Copy constructor: */
		fraction(const fraction& other) noexcept
			: numerator(other.numerator)
			, denominator(other.denominator)
		{;}


		/** Copy assignment operator: */
		fraction& operator=(const fraction& other) noexcept {
			numerator = other.numerator;
			denominator = other.denominator;

			return *this;
		}


		/* If you're wondering why move c-tor and assignment are missing, it's because Arithmetic will always be a fundamental type. (C++14) */


		/** Performs the division and returns the resulting float.
		Returns infinity if the denominator == 0. */
		operator float() const noexcept {
			if (denominator == ZERO)
				return std::numeric_limits<float>::infinity();

			return static_cast<float>(numerator) / static_cast<float>(denominator);
		}


		/** Performs the division and returns the resulting double.
		Returns infinity if the denominator == 0. */
		operator double() const noexcept {
			if (denominator == ZERO)
				return std::numeric_limits<double>::infinity();

			return static_cast<double>(numerator) / static_cast<double>(denominator);
		}


		/** Performs the division and returns the resulting long double.
		Returns infinity if the denominator == 0. */
		operator long double() const noexcept {
			if (denominator == ZERO)
				return std::numeric_limits<long double>::infinity();

			return static_cast<long double>(numerator) / static_cast<long double>(denominator);
		}


		/** Performs the division and returns the resulting Arithmetic (gg::fraction<?>::type).
		If the denominator == 0, a gg::IllegalFraction will be thrown.
		Not declared as an operator, due to the possible conflicts with the ones up above. */
		Arithmetic toType() const {
			if (denominator == ZERO)
				throw IllegalFraction("May not execute toType() on gg::fraction-s with 0 in the denominator.");

			return numerator / denominator;
		}


		/** Returns the fraction as a string. */
		std::string toString() const noexcept {
			std::stringstream stream;
			stream << numerator << '/' << denominator;
			return stream.str();
		}

		/** Returns the fraction in it's reduced, beautified form as a string. */
		std::string humanize() const noexcept {
			fraction f = reduce();

			if (f.denominator == ONE || f.denominator == static_cast<Arithmetic>(-1)) {
				std::stringstream stream;
				stream << (f.denominator * f.numerator);
				return stream.str();
			}

			if (f.denominator != ZERO) {
				if (f.numerator == ZERO)
					return "0";
				if (f.numerator == f.denominator)
					return "1";
				if (f.numerator + f.denominator == ZERO)
					return "-1";
			}

			return f.toString();
		}


		/** Comparison operators */
		inline bool operator==(const fraction& other) const noexcept {
			return denominator != ZERO && other.denominator != ZERO && numerator * other.denominator == other.numerator * denominator;
		}

		inline bool operator!=(const fraction& other) const noexcept {
			return !this->operator==(other);
		}

		inline bool operator<(const fraction& other) const noexcept {
			return denominator != ZERO && other.denominator != ZERO && numerator * other.denominator < other.numerator * denominator;
		}

		inline bool operator>(const fraction& other) const noexcept {
			return denominator != ZERO && other.denominator != ZERO && numerator * other.denominator > other.numerator * denominator;
		}

		inline bool operator>=(const fraction& other) const noexcept {
			return denominator != ZERO && other.denominator != ZERO && numerator * other.denominator >= other.numerator * denominator;
		}

		inline bool operator<=(const fraction& other) const noexcept {
			return denominator != ZERO && other.denominator != ZERO && numerator * other.denominator <= other.numerator * denominator;
		}


		/** Unary sign change operator */
		inline fraction operator-() const noexcept {
			return fraction(-numerator, denominator);
		}


		/** Unary sign keep operator (basically replicates the original) */
		inline fraction operator+() const noexcept {
			return fraction(numerator, denominator);
		}


		/** Invert; returns a fraction containing denominator/numerator. */
		inline fraction reciprocal() const noexcept {
			return fraction(denominator, numerator);
		}


		/** Returns the fraction reduced to it's lowest terms. */
		fraction reduce() const noexcept {
			if (numerator == ZERO || denominator == ZERO)
				return fraction(*this);
			if (numerator == denominator)
				return fraction(ONE, ONE);

			Arithmetic divisor = gcd(numerator, denominator);
			if (divisor == ZERO)
				return fraction(*this);
			return fraction(numerator / divisor, denominator / divisor);
		}


		/** Addition to operator */
		fraction& operator+=(const fraction& other) {
			if (denominator == ZERO || other.denominator == ZERO)
				throw IllegalFraction("Operator+= is undefined when either of the denominators are 0.");

			if (numerator == ZERO) {
				*this = other;
			} else if (other.numerator == ZERO) {
				; // nothing
			} else if (denominator == other.denominator) {
				numerator += other.numerator;
			} else {
				numerator = numerator * other.denominator + other.numerator * denominator;
				denominator *= other.denominator;
			}

			return *this;
		}


		/** Subtraction from operator */
		fraction& operator-=(const fraction& other) {
			if (denominator == ZERO || other.denominator == ZERO)
				throw IllegalFraction("Operator-= is undefined when either of the denominators are 0.");

			if (numerator == ZERO) {
				*this = -other;
			} else if (other.numerator == ZERO) {
				; // nothing
			} else if (denominator == other.denominator) {
				numerator -= other.numerator;
			} else {
				numerator = numerator * other.denominator - other.numerator * denominator;
				denominator *= other.denominator;
			}

			return *this;
		}


		/** Multiplication by operator */
		fraction& operator*=(const fraction& other) noexcept {
			numerator *= other.numerator;
			denominator *= other.denominator;
			return *this;
		}


		/** Division by operator */
		fraction& operator/=(const fraction& other) noexcept {
			numerator *= other.denominator;
			denominator *= other.numerator;
			return *this;
		}


		/** Non-member addition operator */
		GG_FRACTIONS_FRIEND fraction operator+(const fraction& a, const fraction& b) {
			if (a.denominator == fraction::ZERO || b.denominator == fraction::ZERO)
				throw IllegalFraction("Operator+ is undefined when either of the denominators are 0.");

			fraction result;

			if (a.numerator == fraction::ZERO) {
				result = b;
			} else if (b.numerator == fraction::ZERO) {
				result = a; // nothing
			} else if (a.denominator == b.denominator) {
				result.numerator = a.numerator + b.numerator;
				result.denominator = a.denominator;
			} else {
				result.numerator = a.numerator * b.denominator + b.numerator * a.denominator;
				result.denominator = a.denominator * b.denominator;
			}

			return result;
		}


		/** Non-member subtraction operator */
		GG_FRACTIONS_FRIEND fraction operator-(const fraction& a, const fraction& b) {
			if (a.denominator == fraction::ZERO || b.denominator == fraction::ZERO)
				throw IllegalFraction("Operator- is undefined when either of the denominators are 0.");

			fraction result;

			if (a.numerator == fraction::ZERO) {
				result = -b;
			} else if (b.numerator == fraction::ZERO) {
				result = a; // nothing
			} else if (a.denominator == b.denominator) {
				result.numerator = a.numerator - b.numerator;
				result.denominator = a.denominator;
			} else {
				result.numerator = a.numerator * b.denominator - b.numerator * a.denominator;
				result.denominator = a.denominator * b.denominator;
			}

			return result;
		}


		/** Non-member multiplication operator */
		GG_FRACTIONS_FRIEND fraction operator*(const fraction& a, const fraction& b) noexcept {
			return fraction(a.numerator * b.numerator, a.denominator * b.denominator);
		}


		/** Non-member multiplication operator */
		GG_FRACTIONS_FRIEND fraction operator/(const fraction& a, const fraction& b) noexcept {
			return fraction(a.numerator * b.denominator, a.denominator * b.numerator);
		}
	};


} // namespace gg


/** Ostream operator for fractions. Outputs numerator/denominator. */
template <typename T>
std::ostream& operator<<(std::ostream& out, const gg::fraction<T>& f) {
	return out << f.numerator << '/' << f.denominator;
}


// this macro is not needed anymore
#undef GG_FRACTIONS_FRIEND

#endif // FRACTIONS_GG_FRACTIONS_HPP
