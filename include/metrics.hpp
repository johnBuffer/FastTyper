#pragma once

#include "rectangle.hpp"
#include "chart.hpp"

class MetricVisualizer : public Rectangle
{
public:
	MetricVisualizer();



private:
	Chart m_wpm;
	Chart m_accuracy;

};

