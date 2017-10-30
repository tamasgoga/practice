#include "Die.hpp"

#include <string>
#include <stdexcept>

using namespace std;


static constexpr auto VAL_NIL = static_cast<Die::value_type>(0);
static constexpr auto VAL_ONE = static_cast<Die::value_type>(1);


Die::Die(value_type type)
	: type(validateType(type))
{
	roll();
}


Die::Die(value_type type, value_type val)
	: type(validateType(type))
{
	set(val);
}


Die::Die(const Die& other) noexcept
	: type(other.type)
	, value(other.value)
{}


Die::value_type Die::validateType(value_type type) {
	if (type <= VAL_NIL)
		throw domain_error(string("Die's type must be at least 1, instead of ") + to_string(type));
	return type;
}


void Die::set(value_type val) noexcept {
	if (val > type)
		val = type;
	else if (val < VAL_ONE)
		val = VAL_ONE;

	value = val;
}


Die::value_type Die::roll() {
	uniform_int_distribution<value_type> dist(VAL_ONE, type);
	value = dist(generator);
	return value;
}


ostream& operator<<(ostream& out, const Die& d) {
	out << d.value;
	return out;
}


/* Sys rand to seed the Mersenne Twister of Die */
static random_device sys_rand;
static seed_seq seed{sys_rand(), sys_rand(), sys_rand(), sys_rand(), sys_rand(), sys_rand(), sys_rand(), sys_rand()};

/* Init statics of Die */
mt19937 Die::generator(seed);
