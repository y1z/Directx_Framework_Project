#include "Timer.h"
#include <iostream>
#include <chrono>

Timer::Timer()
{	// this init this value 
	m_Result = std::chrono::seconds(0);
}

Timer::~Timer()
{
}

void Timer::StartTiming()
{
	m_Start = TimePointPicoSeconds::time_point::clock().now();
}

void Timer::EndTiming()
{
	m_End = TimePointPicoSeconds::time_point::clock().now();

	m_Result = (m_End - m_Start);

	//PrintResult();
}

void Timer::PrintResult()
{
	/*! Other Representations of time*/
	std::chrono::nanoseconds	ConverstionNano;
	TimeMeasurementSeconds ConverstionSeconds;

	// converting to respective times 
	ConverstionNano = std::chrono::duration_cast<std::chrono::nanoseconds>(m_Result);
	ConverstionSeconds = std::chrono::duration_cast<TimeMeasurementSeconds>(m_Result);

	printf("\n------------(TIMES)--------------\n");
	std::cout << "PicoSeconds = [" << m_Result.count() << "] \n";
	std::cout << "NanoSeconds = [" << ConverstionNano.count() << "] \n";
	std::cout << "Seconds = [" << ConverstionSeconds.count() << "] \n ";
	printf("--------------------------------\n\n");

}

uint64_t Timer::GetResult()
{
	return m_Result.count();
}

uint64_t Timer::GetResultMicroSeconds()
{
	std::chrono::microseconds ElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(m_Result);

	return ElapsedTime.count();
}

uint64_t Timer::GetResultMiliSeconds()
{
	std::chrono::milliseconds Milisecons = std::chrono::duration_cast<std::chrono::milliseconds>(m_Result);
	return Milisecons.count();
}

double Timer::GetResultSeconds()
{
	TimeMeasurementSeconds ConverstionSeconds = std::chrono::duration_cast<TimeMeasurementSeconds>(m_Result);

	return ConverstionSeconds.count();
}
