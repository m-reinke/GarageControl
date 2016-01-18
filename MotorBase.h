#pragma once
/*
GarageMotor.h - Library for a Garage opener.
*/
#ifndef MOTORBASE_H
#define MOTORBASE_H


/// <summary>
/// Abstract interface for the Garage Motor class
/// </summary>
class MotorBase
{
public:
	/// <summary>
	/// Opens the garage.
	/// </summary>
	virtual void Open() = 0;

	/// <summary>
	/// Closes the garage.
	/// </summary>
	virtual void Close() = 0;

	/// <summary>
	/// Determines whether an open or closed can be triggered at this moment.
	/// </summary>
	/// <returns></returns>
	virtual bool canTrigger() = 0;
};

#endif



