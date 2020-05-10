#include "testingClass.h"

testingClass::testingClass()
{

}

testingClass::testingClass(int a)
{
	this->num = a;
}

testingClass::~testingClass()
{

}

int testingClass::getNum()
{
	return this->num;
}