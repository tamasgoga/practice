#ifndef GG_ROLL_TEST_BASIC_HPP
#define GG_ROLL_TEST_BASIC_HPP


#include "test.hpp"
#include "../src/Roll.hpp"

#include <string>
#include <vector>


/* Generate a number of dice rolls from one Die */
void testDieRoll(Die::value_type type, int numOfRolls);

/* Test copy and assignment operations on dice */
void testCopyAssignDie(Die::value_type type, Die::value_type val);

/* Generate a Roll (object) and re-roll it (numOfRolls - 1) times */
void testRollObj(const std::vector<std::string>& command, unsigned int numOfRolls = 1);

/* Try to generate a faulty roll object */
void testRollObjCtorError();


#endif // GG_ROLL_TEST_BASIC_HPP