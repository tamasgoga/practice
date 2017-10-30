#ifndef GG_ROLL_DIE_HPP
#define GG_ROLL_DIE_HPP


//---------------------------------------------------
// class: Die
//---------------------------------------------------


#include <random>
#include <ostream>


/* Die with immutable properties, but assignable value */
class Die {
public:
	/* Type info */
	using value_type = unsigned int;

	/* Defines the max value of the die (die range is [1, type] inclusive) */
	const value_type type;

	/* C-tors:
	   1: Random initial value
	   2: Custom initial value that will be forced within the limits
	   3: Copy c-tor
	  * 1 & 2 throw based on validateType()
	**/
	Die(value_type type);
	Die(value_type type, value_type val);
	Die(const Die& other) noexcept;

	/* D-tor */
	virtual ~Die() {}

	/* Copy assignment operator */
	Die& operator=(const Die& other) noexcept {
		value = other.value;
		return *this;
	}

	/* Value assignment operator: calls set() */
	Die& operator=(value_type val) noexcept {
		set(val);
		return *this;
	}

	/* Comparison operators */
	bool operator==(const Die& other) const noexcept { return value == other.value; }
	bool operator!=(const Die& other) const noexcept { return value != other.value; }

	/* Returns the type if it can be used to construct a Die, otherwise throws domain_error */
	static value_type validateType(value_type type);

	/* Set the Die's value (will be forced withing the limits) */
	virtual void set(value_type val) noexcept;

	/* Get the Die's value */
	virtual value_type get() const noexcept {
		return value;
	}

	/* Generate a random value */
	virtual value_type roll();

	/* Non-member ostream operator for Die */
	friend std::ostream& operator<<(std::ostream& out, const Die& d);

protected:
	/* Value of the Die */
	value_type value;

	/* Random number generator */
	static std::mt19937 generator;
};


#endif // GG_ROLL_DIE_HPP
